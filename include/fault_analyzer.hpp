#pragma once

#include <vector>

#include "fault_report.hpp"
#include "telemetry_data.hpp"

class FaultAnalyzer {
public:
    std::vector<FaultReport> check_analog_faults(const AnalogData& data);

    std::vector<FaultReport> check_battery_temp_faults(double voltage, double temperature);
};