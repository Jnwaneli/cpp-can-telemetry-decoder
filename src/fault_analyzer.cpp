#include "fault_analyzer.hpp"

#include <iostream>

std::size_t FaultAnalyzer::check_analog_faults(const AnalogData& data) {
    std::size_t fault_count = 0;

    if (data.ain1_raw > 4095) {
        std::cout << "FAULT: AIN1 raw value out of 12-bit ADC range" << std::endl;
        fault_count++;
    }

    if (data.ain2_raw > 4095) {
        std::cout << "FAULT: AIN2 raw value out of 12-bit ADC range" << std::endl;
        fault_count++;
    }

    if (data.ain3_raw > 4095) {
        std::cout << "FAULT: AIN3 raw value out of 12-bit ADC range" << std::endl;
        fault_count++;
    }

    if (!data.sensor1_valid) {
        std::cout << "FAULT: Sensor 1 invalid" << std::endl;
        fault_count++;
    }

    if (!data.sensor2_valid) {
        std::cout << "FAULT: Sensor 2 invalid" << std::endl;
        fault_count++;
    }

    if (!data.sensor3_valid) {
        std::cout << "FAULT: Sensor 3 invalid" << std::endl;
        fault_count++;
    }

    if (data.error_flag_set) {
        std::cout << "FAULT: Error flag set in analog status byte" << std::endl;
        fault_count++;
    }

    if (fault_count == 0) {
        std::cout << "Fault Check: Analog OK" << std::endl;
    }

    return fault_count;
}

std::size_t FaultAnalyzer::check_battery_temp_faults(double voltage, double temperature) {
    std::size_t fault_count = 0;

    if (voltage < 10.5) {
        std::cout << "FAULT: Battery voltage too low" << std::endl;
        fault_count++;
    }

    if (voltage > 14.8) {
        std::cout << "FAULT: Battery voltage too high" << std::endl;
        fault_count++;
    }

    if (temperature > 80.0) {
        std::cout << "FAULT: Temperature too high" << std::endl;
        fault_count++;
    }

    if (fault_count == 0) {
        std::cout << "Fault Check: Battery/Temperature OK" << std::endl;
    }

    return fault_count;
}