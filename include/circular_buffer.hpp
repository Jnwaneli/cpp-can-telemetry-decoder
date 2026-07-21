#pragma once

#include <array>
#include <cstddef>
#include "can_frame.hpp"

class CircularBuffer {
public:
    static constexpr std::size_t Capacity = 16;

    CircularBuffer();

    bool push(const CanFrame& frame);
    bool pop(CanFrame& frame);

    bool is_empty() const;
    bool is_full() const;

    std::size_t size() const;

private:
    std::array<CanFrame, Capacity> buffer_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t count_;
};