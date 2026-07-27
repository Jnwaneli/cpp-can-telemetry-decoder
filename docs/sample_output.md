# Sample Output

This document shows expected output examples for the C++ CAN Telemetry Decoder and Fault Analyzer.

The decoder supports both CSV/log input and direct live Waveshare USB-CAN serial input on Windows.

---

## Build Command

From the repository root:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/csv_frame_source.cpp src/waveshare_serial_frame_source.cpp src/fault_summary_writer.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

---

## Default CSV Run

Run:

```powershell
.\main.exe
```

The default run reads:

```text
data/sample_can_log.csv
```

Expected summary:

```text
Frames processed: 10
Valid frames: 8
Unknown IDs: 1
Invalid DLC: 1
Faults: 3
Warnings: 0
Dropped frames: 1
```

After the run, the decoder writes:

```text
output/fault_summary.json
```

---

## Explicit CSV Run

Run:

```powershell
.\main.exe --csv data\sample_can_log.csv
```

Useful CSV test files:

```text
data/good_frames.csv
data/fault_frames.csv
data/stuck_sensor_test.csv
data/freertos_captured_log.csv
```

---

## Live Waveshare Run

Run:

```powershell
.\main.exe --waveshare-serial COM4 2000
```

Replace `COM4` with the actual COM port assigned to the Waveshare USB-CAN adapter.

Expected clean live summary:

```text
Frames processed: 2000
Valid frames: 2000
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

Expected JSON summary for a clean live run:

```json
{
  "source": "waveshare_live",
  "frames_processed": 2000,
  "valid_frames": 2000,
  "invalid_dlc": 0,
  "unknown_ids": 0,
  "faults": 0,
  "warnings": 0,
  "low_voltage_faults": 0,
  "high_voltage_faults": 0,
  "voltage_faults": 0,
  "high_temperature_faults": 0,
  "sensor_invalid_faults": 0,
  "dropped_frames": 0,
  "possible_stuck_sensors": 0,
  "other_faults": 0
}
```

---

## Diagnostic Report Generation

After generating `output/fault_summary.json`, run:

```powershell
python tools\diagnostic_report_agent\generate_report.py
```

If `python` is not recognized, run:

```powershell
py tools\diagnostic_report_agent\generate_report.py
```

Expected terminal output:

```text
Read fault summary: output/fault_summary.json
Wrote diagnostic report: output/diagnostic_report.md
```

Open the generated report:

```powershell
type output\diagnostic_report.md
```

Expected clean-report sections:

```text
Overview
Detected Faults
Likely Causes
Recommended Checks
Severity Notes
Next Steps
```

---

## Clean Live Report Summary

A clean live diagnostic report should state:

```text
No faults or warnings were detected in the structured summary.
Severity: clean run. No faults or warnings were reported.
```

The C++ decoder performs deterministic parsing and fault detection. The diagnostic report agent only explains the structured fault summary.
