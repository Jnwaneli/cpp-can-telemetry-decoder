#include "fault_analyzer.hpp"

std::vector<FaultReport> FaultAnalyzer::check_analog_faults(const AnalogData& data) {
    std::vector<FaultReport> reports;

    if (data.ain1_raw > 4095) {
        reports.push_back({true, "AIN1 raw value out of 12-bit ADC range"});
    }

    if (data.ain2_raw > 4095) {
        reports.push_back({true, "AIN2 raw value out of 12-bit ADC range"});
    }

    if (data.ain3_raw > 4095) {
        reports.push_back({true, "AIN3 raw value out of 12-bit ADC range"});
    }

    if (!data.sensor1_valid) {
        reports.push_back({true, "Sensor 1 invalid"});
    }

    if (!data.sensor2_valid) {
        reports.push_back({true, "Sensor 2 invalid"});
    }

    if (!data.sensor3_valid) {
        reports.push_back({true, "Sensor 3 invalid"});
    }

    if (data.error_flag_set) {
        reports.push_back({true, "Error flag set in analog status byte"});
    }

    return reports;
}

std::vector<FaultReport> FaultAnalyzer::check_battery_temp_faults(double voltage, double temperature) {
    std::vector<FaultReport> reports;

    if (voltage < 10.5) {
        reports.push_back({true, "Battery voltage too low"});
    }

    if (voltage > 14.8) {
        reports.push_back({true, "Battery voltage too high"});
    }

    if (temperature > 80.0) {
        reports.push_back({true, "Temperature too high"});
    }

    return reports;
}