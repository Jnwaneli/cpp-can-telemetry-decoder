#pragma once

#include "telemetry_data.hpp"

class FaultAnalyzer {
public:
    void check_analog_faults(const AnalogData& data);

    void check_battery_temp_faults(double voltage, double temperature);
};