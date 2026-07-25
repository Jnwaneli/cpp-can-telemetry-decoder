#include "counter_tracker.hpp"

#include <sstream>
#include <string>

namespace {
std::string format_can_id(std::uint32_t can_id) {
    std::ostringstream output;

    output << "0x"
           << std::hex
           << std::uppercase
           << can_id;

    return output.str();
}
}

std::vector<FaultReport> CounterTracker::check_counter(std::uint32_t can_id,
                                                       std::uint8_t current_counter) {
    std::vector<FaultReport> reports;

    if (!has_previous_counter_[can_id]) {
        previous_counter_[can_id] = current_counter;
        has_previous_counter_[can_id] = true;
        return reports;
    }

    std::uint8_t previous = previous_counter_[can_id];

    std::uint8_t expected = static_cast<std::uint8_t>(previous + 1);

    if (current_counter != expected) {
        std::ostringstream message;

        message << "Dropped frame counter detected for ID "
                << format_can_id(can_id)
                << ": expected "
                << static_cast<int>(expected)
                << ", got "
                << static_cast<int>(current_counter);

        reports.push_back({
            true,
            message.str(),
            FaultSeverity::Fault,
            FaultCategory::DroppedFrame
        });
    }

    previous_counter_[can_id] = current_counter;

    return reports;
}

void CounterTracker::reset() {
    previous_counter_.clear();
    has_previous_counter_.clear();
}