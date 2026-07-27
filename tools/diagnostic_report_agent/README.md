# Diagnostic Report Agent

## Overview

This tool reads the structured C++ decoder summary from:

```text
output/fault_summary.json
```

and generates a human-readable diagnostic report at:

```text
output/diagnostic_report.md
```

The C++ decoder performs deterministic parsing and fault detection. The diagnostic report agent only explains the structured fault summary.

---

## Purpose

The diagnostic report agent is the explanation layer for the C++ CAN Telemetry Decoder and Fault Analyzer project.

It converts machine-readable fault data into a readable engineering report with:

```text
Overview
Detected Faults
Likely Causes
Recommended Checks
Severity Notes
Next Steps
```

---

## Input

Expected input file:

```text
output/fault_summary.json
```

Example:

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

## Output

Generated output file:

```text
output/diagnostic_report.md
```

The generated report contains:

```text
Overview
Detected Faults
Likely Causes
Recommended Checks
Severity Notes
Next Steps
```

---

## Run

From the repository root:

```powershell
python tools\diagnostic_report_agent\generate_report.py
```

If `python` is not recognized, use:

```powershell
py tools\diagnostic_report_agent\generate_report.py
```

Expected terminal output:

```text
Read fault summary: output/fault_summary.json
Wrote diagnostic report: output/diagnostic_report.md
```

Then open the report:

```powershell
type output\diagnostic_report.md
```

---

## Example Clean Report

A clean live run may generate a report like this:

```markdown
# AI-Assisted CAN Diagnostic Report

## Overview

- Source: `waveshare_live`
- Frames processed: 2000
- Valid frames: 2000
- Faults: 0
- Warnings: 0

The C++ decoder performs deterministic parsing and fault detection. The AI assistant only explains the structured fault summary.

## Detected Faults

- No faults or warnings were detected in the structured summary.

## Likely Causes

- No likely fault causes were identified because the structured summary reports a clean run.

## Recommended Checks

- Confirm that the decoder input source matches the intended test path.
- Confirm that the CAN bitrate is 500 kbps on both the STM32 sender and the USB-CAN adapter.
- Confirm that all expected CAN IDs are present: 0x100, 0x101, 0x102, and 0x200.
- No immediate corrective action is required for this run.

## Severity Notes

- Severity: clean run. No faults or warnings were reported.
- The C++ decoder performs deterministic parsing and fault detection. The diagnostic assistant only explains the structured fault summary.

## Next Steps

- Save this report as evidence of a clean live or logged decoder run.
- Use the same workflow with injected fault data to demonstrate fault-report behavior.
- Reference this report in project documentation or demos as proof of the full diagnostic pipeline.
```

---

## Project Role

The diagnostic report agent sits after the C++ decoder pipeline:

```text
STM32 FreeRTOS CAN sender
        ↓
Waveshare USB-CAN live serial input
        ↓
C++ deterministic decoder and fault analyzer
        ↓
output/fault_summary.json
        ↓
Diagnostic report agent
        ↓
output/diagnostic_report.md
```

The agent does not decide whether a frame is valid. It does not decode raw CAN bytes. It only explains the structured summary produced by the C++ decoder.

---

## Files

```text
tools/diagnostic_report_agent/
├── README.md
├── generate_report.py
├── sample_fault_summary.json
└── sample_report.md
```

---

## Notes

This tool is intentionally lightweight and deterministic.

It does not call an external AI API. Instead, it uses rule-based explanation logic to produce an AI-style diagnostic report from the structured JSON summary.

This keeps the project easy to run locally while still demonstrating how a diagnostic assistant layer can sit on top of a C++ embedded telemetry decoder.
