#pragma once

#include <cstdint>

std::uint16_t pack_u16(std::uint8_t low, std::uint8_t high);

bool get_bit(std::uint8_t value, int bit_position);

bool is_mask_set(std::uint8_t value, std::uint8_t mask);