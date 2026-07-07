#pragma once

#include <cstdint>
#include "can_frame.hpp"

inline bool is_known_id(std::uint32_t id) {
    return id == 0x100 ||
           id == 0x101 ||
           id == 0x102 ||
           id == 0x200;
}

inline bool has_valid_dlc(const CanFrame& frame) {
    return frame.dlc == 8;
}