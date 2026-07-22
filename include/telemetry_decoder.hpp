#pragma once

#include <cstddef>

#include "can_frame.hpp"
#include "fault_analyzer.hpp"

class TelemetryDecoder {
public:
    TelemetryDecoder();

    void record_frame_seen();

    std::size_t decode_0x100(const CanFrame& frame);
    std::size_t decode_0x101(const CanFrame& frame);
    std::size_t decode_0x102(const CanFrame& frame);
    std::size_t decode_0x200(const CanFrame& frame);

    std::size_t frames_seen() const;

private:
    FaultAnalyzer fault_analyzer_;
    std::size_t frames_seen_;
};