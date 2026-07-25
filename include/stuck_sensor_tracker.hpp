#pragma once

#include <cstdint>
#include <vector>

#include "fault_report.hpp"

class StuckSensorTracker {
public:
    StuckSensorTracker();

    std::vector<FaultReport> check_ain1(std::uint16_t ain1_raw,
                                        bool system_active);

    void reset();

private:
    bool has_previous_value_;
    std::uint16_t previous_value_;
    std::uint32_t repeat_count_;
};