#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>

#include "can_frame.hpp"
#include "can_validation.hpp"
#include "bit_utils.hpp"
#include "circular_buffer.hpp"

void decode_analog_inputs(const CanFrame& frame) {
    if (frame.id != 0x100) {
        return;
    }

    if (frame.dlc < 8) {
        std::cout << "Cannot decode analog inputs: invalid DLC" << std::endl;
        return;
    }

    std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
    std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);

    std::uint8_t status = frame.data[6];
    std::uint8_t counter = frame.data[7];

    std::cout << "Decoded analog inputs:" << std::endl;

    std::cout << "AIN1: " << ain1
              << " decimal, 0x" << std::hex << ain1 << std::dec
              << std::endl;

    std::cout << "AIN2: " << ain2
              << " decimal, 0x" << std::hex << ain2 << std::dec
              << std::endl;

    std::cout << "AIN3: " << ain3
              << " decimal, 0x" << std::hex << ain3 << std::dec
              << std::endl;

    std::cout << "Status byte: 0x"
              << std::hex
              << static_cast<int>(status)
              << std::dec
              << std::endl;

    std::cout << "Counter: "
              << static_cast<int>(counter)
              << std::endl;
}

void process_frame(const CanFrame& frame) {
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

        if (frame.id == 0x100) {
            decode_analog_inputs(frame);
        }
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

void process_buffered_frames(CircularBuffer& rx_buffer) {
    std::cout << "Processing buffered CAN frames:" << std::endl;
    std::cout << "===============================" << std::endl;

    CanFrame frame{};

    while (rx_buffer.pop(frame)) {
        process_frame(frame);
        std::cout << std::endl;
    }

    if (rx_buffer.is_empty()) {
        std::cout << "RX buffer is now empty." << std::endl;
    }
}

int main() {
    std::vector<CanFrame> simulated_log = {
        {0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05}},
        {0x101, 8, {0x88, 0x13, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x102, 8, {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0x999, 8, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}},
        {0x100, 4, {0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}}
    };

    CircularBuffer rx_buffer;

    load_frames_into_buffer(simulated_log, rx_buffer);
    process_buffered_frames(rx_buffer);

    return 0;
}