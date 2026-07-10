#pragma once

#include "can_frame.hpp"

class TelemetryDecoder {
public:
    void decode(const CanFrame& frame) const;

private:
    void decode_0x100(const CanFrame& frame) const;
};