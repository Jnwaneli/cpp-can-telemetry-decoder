#pragma once

#include <cstddef>
#include <vector>

#include "can_frame.hpp"
#include "counter_tracker.hpp"
#include "fault_analyzer.hpp"
#include "fault_report.hpp"
#include "signal_stats.hpp"
#include "stuck_sensor_tracker.hpp"

class TelemetryDecoder {
public:
    TelemetryDecoder();

    void record_frame_seen();

    std::vector<FaultReport> decode_0x100(const CanFrame& frame);
    std::vector<FaultReport> decode_0x101(const CanFrame& frame);
    std::vector<FaultReport> decode_0x102(const CanFrame& frame);
    std::vector<FaultReport> decode_0x200(const CanFrame& frame);

    std::size_t frames_seen() const;

    void print_signal_stats() const;

private:
    FaultAnalyzer fault_analyzer_;
    CounterTracker counter_tracker_;
    StuckSensorTracker stuck_sensor_tracker_;

    std::size_t frames_seen_;

    SignalStats ain1_stats_;
    SignalStats ain2_stats_;
    SignalStats ain3_stats_;
    SignalStats battery_mV_stats_;
    SignalStats temperature_deciC_stats_;
};