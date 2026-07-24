#include "signal_stats.hpp"

SignalStats::SignalStats()
    : min_value(0),
      max_value(0),
      average(0.0),
      count(0) {
}

void SignalStats::update(std::uint16_t new_value) {
    if (count == 0) {
        min_value = new_value;
        max_value = new_value;
        average = static_cast<double>(new_value);
        count = 1;
        return;
    }

    if (new_value < min_value) {
        min_value = new_value;
    }

    if (new_value > max_value) {
        max_value = new_value;
    }

    count++;

    average += (static_cast<double>(new_value) - average) /
               static_cast<double>(count);
}

void SignalStats::reset() {
    min_value = 0;
    max_value = 0;
    average = 0.0;
    count = 0;
}

bool SignalStats::has_samples() const {
    return count > 0;
}