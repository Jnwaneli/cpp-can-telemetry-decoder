#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "bit_utils.hpp"
#include "can_frame.hpp"
#include "can_validation.hpp"
#include "circular_buffer.hpp"
#include "telemetry_decoder.hpp"

void bitExperiment() {
    std::cout << "Bit experiment:" << std::endl;

    std::uint8_t value = 0x00;
    std::uint8_t mask = 0x04;  // bit 2

    std::cout << "Initial value: 0x"
              << std::hex
              << static_cast<int>(value)
              << std::dec
              << std::endl;

    value |= mask;

    std::cout << "After set bit 2: 0x"
              << std::hex
              << static_cast<int>(value)
              << std::dec
              << std::endl;

    bool bit_2_set = get_bit(value, 2);

    std::cout << "Bit 2 set? "
              << (bit_2_set ? "yes" : "no")
              << std::endl;

    value ^= mask;

    std::cout << "After toggle bit 2: 0x"
              << std::hex
              << static_cast<int>(value)
              << std::dec
              << std::endl;

    value |= mask;

    std::cout << "After set bit 2 again: 0x"
              << std::hex
              << static_cast<int>(value)
              << std::dec
              << std::endl;

    value &= static_cast<std::uint8_t>(~mask);

    std::cout << "After clear bit 2: 0x"
              << std::hex
              << static_cast<int>(value)
              << std::dec
              << std::endl;

    std::uint8_t status = 0x85;      // 1000 0101
    std::uint8_t error_mask = 0x80;  // 1000 0000

    std::cout << "Status byte: 0x"
              << std::hex
              << static_cast<int>(status)
              << std::dec
              << std::endl;

    std::cout << "Error mask set? "
              << (is_mask_set(status, error_mask) ? "yes" : "no")
              << std::endl;

    std::cout << std::endl;
}

void arrayExperiment() {
    std::cout << "Array experiment:" << std::endl;

    std::uint8_t raw_data[8] = {
        0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05
    };

    std::array<std::uint8_t, 8> cpp_data = {
        0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05
    };

    std::cout << "Raw array first byte: 0x"
              << std::hex
              << static_cast<int>(raw_data[0])
              << std::dec
              << std::endl;

    std::cout << "std::array first byte: 0x"
              << std::hex
              << static_cast<int>(cpp_data[0])
              << std::dec
              << std::endl;

    std::cout << "Raw array size using sizeof: "
              << sizeof(raw_data) / sizeof(raw_data[0])
              << std::endl;

    std::cout << "std::array size using .size(): "
              << cpp_data.size()
              << std::endl;

    std::cout << std::endl;
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

void process_frame(const CanFrame& frame, TelemetryDecoder& decoder) {
    std::cout << "------------------------------" << std::endl;

    print_frame_header(frame);

    std::vector<std::string> faults;

    if (!is_known_id(frame.id)) {
        faults.push_back("Unknown CAN ID");
    }

    if (!has_valid_dlc(frame)) {
        faults.push_back("Invalid DLC");
    }

    if (!faults.empty()) {
        std::cout << "Frame Validation: FAULT" << std::endl;

        for (const std::string& fault : faults) {
            std::cout << "FAULT: " << fault << std::endl;
        }

        return;
    }

    decoder.decode(frame);

    std::cout << "Frame Validation: OK" << std::endl;
}

void load_frames_into_buffer(const std::vector<CanFrame>& log, CircularBuffer& rx_buffer) {
    std::cout << "Loading simulated CAN frames into RX buffer:" << std::endl;

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

void process_buffered_frames(CircularBuffer& rx_buffer, TelemetryDecoder& decoder) {
    std::cout << "Processing buffered CAN frames:" << std::endl;

    CanFrame frame{};

    while (rx_buffer.pop(frame)) {
        process_frame(frame, decoder);
    }

    std::cout << "------------------------------" << std::endl;

    if (rx_buffer.is_empty()) {
        std::cout << "RX buffer is now empty." << std::endl;
    }
}

int main() {
    bitExperiment();
    arrayExperiment();

    std::vector<CanFrame> simulated_log = {
        // status 0x00 = sensor 1, 2, and 3 invalid
        {0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x00, 0x05}},

        // 12600 mV = 12.60 V
        // 345 deciC = 34.5 C
        {0x101, 8, {0x38, 0x31, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00}},

        // sensor valid flags = 0x07, system fault flags = 0x00, mode = 1, error code = 0
        {0x102, 8, {0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}},

        {0x999, 8, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}},
        {0x100, 4, {0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}}
    };

    CircularBuffer rx_buffer;
    TelemetryDecoder decoder;

    load_frames_into_buffer(simulated_log, rx_buffer);
    process_buffered_frames(rx_buffer, decoder);

    std::cout << "Decoder frames seen: "
              << decoder.frames_seen()
              << std::endl;

    return 0;
}