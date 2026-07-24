# Hardware and Log Status

This document records the current hardware and log-input status for the C++ CAN Telemetry Decoder and Fault Analyzer.

---

## Implemented

```text
CANDispatcher routes valid CAN frames by ID.
CSV/log parser reads saved CAN log lines.
CircularBuffer stores parsed CAN frames before decoding.
TelemetryDecoder decodes supported CAN IDs.
FaultAnalyzer returns structured FaultReport objects.
0x100 Analog Inputs decoding is implemented.
0x101 Battery and Temperature decoding is implemented.
0x102 Status Flags decoding is implemented.
0x200 Vehicle Telemetry decoding is implemented.
Per-frame decoded report is implemented.
Summary statistics are implemented.