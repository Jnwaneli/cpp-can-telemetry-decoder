#include "bit_utils.hpp"

std::uint16_t pack_u16(std::uint8_t low, std::uint8_t high) {
    return static_cast<std::uint16_t>(low) |
           (static_cast<std::uint16_t>(high) << 8);
}

bool get_bit(std::uint8_t value, int bit_position) {
    if (bit_position < 0 || bit_position > 7) {
        return false;
    }

    std::uint8_t mask = static_cast<std::uint8_t>(1u << bit_position);

    return (value & mask) != 0;
}

bool is_mask_set(std::uint8_t value, std::uint8_t mask) {
    return (value & mask) == mask;
}