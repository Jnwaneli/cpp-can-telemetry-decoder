#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include "can_frame.hpp"

std::uint16_t pack_u16(std::uint8_t low, std::uint8_t high) {
    return static_cast<std::uint16_t>(low) |
           (static_cast<std::uint16_t>(high) << 8);
}

bool get_bit(std::uint8_t value, int bit_position) {
    return (value & (1 << bit_position)) != 0;
}

void decodeAnalogInputs(const CanFrame& frame) {
    if (frame.dlc < 6) {
        std::cout << "Frame does not have enough bytes to decode AIN1, AIN2, and AIN3." << std::endl;
        return;
    }

    std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
    std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);

    std::cout << "Decoded analog inputs:" << std::endl;
    std::cout << "AIN1_RAW: " << ain1 << " decimal, 0x" << std::hex << ain1 << std::dec << std::endl;
    std::cout << "AIN2_RAW: " << ain2 << " decimal, 0x" << std::hex << ain2 << std::dec << std::endl;
    std::cout << "AIN3_RAW: " << ain3 << " decimal, 0x" << std::hex << ain3 << std::dec << std::endl;
}

void printFlags(std::uint8_t flags) {
    std::cout << "Flags byte: 0x"
              << std::hex
              << static_cast<int>(flags)
              << std::dec
              << std::endl;

    std::cout << "Bit 0: " << get_bit(flags, 0) << std::endl;
    std::cout << "Bit 1: " << get_bit(flags, 1) << std::endl;
    std::cout << "Bit 2: " << get_bit(flags, 2) << std::endl;
    std::cout << "Bit 3: " << get_bit(flags, 3) << std::endl;
}

void reverseString(std::vector<char>& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;

    while (left < right) {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;

        left++;
        right--;
    }
}

void printCharVector(const std::vector<char>& chars) {
    for (char c : chars) {
        std::cout << c;
    }

    std::cout << std::endl;
}

int main() {
    CanFrame frame = {
        0x100,
        8,
        {0x34, 0x12, 0x78, 0x56, 0xBC, 0x9A, 0b00000101, 0x01}
    };

    print_frame(frame);

    std::cout << std::endl;
    decodeAnalogInputs(frame);

    std::cout << std::endl;
    printFlags(frame.data[6]);

    std::cout << std::endl;

    std::vector<char> word = {'h', 'e', 'l', 'l', 'o'};

    std::cout << "Before reverse: ";
    printCharVector(word);

    reverseString(word);

    std::cout << "After reverse: ";
    printCharVector(word);

    return 0;
}