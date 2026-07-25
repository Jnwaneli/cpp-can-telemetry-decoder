#include "stuck_sensor_tracker.hpp"

#include <sstream>

namespace {
constexpr std::uint32_t STUCK_SENSOR_FRAME_THRESHOLD = 20;
}

StuckSensorTracker::StuckSensorTracker()
    : has_previous_value_(false),
      previous_value_(0),
      repeat_count_(0) {
}

std::vector<FaultReport> StuckSensorTracker::check_ain1(std::uint16_t ain1_raw,
                                                        bool system_active) {
    std::vector<FaultReport> reports;

    if (!system_active) {
        reset();
        return reports;
    }

    if (!has_previous_value_) {
        previous_value_ = ain1_raw;
        repeat_count_ = 1;
        has_previous_value_ = true;
        return reports;
    }

    if (ain1_raw == previous_value_) {
        repeat_count_++;
    } else {
        previous_value_ = ain1_raw;
        repeat_count_ = 1;
        return reports;
    }

    if (repeat_count_ == STUCK_SENSOR_FRAME_THRESHOLD) {
        std::ostringstream message;

        message << "Possible stuck sensor warning: AIN1 stayed at "
                << ain1_raw
                << " for "
                << repeat_count_
                << " active frames";

        reports.push_back({
            false,
            message.str(),
            FaultSeverity::Warning,
            FaultCategory::StuckSensor
        });
    }

    return reports;
}

void StuckSensorTracker::reset() {
    has_previous_value_ = false;
    previous_value_ = 0;
    repeat_count_ = 0;
}