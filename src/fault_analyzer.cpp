#include "fault_analyzer.hpp"

#include <iostream>

FaultAnalyzer::FaultAnalyzer()
    : invalid_dlc_count_(0),
      unknown_id_count_(0),
      voltage_fault_count_(0),
      temperature_fault_count_(0),
      sensor_invalid_count_(0),
      dropped_frame_count_(0),
      stuck_sensor_warning_count_(0),
      other_fault_count_(0) {
}

std::vector<FaultReport> FaultAnalyzer::check_analog_faults(const AnalogData& data) {
    std::vector<FaultReport> reports;

    if (data.ain1_raw > 4095) {
        reports.push_back({
            true,
            "AIN1 raw value out of 12-bit ADC range",
            FaultSeverity::Fault,
            FaultCategory::Other
        });
    }

    if (data.ain2_raw > 4095) {
        reports.push_back({
            true,
            "AIN2 raw value out of 12-bit ADC range",
            FaultSeverity::Fault,
            FaultCategory::Other
        });
    }

    if (data.ain3_raw > 4095) {
        reports.push_back({
            true,
            "AIN3 raw value out of 12-bit ADC range",
            FaultSeverity::Fault,
            FaultCategory::Other
        });
    }

    if (!data.sensor1_valid) {
        reports.push_back({
            true,
            "Sensor 1 invalid",
            FaultSeverity::Fault,
            FaultCategory::SensorInvalid
        });
    }

    if (!data.sensor2_valid) {
        reports.push_back({
            true,
            "Sensor 2 invalid",
            FaultSeverity::Fault,
            FaultCategory::SensorInvalid
        });
    }

    if (!data.sensor3_valid) {
        reports.push_back({
            true,
            "Sensor 3 invalid",
            FaultSeverity::Fault,
            FaultCategory::SensorInvalid
        });
    }

    if (data.error_flag_set) {
        reports.push_back({
            true,
            "Error flag set in analog status byte",
            FaultSeverity::Fault,
            FaultCategory::Other
        });
    }

    return reports;
}

std::vector<FaultReport> FaultAnalyzer::check_battery_temp_faults(double voltage, double temperature) {
    std::vector<FaultReport> reports;

    if (voltage < 10.5) {
        reports.push_back({
            true,
            "Battery voltage too low",
            FaultSeverity::Fault,
            FaultCategory::Voltage
        });
    }

    if (voltage > 14.8) {
        reports.push_back({
            true,
            "Battery voltage too high",
            FaultSeverity::Fault,
            FaultCategory::Voltage
        });
    }

    if (temperature > 80.0) {
        reports.push_back({
            true,
            "Temperature too high",
            FaultSeverity::Fault,
            FaultCategory::Temperature
        });
    }

    return reports;
}

void FaultAnalyzer::record_report(const FaultReport& report) {
    if (report.severity == FaultSeverity::Warning &&
        report.category == FaultCategory::StuckSensor) {
        stuck_sensor_warning_count_++;
        return;
    }

    if (!report.has_fault) {
        return;
    }

    switch (report.category) {
        case FaultCategory::InvalidDlc:
            invalid_dlc_count_++;
            break;

        case FaultCategory::UnknownId:
            unknown_id_count_++;
            break;

        case FaultCategory::Voltage:
            voltage_fault_count_++;
            break;

        case FaultCategory::Temperature:
            temperature_fault_count_++;
            break;

        case FaultCategory::SensorInvalid:
            sensor_invalid_count_++;
            break;

        case FaultCategory::DroppedFrame:
            dropped_frame_count_++;
            break;

        case FaultCategory::StuckSensor:
            stuck_sensor_warning_count_++;
            break;

        case FaultCategory::Other:
        default:
            other_fault_count_++;
            break;
    }
}

void FaultAnalyzer::reset_summary() {
    invalid_dlc_count_ = 0;
    unknown_id_count_ = 0;
    voltage_fault_count_ = 0;
    temperature_fault_count_ = 0;
    sensor_invalid_count_ = 0;
    dropped_frame_count_ = 0;
    stuck_sensor_warning_count_ = 0;
    other_fault_count_ = 0;
}

void FaultAnalyzer::print_summary() const {
    std::cout << "Fault Summary:" << std::endl;

    std::cout << "Invalid DLC: "
              << invalid_dlc_count_
              << std::endl;

    std::cout << "Unknown IDs: "
              << unknown_id_count_
              << std::endl;

    std::cout << "Voltage faults: "
              << voltage_fault_count_
              << std::endl;

    std::cout << "Temperature faults: "
              << temperature_fault_count_
              << std::endl;

    std::cout << "Sensor invalid faults: "
              << sensor_invalid_count_
              << std::endl;

    std::cout << "Dropped frames: "
              << dropped_frame_count_
              << std::endl;

    std::cout << "Possible stuck sensors: "
              << stuck_sensor_warning_count_
              << std::endl;

    std::cout << "Other faults: "
              << other_fault_count_
              << std::endl;
}