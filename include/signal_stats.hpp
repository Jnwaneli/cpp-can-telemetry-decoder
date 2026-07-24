#pragma once

#include <cstdint>

struct SignalStats {
    std::uint16_t min_value;
    std::uint16_t max_value;
    double average;
    std::uint32_t count;

    SignalStats();

    void update(std::uint16_t new_value);
    void reset();
    bool has_samples() const;
};