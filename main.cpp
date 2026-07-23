#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "can_dispatcher.hpp"
#include "can_frame.hpp"
#include "can_log_parser.hpp"
#include "can_validation.hpp"
#include "circular_buffer.hpp"
#include "decoder_stats.hpp"
#include "parser_state.hpp"
#include "telemetry_decoder.hpp"

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
        {0x100, 8, {0x34, 0x12, 0x78, 0x05, 0x21, 0x09, 0x07, 0x03}},
        {0x200, 8, {0xD2, 0x04, 0xAC, 0x0D, 0x03, 0x2D, 0x00, 0x04}},
        {0x101, 8, {0xC8, 0x32, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x999, 8, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}},
        {0x100, 4, {0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}}
    };
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
    std::cout << "Frame ID: 0x"
              << std::hex
              << frame.id
              << std::dec
              << std::endl;

    std::cout << "Frame Name: "
              << frame_type_name(frame.id)
              << std::endl;

    std::cout << "DLC: "
              << static_cast<int>(frame.dlc)
              << std::endl;

    print_payload(frame);
}

void process_frame(const CanFrame& frame, CanDispatcher& dispatcher, DecoderStats& stats) {
    print_parser_state(ParserState::WAIT_FOR_FRAME);

    stats.record_frame_received();

    std::cout << "------------------------------" << std::endl;

    print_frame_header(frame);

    std::vector<std::string> faults;

    print_parser_state(ParserState::VALIDATE_ID);

    if (!is_known_id(frame.id)) {
        faults.push_back("Unknown CAN ID");
        stats.record_unknown_id();
    }

    print_parser_state(ParserState::VALIDATE_DLC);

    if (!has_valid_dlc(frame)) {
        faults.push_back("Invalid DLC");
        stats.record_invalid_dlc();
    }

    if (!faults.empty()) {
        print_parser_state(ParserState::PRINT_RESULT);

        std::cout << "Frame Validation: FAULT" << std::endl;

        for (const std::string& fault : faults) {
            std::cout << "FAULT: " << fault << std::endl;
        }

        return;
    }

    stats.record_valid_frame();

    print_parser_state(ParserState::DECODE);

    std::size_t decoded_faults = dispatcher.dispatch(frame);

    print_parser_state(ParserState::ANALYZE_FAULTS);

    stats.add_faults(decoded_faults);

    print_parser_state(ParserState::PRINT_RESULT);

    if (decoded_faults > 0) {
        std::cout << "Decoded Faults: "
                  << decoded_faults
                  << std::endl;
    }

    std::cout << "Frame Validation: OK" << std::endl;
}

void load_frames_into_buffer(const std::vector<CanFrame>& log, CircularBuffer& rx_buffer) {
    std::cout << "Loading CAN frames into RX buffer:" << std::endl;

    for (const CanFrame& frame : log) {
        bool pushed = rx_buffer.push(frame);

        if (pushed) {
            std::cout << "Pushed frame ID 0x"
                      << std::hex
                      << frame.id
                      << std::dec
                      << ". Buffer size: "
                      << rx_buffer.size()
                      << std::endl;
        } else {
            std::cout << "RX buffer full. Dropped frame ID 0x"
                      << std::hex
                      << frame.id
                      << std::dec
                      << std::endl;
        }
    }

    std::cout << std::endl;
}

void process_buffered_frames(CircularBuffer& rx_buffer,
                             CanDispatcher& dispatcher,
                             DecoderStats& stats) {
    std::cout << "Processing buffered CAN frames:" << std::endl;

    CanFrame frame{};

    while (rx_buffer.pop(frame)) {
        process_frame(frame, dispatcher, stats);
    }

    std::cout << "------------------------------" << std::endl;

    if (rx_buffer.is_empty()) {
        std::cout << "RX buffer is now empty." << std::endl;
    }
}

void run_decoder_pipeline(const std::vector<CanFrame>& can_log) {
    CircularBuffer rx_buffer;
    TelemetryDecoder decoder;
    CanDispatcher dispatcher(decoder);
    DecoderStats stats;

    load_frames_into_buffer(can_log, rx_buffer);
    process_buffered_frames(rx_buffer, dispatcher, stats);

    std::cout << "Decoder frames seen: "
              << decoder.frames_seen()
              << std::endl;

    stats.print();
}

int main() {
    std::vector<CanFrame> can_log = load_can_log_from_csv("data/sample_can_log.csv");

    if (can_log.empty()) {
        std::cout << "Using fallback built-in CAN log."
                  << std::endl
                  << std::endl;

        can_log = create_fallback_can_log();
    }

    run_decoder_pipeline(can_log);

    return 0;
}