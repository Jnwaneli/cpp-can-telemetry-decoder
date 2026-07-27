# Real-Time C++ CAN Ingestion

This document explains how the desktop C++ decoder receives live CAN traffic through the Waveshare USB-CAN serial workflow.

---

## Purpose

The live ingestion path lets the same C++ decoder process hardware-generated CAN frames without manually converting them to CSV first.

The decoder uses a common `FrameSource` interface so CSV input and live hardware input feed the same downstream pipeline.

---

## Implemented Input Backends

```text
CsvFrameSource
WaveshareSerialFrameSource
```

`CsvFrameSource` reads CAN frames from CSV files.

`WaveshareSerialFrameSource` opens the Waveshare USB-CAN adapter as a Windows COM port, parses the adapter's serial CAN packet format, converts each packet into a `CanFrame`, and feeds it into the decoder.

---

## Live Hardware Path

```text
STM32 FreeRTOS telemetry sender
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
Windows COM port
        ↓
WaveshareSerialFrameSource
        ↓
CanFrame
        ↓
run_decoder_live()
        ↓
process_frame()
        ↓
CanDispatcher
        ↓
TelemetryDecoder
        ↓
FaultAnalyzer
        ↓
output/fault_summary.json
        ↓
Diagnostic report agent
        ↓
output/diagnostic_report.md
```

---

## Build Command

From the repository root:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/csv_frame_source.cpp src/waveshare_serial_frame_source.cpp src/fault_summary_writer.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

---

## Run Command

Close the Waveshare receive software before running the C++ live reader because the C++ application needs to own the COM port.

```powershell
.\main.exe --waveshare-serial COM4 2000
```

Replace `COM4` with the adapter's actual COM port.

---

## Verified Result

The decoder processed 2,000 live STM32-generated CAN frames through the Waveshare live serial backend with:

```text
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

---

## Output Files

The live decoder run writes:

```text
output/fault_summary.json
```

The diagnostic report agent then writes:

```text
output/diagnostic_report.md
```

---

## Future Work

```text
Linux SocketCAN can0 backend
candump parser
production-grade reconnect/error recovery
```
