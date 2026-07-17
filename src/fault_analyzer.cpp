#include "fault_analyzer.hpp"

#include <iostream>

void FaultAnalyzer::check_analog_faults(const AnalogData& data) {
    bool has_fault = false;

    if (data.ain1_raw > 4095) {
        std::cout << "FAULT: AIN1 raw value out of 12-bit ADC range" << std::endl;
        has_fault = true;
    }

    if (data.ain2_raw > 4095) {
        std::cout << "FAULT: AIN2 raw value out of 12-bit ADC range" << std::endl;
        has_fault = true;
    }

    if (data.ain3_raw > 4095) {
        std::cout << "FAULT: AIN3 raw value out of 12-bit ADC range" << std::endl;
        has_fault = true;
    }

    if (!data.sensor1_valid) {
        std::cout << "FAULT: Sensor 1 invalid" << std::endl;
        has_fault = true;
    }

    if (!data.sensor2_valid) {
        std::cout << "FAULT: Sensor 2 invalid" << std::endl;
        has_fault = true;
    }

    if (!data.sensor3_valid) {
        std::cout << "FAULT: Sensor 3 invalid" << std::endl;
        has_fault = true;
    }

    if (data.error_flag_set) {
        std::cout << "FAULT: Error flag set in analog status byte" << std::endl;
        has_fault = true;
    }

    if (!has_fault) {
        std::cout << "Fault Check: Analog OK" << std::endl;
    }
}

void FaultAnalyzer::check_battery_temp_faults(double voltage, double temperature) {
    bool has_fault = false;

    if (voltage < 10.5) {
        std::cout << "FAULT: Battery voltage too low" << std::endl;
        has_fault = true;
    }

    if (voltage > 14.8) {
        std::cout << "FAULT: Battery voltage too high" << std::endl;
        has_fault = true;
    }

    if (temperature > 80.0) {
        std::cout << "FAULT: Temperature too high" << std::endl;
        has_fault = true;
    }

    if (!has_fault) {
        std::cout << "Fault Check: Battery/Temperature OK" << std::endl;
    }
}