#pragma once

#include <cstddef>
#include <vector>

#include "fault_report.hpp"
#include "telemetry_data.hpp"

class FaultAnalyzer {
public:
    FaultAnalyzer();

    std::vector<FaultReport> check_analog_faults(const AnalogData& data);
    std::vector<FaultReport> check_battery_temp_faults(double voltage, double temperature);

    void record_report(const FaultReport& report);
    void reset_summary();
    void print_summary() const;

private:
    std::size_t invalid_dlc_count_;
    std::size_t unknown_id_count_;
    std::size_t voltage_fault_count_;
    std::size_t temperature_fault_count_;
    std::size_t sensor_invalid_count_;
    std::size_t dropped_frame_count_;
    std::size_t stuck_sensor_warning_count_;
    std::size_t other_fault_count_;
};