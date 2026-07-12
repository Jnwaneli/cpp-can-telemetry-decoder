#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "can_frame.hpp"
#include "can_validation.hpp"
#include "circular_buffer.hpp"
#include "telemetry_decoder.hpp"

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

void process_frame(const CanFrame& frame, TelemetryDecoder& decoder) {
    print_frame(frame);

    bool has_fault = false;

    if (!is_known_id(frame.id)) {
        std::cout << "FAULT: Unknown CAN ID" << std::endl;
        has_fault = true;
    }

    if (!has_valid_dlc(frame)) {
        std::cout << "FAULT: Invalid DLC" << std::endl;
        has_fault = true;
    }

    if (!has_fault) {
        std::cout << "Frame status: OK" << std::endl;
        decoder.decode(frame);
    }
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
    std::cout << "===============================" << std::endl;

    CanFrame frame{};

    while (rx_buffer.pop(frame)) {
        process_frame(frame, decoder);
        std::cout << std::endl;
    }

    if (rx_buffer.is_empty()) {
        std::cout << "RX buffer is now empty." << std::endl;
    }
}

int main() {
    arrayExperiment();

    std::vector<CanFrame> simulated_log = {
        {0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05}},
        {0x101, 8, {0x88, 0x13, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x102, 8, {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
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