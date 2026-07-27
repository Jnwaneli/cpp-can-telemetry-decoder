#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "can_dispatcher.hpp"
#include "can_frame.hpp"
#include "can_validation.hpp"
#include "circular_buffer.hpp"
#include "csv_frame_source.hpp"
#include "decoder_stats.hpp"
#include "fault_analyzer.hpp"
#include "fault_report.hpp"
#include "fault_summary_writer.hpp"
#include "frame_report.hpp"
#include "frame_source.hpp"
#include "parser_state.hpp"
#include "telemetry_decoder.hpp"
#include "waveshare_serial_frame_source.hpp"

namespace {
const std::string FAULT_SUMMARY_OUTPUT_PATH = "output/fault_summary.json";
}

void print_parser_state(ParserState state) {
    std::cout << "Parser State: "
              << parser_state_name(state)
              << std::endl;
}

std::vector<CanFrame> create_fallback_can_log() {
    return {
        {0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x07, 0x01}},
        {0x101, 8, {0x38, 0x31, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x102, 8, {0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0x100, 8, {0x20, 0x03, 0x40, 0x06, 0x80, 0x09, 0x07, 0x02}},
        {0x101, 8, {0xE0, 0x2E, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x100, 8, {0x34, 0x0C, 0x78, 0x05, 0x21, 0x09, 0x07, 0x04}},
        {0x200, 8, {0xD2, 0x04, 0xAC, 0x0D, 0x03, 0x2D, 0x00, 0x04}},
        {0x101, 8, {0xC8, 0x32, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x999, 8, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}},
        {0x100, 4, {0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}}
    };
}

std::string format_can_id(std::uint32_t id) {
    std::ostringstream output;

    output << "0x"
           << std::hex
           << std::uppercase
           << id;

    return output.str();
}

std::string frame_type_name(std::uint32_t id) {
    static const std::unordered_map<std::uint32_t, std::string> frame_names = {
        {0x100, "Analog Inputs"},
        {0x101, "Battery and Temperature"},
        {0x102, "Status Flags"},
        {0x200, "Vehicle Telemetry"}
    };

    auto it = frame_names.find(id);

    if (it != frame_names.end()) {
        return it->second;
    }

    return "Unknown";
}

std::size_t count_faults(const std::vector<FaultReport>& reports) {
    std::size_t count = 0;

    for (const FaultReport& report : reports) {
        if (report.has_fault && report.severity == FaultSeverity::Fault) {
            count++;
        }
    }

    return count;
}

std::size_t count_warnings(const std::vector<FaultReport>& reports) {
    std::size_t count = 0;

    for (const FaultReport& report : reports) {
        if (report.severity == FaultSeverity::Warning) {
            count++;
        }
    }

    return count;
}

void record_reports_in_summary(const std::vector<FaultReport>& reports,
                               FaultAnalyzer& fault_summary_tracker) {
    for (const FaultReport& report : reports) {
        fault_summary_tracker.record_report(report);
    }
}

void add_report_messages(const std::vector<FaultReport>& reports,
                         FrameReport& frame_report) {
    for (const FaultReport& report : reports) {
        if (report.severity == FaultSeverity::Warning) {
            frame_report.warning_messages.push_back(report.message);
        } else if (report.has_fault) {
            frame_report.fault_messages.push_back(report.message);
            frame_report.ok = false;
        }
    }
}

void print_payload(const CanFrame& frame) {
    std::cout << "Payload: ";

    for (int i = 0; i < frame.dlc && i < 8; i++) {
        std::cout << "0x"
                  << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(frame.data[i])
                  << " ";
    }

    std::cout << std::dec
              << std::setfill(' ')
              << std::endl;
}

void print_frame_header(const CanFrame& frame) {
    std::cout << "Frame ID: "
              << format_can_id(frame.id)
              << std::endl;

    std::cout << "Frame Name: "
              << frame_type_name(frame.id)
              << std::endl;

    std::cout << "DLC: "
              << static_cast<int>(frame.dlc)
              << std::endl;

    print_payload(frame);
}

void print_frame_report(const FrameReport& report) {
    std::cout << "Frame "
              << report.frame_number
              << ": "
              << format_can_id(report.can_id)
              << " "
              << report.frame_name
              << " - ";

    if (report.ok) {
        std::cout << "OK";
    } else {
        std::cout << "FAULT";

        if (!report.fault_messages.empty()) {
            std::cout << ": ";

            for (std::size_t i = 0; i < report.fault_messages.size(); i++) {
                std::cout << report.fault_messages[i];

                if (i + 1 < report.fault_messages.size()) {
                    std::cout << "; ";
                }
            }
        }
    }

    if (!report.warning_messages.empty()) {
        std::cout << "; WARNING: ";

        for (std::size_t i = 0; i < report.warning_messages.size(); i++) {
            std::cout << report.warning_messages[i];

            if (i + 1 < report.warning_messages.size()) {
                std::cout << "; ";
            }
        }
    }

    std::cout << std::endl;
}

FrameReport process_frame(std::size_t frame_number,
                          const CanFrame& frame,
                          CanDispatcher& dispatcher,
                          DecoderStats& stats,
                          FaultAnalyzer& fault_summary_tracker) {
    FrameReport report{
        frame_number,
        frame.id,
        frame_type_name(frame.id),
        true,
        {},
        {}
    };

    print_parser_state(ParserState::WAIT_FOR_FRAME);

    stats.record_frame_received();

    std::cout << "------------------------------" << std::endl;

    print_frame_header(frame);

    std::vector<FaultReport> validation_reports;

    print_parser_state(ParserState::VALIDATE_ID);

    if (!is_known_id(frame.id)) {
        validation_reports.push_back({
            true,
            "Unknown CAN ID",
            FaultSeverity::Fault,
            FaultCategory::UnknownId
        });

        stats.record_unknown_id();
    }

    print_parser_state(ParserState::VALIDATE_DLC);

    if (!has_valid_dlc(frame)) {
        validation_reports.push_back({
            true,
            "Invalid DLC",
            FaultSeverity::Fault,
            FaultCategory::InvalidDlc
        });

        stats.record_invalid_dlc();
    }

    if (!validation_reports.empty()) {
        add_report_messages(validation_reports, report);
        record_reports_in_summary(validation_reports, fault_summary_tracker);

        print_parser_state(ParserState::PRINT_RESULT);
        print_frame_report(report);

        return report;
    }

    stats.record_valid_frame();

    print_parser_state(ParserState::DECODE);

    std::vector<FaultReport> decoded_reports = dispatcher.dispatch(frame);

    print_parser_state(ParserState::ANALYZE_FAULTS);

    std::size_t decoded_fault_count = count_faults(decoded_reports);
    std::size_t decoded_warning_count = count_warnings(decoded_reports);

    stats.add_faults(decoded_fault_count);
    stats.add_warnings(decoded_warning_count);

    add_report_messages(decoded_reports, report);
    record_reports_in_summary(decoded_reports, fault_summary_tracker);

    print_parser_state(ParserState::PRINT_RESULT);
    print_frame_report(report);

    return report;
}

void load_frames_into_buffer(const std::vector<CanFrame>& log, CircularBuffer& rx_buffer) {
    std::cout << "Loading CAN frames into RX buffer:" << std::endl;

    for (const CanFrame& frame : log) {
        bool pushed = rx_buffer.push(frame);

        if (pushed) {
            std::cout << "Pushed frame ID "
                      << format_can_id(frame.id)
                      << ". Buffer size: "
                      << rx_buffer.size()
                      << std::endl;
        } else {
            std::cout << "RX buffer full. Dropped frame ID "
                      << format_can_id(frame.id)
                      << std::endl;
        }
    }

    std::cout << std::endl;
}

std::vector<FrameReport> process_buffered_frames(CircularBuffer& rx_buffer,
                                                 CanDispatcher& dispatcher,
                                                 DecoderStats& stats,
                                                 FaultAnalyzer& fault_summary_tracker) {
    std::cout << "Processing buffered CAN frames:" << std::endl;

    std::vector<FrameReport> reports;
    CanFrame frame{};
    std::size_t frame_number = 1;

    while (rx_buffer.pop(frame)) {
        FrameReport report =
            process_frame(frame_number, frame, dispatcher, stats, fault_summary_tracker);

        reports.push_back(report);
        frame_number++;
    }

    std::cout << "------------------------------" << std::endl;

    if (rx_buffer.is_empty()) {
        std::cout << "RX buffer is now empty." << std::endl;
    }

    return reports;
}

std::vector<FrameReport> process_live_frames(FrameSource& source,
                                             CanDispatcher& dispatcher,
                                             DecoderStats& stats,
                                             FaultAnalyzer& fault_summary_tracker,
                                             std::size_t max_frames = 0) {
    std::cout << "Processing CAN frames in live-style mode:" << std::endl;

    std::vector<FrameReport> reports;
    CanFrame frame{};
    std::size_t frame_number = 1;

    while (source.read_next(frame)) {
        FrameReport report =
            process_frame(frame_number, frame, dispatcher, stats, fault_summary_tracker);

        reports.push_back(report);
        frame_number++;

        if (max_frames != 0 && reports.size() >= max_frames) {
            std::cout << "Reached live frame limit: "
                      << max_frames
                      << std::endl;
            break;
        }
    }

    std::cout << "------------------------------" << std::endl;
    std::cout << "Live-style frame source finished." << std::endl;

    return reports;
}

void print_summary_report(const std::vector<FrameReport>& reports,
                          const DecoderStats& stats,
                          const FaultAnalyzer& fault_summary_tracker) {
    std::cout << std::endl;
    std::cout << "Decoded Frame Report:" << std::endl;

    for (const FrameReport& report : reports) {
        print_frame_report(report);
    }

    std::cout << std::endl;
    std::cout << "Summary:" << std::endl;
    stats.print();

    std::cout << std::endl;
    fault_summary_tracker.print_summary();
}

void write_summary_output(const std::string& source_name,
                          const DecoderStats& stats,
                          const FaultAnalyzer& fault_summary_tracker) {
    write_fault_summary_json(FAULT_SUMMARY_OUTPUT_PATH,
                             source_name,
                             stats,
                             fault_summary_tracker);
}

void run_decoder_pipeline(const std::vector<CanFrame>& can_log,
                          const std::string& source_name) {
    CircularBuffer rx_buffer;
    TelemetryDecoder decoder;
    CanDispatcher dispatcher(decoder);
    DecoderStats stats;
    FaultAnalyzer fault_summary_tracker;

    load_frames_into_buffer(can_log, rx_buffer);

    std::vector<FrameReport> reports =
        process_buffered_frames(rx_buffer, dispatcher, stats, fault_summary_tracker);

    std::cout << "Decoder frames seen: "
              << decoder.frames_seen()
              << std::endl;

    decoder.print_signal_stats();

    print_summary_report(reports, stats, fault_summary_tracker);
    write_summary_output(source_name, stats, fault_summary_tracker);
}

void run_decoder_live(FrameSource& source,
                      const std::string& source_name,
                      std::size_t max_frames = 0) {
    TelemetryDecoder decoder;
    CanDispatcher dispatcher(decoder);
    DecoderStats stats;
    FaultAnalyzer fault_summary_tracker;

    std::vector<FrameReport> reports =
        process_live_frames(source, dispatcher, stats, fault_summary_tracker, max_frames);

    std::cout << "Decoder frames seen: "
              << decoder.frames_seen()
              << std::endl;

    decoder.print_signal_stats();

    print_summary_report(reports, stats, fault_summary_tracker);
    write_summary_output(source_name, stats, fault_summary_tracker);
}

void print_usage(const std::string& program_name) {
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << program_name << std::endl;
    std::cout << "  " << program_name << " --csv <path>" << std::endl;
    std::cout << "  " << program_name << " --waveshare-serial <COM_PORT> [max_frames]" << std::endl;
    std::cout << std::endl;

    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program_name << " --csv data/sample_can_log.csv" << std::endl;
    std::cout << "  " << program_name << " --waveshare-serial COM4 100" << std::endl;
    std::cout << std::endl;
    std::cout << "Each successful run writes "
              << FAULT_SUMMARY_OUTPUT_PATH
              << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        CsvFrameSource source("data/sample_can_log.csv");

        if (!source.is_open()) {
            std::cout << "Using fallback built-in CAN log."
                      << std::endl
                      << std::endl;

            std::vector<CanFrame> fallback_log = create_fallback_can_log();
            run_decoder_pipeline(fallback_log, "fallback_log");

            return 0;
        }

        run_decoder_live(source, "csv_default");
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "--help" || mode == "-h") {
        print_usage(argv[0]);
        return 0;
    }

    if (mode == "--csv") {
        std::string path = "data/sample_can_log.csv";

        if (argc >= 3) {
            path = argv[2];
        }

        CsvFrameSource source(path);

        if (!source.is_open()) {
            std::cout << "CSV source could not be opened." << std::endl;
            return 1;
        }

        run_decoder_live(source, "csv_log");
        return 0;
    }

    if (mode == "--waveshare-serial") {
        if (argc < 3) {
            std::cout << "Missing COM port." << std::endl;
            print_usage(argv[0]);
            return 1;
        }

        std::string port_name = argv[2];
        std::size_t max_frames = 100;

        if (argc >= 4) {
            max_frames = static_cast<std::size_t>(std::stoull(argv[3]));
        }

        WaveshareSerialFrameSource source(port_name);

        if (!source.is_open()) {
            std::cout << "Waveshare serial source could not be opened." << std::endl;
            return 1;
        }

        run_decoder_live(source, "waveshare_live", max_frames);
        return 0;
    }

    std::cout << "Unknown mode: "
              << mode
              << std::endl
              << std::endl;

    print_usage(argv[0]);

    return 1;
}
