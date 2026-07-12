#pragma once

#include <cstddef>
#include "can_frame.hpp"

class TelemetryDecoder {
public:
    TelemetryDecoder();

    void decode(const CanFrame& frame);

    std::size_t frames_seen() const;

private:
    void decode_0x100(const CanFrame& frame);
    void decode_0x101(const CanFrame& frame);

    std::size_t frames_seen_;
};