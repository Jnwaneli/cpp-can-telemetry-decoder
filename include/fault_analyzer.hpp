#pragma once

#include <cstddef>

#include "telemetry_data.hpp"

class FaultAnalyzer {
public:
    std::size_t check_analog_faults(const AnalogData& data);

    std::size_t check_battery_temp_faults(double voltage, double temperature);
};