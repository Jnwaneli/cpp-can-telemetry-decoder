#include "circular_buffer.hpp"

CircularBuffer::CircularBuffer()
    : buffer_{},
      head_(0),
      tail_(0),
      count_(0) {
}

bool CircularBuffer::push(const CanFrame& frame) {
    if (is_full()) {
        return false;
    }

    buffer_[head_] = frame;
    head_ = (head_ + 1) % Capacity;
    count_++;

    return true;
}

bool CircularBuffer::pop(CanFrame& frame) {
    if (is_empty()) {
        return false;
    }

    frame = buffer_[tail_];
    tail_ = (tail_ + 1) % Capacity;
    count_--;

    return true;
}

bool CircularBuffer::is_empty() const {
    return count_ == 0;
}

bool CircularBuffer::is_full() const {
    return count_ == Capacity;
}

std::size_t CircularBuffer::size() const {
    return count_;
}