# Fault Summary JSON Output

## Purpose

This document describes the structured machine-readable diagnostic output produced by the C++ CAN decoder.

After each successful CSV or live Waveshare decoder run, the program writes:

```text
output/fault_summary.json
```

This file summarizes the same decoder and fault-analysis results that are printed in the terminal, but in JSON format so the diagnostic report agent can explain the run in a readable engineering report.

---

## Supported Run Modes

The JSON output is produced by all normal decoder paths:

```text
Default CSV run
CSV log run
Waveshare USB-CAN live serial run
Fallback built-in log run
```

Example commands:

```powershell
./main.exe
./main.exe --csv data/sample_can_log.csv
./main.exe --waveshare-serial COM4 2000
```

---

## Output Schema

Example clean live summary:

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

## Source Values

The `source` field identifies the input path used for the run.

```text
csv_default     = default data/sample_can_log.csv run
csv_log         = explicit --csv run
waveshare_live  = direct Waveshare USB-CAN serial live run
fallback_log    = built-in fallback CAN log
```

---

## Implementation Notes

The JSON writer is implemented in:

```text
include/fault_summary_writer.hpp
src/fault_summary_writer.cpp
```

The writer uses the existing `DecoderStats` and `FaultAnalyzer` counters, creates the `output/` directory when needed, and writes `output/fault_summary.json` after the terminal summary is printed.

`FaultAnalyzer` exposes summary counter getters so the JSON writer can access diagnostic totals without duplicating fault-analysis logic.

---

## Diagnostic Report Connection

The JSON summary is the bridge between the C++ decoder and the diagnostic report agent.

```text
C++ decoder run
        ↓
output/fault_summary.json
        ↓
Diagnostic report agent
        ↓
output/diagnostic_report.md
```

The C++ decoder performs deterministic parsing and fault detection. The diagnostic report agent only explains the structured fault summary.
