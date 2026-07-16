#include "telemetry_decoder.hpp"
#include "bit_utils.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

namespace {
constexpr std::uint8_t SENSOR1_VALID_MASK = 0x01;
constexpr std::uint8_t SENSOR2_VALID_MASK = 0x02;
constexpr std::uint8_t SENSOR3_VALID_MASK = 0x04;
constexpr std::uint8_t ERROR_FLAG_MASK = 0x80;
}

TelemetryDecoder::TelemetryDecoder()
    : frames_seen_(0) {
}

void TelemetryDecoder::decode(const CanFrame& frame) {
    frames_seen_++;

    switch (frame.id) {
        case 0x100:
            decode_0x100(frame);
            break;

        case 0x101:
            decode_0x101(frame);
            break;

        default:
            std::cout << "Type: Known frame, decoder not implemented yet" << std::endl;
            break;
    }
}

std::size_t TelemetryDecoder::frames_seen() const {
    return frames_seen_;
}

void TelemetryDecoder::decode_0x100(const CanFrame& frame) {
    if (frame.dlc < 8) {
        std::cout << "Type: Analog Inputs" << std::endl;
        std::cout << "Result: FAULT - Invalid DLC for 0x100 decoder" << std::endl;
        return;
    }

    std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
    std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);

    std::uint8_t status = frame.data[6];
    std::uint8_t counter = frame.data[7];

    bool sensor1_valid = is_mask_set(status, SENSOR1_VALID_MASK);
    bool sensor2_valid = is_mask_set(status, SENSOR2_VALID_MASK);
    bool sensor3_valid = is_mask_set(status, SENSOR3_VALID_MASK);
    bool error_flag_set = is_mask_set(status, ERROR_FLAG_MASK);

    std::cout << "Type: Analog Inputs" << std::endl;

    std::cout << "AIN1_RAW: "
              << ain1
              << std::endl;

    std::cout << "AIN2_RAW: "
              << ain2
              << std::endl;

    std::cout << "AIN3_RAW: "
              << ain3
              << std::endl;

    std::cout << "Status: 0x"
              << std::hex
              << std::setw(2)
              << std::setfill('0')
              << static_cast<int>(status)
              << std::dec
              << std::setfill(' ')
              << std::endl;

    std::cout << "Sensor1_VALID: "
              << (sensor1_valid ? "yes" : "no")
              << std::endl;

    std::cout << "Sensor2_VALID: "
              << (sensor2_valid ? "yes" : "no")
              << std::endl;

    std::cout << "Sensor3_VALID: "
              << (sensor3_valid ? "yes" : "no")
              << std::endl;

    std::cout << "Error_Flag: "
              << (error_flag_set ? "yes" : "no")
              << std::endl;

    std::cout << "Counter: "
              << static_cast<int>(counter)
              << std::endl;
}

void TelemetryDecoder::decode_0x101(const CanFrame& frame) {
    if (frame.dlc < 8) {
        std::cout << "Type: Battery and Temperature" << std::endl;
        std::cout << "Result: FAULT - Invalid DLC for 0x101 decoder" << std::endl;
        return;
    }

    std::uint16_t battery_mV = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t temperature_deciC = pack_u16(frame.data[2], frame.data[3]);

    double battery_V = battery_mV / 1000.0;
    double temperature_C = temperature_deciC / 10.0;

    std::cout << "Type: Battery and Temperature" << std::endl;

    std::cout << "Battery_mV: "
              << battery_mV
              << std::endl;

    std::cout << "Battery: "
              << std::fixed
              << std::setprecision(2)
              << battery_V
              << " V"
              << std::defaultfloat
              << std::setprecision(6)
              << std::endl;

    std::cout << "Temperature_deciC: "
              << temperature_deciC
              << std::endl;

    std::cout << "Temperature: "
              << std::fixed
              << std::setprecision(1)
              << temperature_C
              << " C"
              << std::defaultfloat
              << std::setprecision(6)
              << std::endl;
}