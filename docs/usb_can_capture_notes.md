# USB-CAN Capture Notes

This document records the USB-CAN capture and live-ingestion status for the STM32 CAN telemetry project.

---

## Current Status

```text
Hardware CAN transmit path: working
Waveshare USB-CAN receive software: working
Desktop C++ live serial backend: working on Windows
CSV capture path: available
Structured JSON output: working
Diagnostic report generation: working after JSON generation
```

The STM32 NUCLEO successfully transmits CAN traffic through the SN65HVD230 CAN transceiver to the Waveshare USB-CAN adapter.

The PC can observe the traffic using either:

```text
Waveshare USB-CAN receive software
Desktop C++ live reader through WaveshareSerialFrameSource
```

---

## Hardware Chain

```text
NUCLEO-G431RB FDCAN TX/RX
        ↓
SN65HVD230 TXD/RXD
        ↓
SN65HVD230 CANH/CANL
        ↓
CANH/CANL bus
        ↓
Waveshare USB-CAN
        ↓
PC
```

---

## USB-CAN Receive Settings

Working receive setup:

```text
Adapter: Waveshare USB-CAN
Mode: Normal
Frame type: Standard CAN
CAN format: Classic CAN
Bitrate: 500 kbps
Receive: enabled
```

---

## Confirmed CAN IDs

The STM32 FreeRTOS sender transmits all four supported CAN IDs:

```text
0x100 Analog Inputs
0x101 Battery and Temperature
0x102 Status Flags
0x200 Vehicle Telemetry
```

All frames use:

```text
DLC: 8 bytes
Classic CAN
Standard 11-bit ID
```

---

## Desktop Live Reader Command

Close the Waveshare receive software before running the C++ live serial backend because only one program can own the COM port at a time.

```powershell
.\main.exe --waveshare-serial COM4 2000
```

Replace `COM4` with the actual COM port assigned to the adapter.

---

## Live Decoder Result

The desktop decoder successfully processed 2,000 live STM32-generated CAN frames through the Waveshare serial backend.

```text
Frames processed: 2000
Valid frames: 2000
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

Because the firmware transmits four frame types per transmit cycle, the run represents 500 telemetry cycles.

```text
2000 total frames / 4 frame types = 500 telemetry cycles
```

---

## CSV Capture Path

A short captured FreeRTOS log is also available:

```text
data/freertos_captured_log.csv
```

This file can be processed with:

```powershell
.\main.exe --csv data\freertos_captured_log.csv
```

The CSV path is useful for repeatable tests when the hardware is not connected.

---

## Generated Outputs

After a decoder run, the C++ application writes:

```text
output/fault_summary.json
```

The diagnostic report agent reads that JSON file and writes:

```text
output/diagnostic_report.md
```

---

## Debug Checklist Used

The hardware CAN bridge worked after checking the core CAN requirements:

```text
matching bitrate
CANH connected to CANH
CANL connected to CANL
common ground
3.3 V transceiver power
termination across CANH/CANL
Waveshare receive mode
```
