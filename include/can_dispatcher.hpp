#pragma once

#include <vector>

#include "can_frame.hpp"
#include "fault_report.hpp"
#include "telemetry_decoder.hpp"

class CanDispatcher {
public:
    explicit CanDispatcher(TelemetryDecoder& decoder);

    std::vector<FaultReport> dispatch(const CanFrame& frame);

private:
    TelemetryDecoder& decoder_;
};