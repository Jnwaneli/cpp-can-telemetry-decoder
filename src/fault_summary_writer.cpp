#include "fault_summary_writer.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {
std::string escape_json_string(const std::string& text) {
    std::string escaped;

    for (char ch : text) {
        switch (ch) {
            case '\\':
                escaped += "\\\\";
                break;
            case '"':
                escaped += "\\\"";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped += ch;
                break;
        }
    }

    return escaped;
}
}

bool write_fault_summary_json(const std::string& output_path,
                              const std::string& source_name,
                              const DecoderStats& stats,
                              const FaultAnalyzer& fault_summary_tracker) {
    std::filesystem::path path(output_path);

    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream output_file(output_path);

    if (!output_file) {
        std::cout << "Failed to write fault summary JSON: "
                  << output_path
                  << std::endl;

        return false;
    }

    output_file << "{\n";
    output_file << "  \"source\": \"" << escape_json_string(source_name) << "\",\n";
    output_file << "  \"frames_processed\": " << stats.total_frames() << ",\n";
    output_file << "  \"valid_frames\": " << stats.valid_frames() << ",\n";
    output_file << "  \"invalid_dlc\": " << stats.invalid_dlc_frames() << ",\n";
    output_file << "  \"unknown_ids\": " << stats.unknown_id_frames() << ",\n";
    output_file << "  \"faults\": " << stats.fault_count() << ",\n";
    output_file << "  \"warnings\": " << stats.warning_count() << ",\n";
    output_file << "  \"low_voltage_faults\": " << fault_summary_tracker.low_voltage_fault_count() << ",\n";
    output_file << "  \"high_voltage_faults\": " << fault_summary_tracker.high_voltage_fault_count() << ",\n";
    output_file << "  \"voltage_faults\": " << fault_summary_tracker.voltage_fault_count() << ",\n";
    output_file << "  \"high_temperature_faults\": " << fault_summary_tracker.temperature_fault_count() << ",\n";
    output_file << "  \"sensor_invalid_faults\": " << fault_summary_tracker.sensor_invalid_count() << ",\n";
    output_file << "  \"dropped_frames\": " << fault_summary_tracker.dropped_frame_count() << ",\n";
    output_file << "  \"possible_stuck_sensors\": " << fault_summary_tracker.stuck_sensor_warning_count() << ",\n";
    output_file << "  \"other_faults\": " << fault_summary_tracker.other_fault_count() << "\n";
    output_file << "}\n";

    std::cout << "Wrote fault summary JSON: "
              << output_path
              << std::endl;

    return true;
}
