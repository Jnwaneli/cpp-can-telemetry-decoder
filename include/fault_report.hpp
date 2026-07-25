#pragma once

#include <string>

enum class FaultSeverity {
    Fault,
    Warning
};

enum class FaultCategory {
    InvalidDlc,
    UnknownId,
    Voltage,
    Temperature,
    SensorInvalid,
    DroppedFrame,
    StuckSensor,
    Other
};

struct FaultReport {
    bool has_fault;
    std::string message;
    FaultSeverity severity = FaultSeverity::Fault;
    FaultCategory category = FaultCategory::Other;
};