#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "fault_report.hpp"

class CounterTracker {
public:
    std::vector<FaultReport> check_counter(std::uint32_t can_id,
                                           std::uint8_t current_counter);

    void reset();

private:
    std::unordered_map<std::uint32_t, std::uint8_t> previous_counter_;
    std::unordered_map<std::uint32_t, bool> has_previous_counter_;
};