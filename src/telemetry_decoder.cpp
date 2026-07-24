#include "telemetry_decoder.hpp"
#include "bit_utils.hpp"
#include "telemetry_data.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

namespace {
constexpr std::uint8_t SENSOR1_VALID_MASK = 0x01;
constexpr std::uint8_t SENSOR2_VALID_MASK = 0x02;
constexpr std::uint8_t SENSOR3_VALID_MASK = 0x04;
constexpr std::uint8_t ERROR_FLAG_MASK = 0x80;
}

TelemetryDecoder::TelemetryDecoder()
    : fault_analyzer_(),
      frames_seen_(0) {
}

void TelemetryDecoder::record_frame_seen() {
    frames_seen_++;
}

std::size_t TelemetryDecoder::frames_seen() const {
    return frames_seen_;
}

std::vector<FaultReport> TelemetryDecoder::decode_0x100(const CanFrame& frame) {
    if (frame.dlc < 8) {
        std::cout << "Type: Analog Inputs" << std::endl;
        return {{true, "Invalid DLC for 0x100 decoder"}};
    }

    std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
    std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);

    std::uint8_t status = frame.data[6];
    std::uint8_t counter = frame.data[7];

    bool sensor1_valid = is_mask_set(status, SENSOR1_VALID_MASK);
    bool sensor2_valid = is_mask_set(status, SENSOR2_VALID_MASK);
    bool sensor3_valid = is_mask_set(status, SENSOR3_VALID_MASK);
    bool error_flag_set = is_mask_set(status, ERROR_FLAG_MASK);

    AnalogData data{
        ain1,
        ain2,
        ain3,
        status,
        counter,
        sensor1_valid,
        sensor2_valid,
        sensor3_valid,
        error_flag_set
    };

    std::cout << "Type: Analog Inputs" << std::endl;

    std::cout << "AIN1_RAW: "
              << data.ain1_raw
              << std::endl;

    std::cout << "AIN2_RAW: "
              << data.ain2_raw
              << std::endl;

    std::cout << "AIN3_RAW: "
              << data.ain3_raw
              << std::endl;

    std::cout << "Status: 0x"
              << std::hex
              << std::setw(2)
              << std::setfill('0')
              << static_cast<int>(data.status)
              << std::dec
              << std::setfill(' ')
              << std::endl;

    std::cout << "Sensor1_VALID: "
              << (data.sensor1_valid ? "yes" : "no")
              << std::endl;

    std::cout << "Sensor2_VALID: "
              << (data.sensor2_valid ? "yes" : "no")
              << std::endl;

    std::cout << "Sensor3_VALID: "
              << (data.sensor3_valid ? "yes" : "no")
              << std::endl;

    std::cout << "Error_Flag: "
              << (data.error_flag_set ? "yes" : "no")
              << std::endl;

    std::cout << "Counter: "
              << static_cast<int>(data.counter)
              << std::endl;

    return fault_analyzer_.check_analog_faults(data);
}

std::vector<FaultReport> TelemetryDecoder::decode_0x101(const CanFrame& frame) {
    if (frame.dlc < 8) {
        std::cout << "Type: Battery and Temperature" << std::endl;
        return {{true, "Invalid DLC for 0x101 decoder"}};
    }

    std::uint16_t battery_mV = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t temperature_deciC = pack_u16(frame.data[2], frame.data[3]);

    double battery_V = battery_mV / 1000.0;
    double temperature_C = temperature_deciC / 10.0;

    std::cout << "Type: Battery and Temperature" << std::endl;

    std::cout << "Battery_mV: "
              << battery_mV
              << std::endl;

    std::cout << "Battery: "
              << std::fixed
              << std::setprecision(2)
              << battery_V
              << " V"
              << std::defaultfloat
              << std::setprecision(6)
              << std::endl;

    std::cout << "Temperature_deciC: "
              << temperature_deciC
              << std::endl;

    std::cout << "Temperature: "
              << std::fixed
              << std::setprecision(1)
              << temperature_C
              << " C"
              << std::defaultfloat
              << std::setprecision(6)
              << std::endl;

    return fault_analyzer_.check_battery_temp_faults(battery_V, temperature_C);
}

std::vector<FaultReport> TelemetryDecoder::decode_0x102(const CanFrame& frame) {
    if (frame.dlc < 8) {
        std::cout << "Type: Status Flags" << std::endl;
        return {{true, "Invalid DLC for 0x102 decoder"}};
    }

    std::uint8_t sensor_valid_flags = frame.data[0];
    std::uint8_t system_fault_flags = frame.data[1];
    std::uint8_t mode = frame.data[2];
    std::uint8_t error_code = frame.data[3];

    bool sensor1_valid = is_mask_set(sensor_valid_flags, SENSOR1_VALID_MASK);
    bool sensor2_valid = is_mask_set(sensor_valid_flags, SENSOR2_VALID_MASK);
    bool sensor3_valid = is_mask_set(sensor_valid_flags, SENSOR3_VALID_MASK);

    std::cout << "Type: Status Flags" << std::endl;

    std::cout << "Status Frame 0x102:" << std::endl;

    std::cout << "Sensor1 Valid: "
              << (sensor1_valid ? "YES" : "NO")
              << std::endl;

    std::cout << "Sensor2 Valid: "
              << (sensor2_valid ? "YES" : "NO")
              << std::endl;

    std::cout << "Sensor3 Valid: "
              << (sensor3_valid ? "YES" : "NO")
              << std::endl;

    std::cout << "System Fault Byte: 0x"
              << std::hex
              << std::setw(2)
              << std::setfill('0')
              << static_cast<int>(system_fault_flags)
              << std::dec
              << std::setfill(' ')
              << std::endl;

    std::cout << "Mode: "
              << static_cast<int>(mode)
              << std::endl;

    std::cout << "Error Code: "
              << static_cast<int>(error_code)
              << std::endl;

    return {};
}

std::vector<FaultReport> TelemetryDecoder::decode_0x200(const CanFrame& frame) {
    if (frame.dlc < 8) {
        std::cout << "Type: Vehicle Telemetry" << std::endl;
        return {{true, "Invalid DLC for 0x200 decoder"}};
    }

    VehicleData data{
        pack_u16(frame.data[0], frame.data[1]),
        pack_u16(frame.data[2], frame.data[3]),
        frame.data[4],
        frame.data[5],
        frame.data[6],
        frame.data[7]
    };

    std::cout << "Type: Vehicle Telemetry" << std::endl;

    std::cout << "Speed_RAW: "
              << data.speed_raw
              << std::endl;

    std::cout << "RPM: "
              << data.rpm
              << std::endl;

    std::cout << "Gear: "
              << static_cast<int>(data.gear)
              << std::endl;

    std::cout << "Throttle_Percent: "
              << static_cast<int>(data.throttle_percent)
              << "%"
              << std::endl;

    std::cout << "Brake_Percent: "
              << static_cast<int>(data.brake_percent)
              << "%"
              << std::endl;

    std::cout << "Counter: "
              << static_cast<int>(data.counter)
              << std::endl;

    std::cout << "Speed scaling: not implemented yet" << std::endl;

    return {};
}