#include "telemetry_decoder.hpp"
#include "bit_utils.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

void TelemetryDecoder::decode(const CanFrame& frame) const {
    switch (frame.id) {
        case 0x100:
            decode_0x100(frame);
            break;

        default:
            std::cout << "No decoder implemented for CAN ID 0x"
                      << std::hex
                      << frame.id
                      << std::dec
                      << std::endl;
            break;
    }
}

void TelemetryDecoder::decode_0x100(const CanFrame& frame) const {
    if (frame.dlc < 8) {
        std::cout << "Cannot decode 0x100: invalid DLC" << std::endl;
        return;
    }

    std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
    std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);

    std::uint8_t status = frame.data[6];
    std::uint8_t counter = frame.data[7];

    std::cout << "Decoded 0x100 analog inputs:" << std::endl;

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