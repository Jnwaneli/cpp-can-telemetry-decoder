# AI-Assisted CAN Diagnostic Report

## Overview

- Source: `waveshare_live`
- Frames processed: 200
- Valid frames: 200
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
- Continue to the final Week 9 polish step: connect this report generator to the README/demo documentation.
