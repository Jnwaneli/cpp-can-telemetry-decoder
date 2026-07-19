#pragma once

#include <cstddef>

#include "can_frame.hpp"
#include "fault_analyzer.hpp"

class TelemetryDecoder {
public:
    TelemetryDecoder();

    std::size_t decode(const CanFrame& frame);

    std::size_t frames_seen() const;

private:
    std::size_t decode_0x100(const CanFrame& frame);
    std::size_t decode_0x101(const CanFrame& frame);
    std::size_t decode_0x102(const CanFrame& frame);

    FaultAnalyzer fault_analyzer_;
    std::size_t frames_seen_;
};