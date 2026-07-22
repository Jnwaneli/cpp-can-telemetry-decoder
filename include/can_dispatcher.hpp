#pragma once

#include <cstddef>

#include "can_frame.hpp"
#include "telemetry_decoder.hpp"

class CanDispatcher {
public:
    explicit CanDispatcher(TelemetryDecoder& decoder);

    std::size_t dispatch(const CanFrame& frame);

private:
    TelemetryDecoder& decoder_;
};