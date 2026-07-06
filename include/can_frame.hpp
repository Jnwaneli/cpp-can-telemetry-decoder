#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

struct CanFrame {
    std::uint32_t id;
    std::uint8_t dlc;
    std::array<std::uint8_t, 8> data;
};

inline void print_frame(const CanFrame& frame) {
    std::cout << "CAN ID: 0x" << std::hex << frame.id << std::dec << std::endl;
    std::cout << "DLC: " << static_cast<int>(frame.dlc) << std::endl;

    std::cout << "Data bytes: ";

    for (int i = 0; i < frame.dlc; i++) {
        std::cout << "0x"
                  << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(frame.data[i])
                  << " ";
    }

    std::cout << std::dec << std::setfill(' ') << std::endl;
}