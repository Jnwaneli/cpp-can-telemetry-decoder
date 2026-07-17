#pragma once

#include <cstdint>

struct AnalogData {
    std::uint16_t ain1_raw;
    std::uint16_t ain2_raw;
    std::uint16_t ain3_raw;

    std::uint8_t status;
    std::uint8_t counter;

    bool sensor1_valid;
    bool sensor2_valid;
    bool sensor3_valid;
    bool error_flag_set;
};