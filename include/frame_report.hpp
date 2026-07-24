#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

struct FrameReport {
    std::size_t frame_number;
    std::uint32_t can_id;
    std::string frame_name;
    bool ok;
    std::vector<std::string> fault_messages;
};