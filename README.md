# C++ CAN Telemetry Decoder and Fault Analyzer

## Overview

This project is a C++17 CAN telemetry decoder and fault analyzer for simulated, logged, STM32-generated, and live Waveshare USB-CAN frames.

The desktop C++ application validates CAN message IDs and DLC values, decodes little-endian payloads, extracts status flags with bit masks, tracks running signal statistics, detects faults, and prints structured diagnostic summaries.

The project also includes an STM32 FreeRTOS CAN telemetry sender that generates simulated live telemetry values and transmits CAN frames through an SN65HVD230 CAN transceiver to a Waveshare USB-CAN adapter.

The Week 9 upgrade adds a `FrameSource` input architecture and a direct Waveshare serial live reader on Windows. The C++ application can now open the Waveshare COM port, receive hardware-generated CAN frames, convert them into `CanFrame` objects, and feed them through the same dispatcher, decoder, and fault analyzer used by CSV mode.

---

## Current Status

```text
C++17 desktop decoder: working
CSV log decoding: working
FrameSource interface: working
CsvFrameSource backend: working
Live-style frame loop: working
WaveshareSerialFrameSource backend: working on Windows
STM32 FreeRTOS CAN sender: working
SN65HVD230 CAN transceiver path: tested
Waveshare USB-CAN live ingestion: tested
SocketCAN can0 backend: future work
fault_summary.json output: Week 9 Day 5
AI diagnostic assistant: Week 9 Day 6
```

---

## Key Features

```text
CanFrame common frame representation
FrameSource input interface
CsvFrameSource log backend
WaveshareSerialFrameSource live serial backend
CSV CAN log parser
CAN frame validation
CANDispatcher routing by CAN ID
TelemetryDecoder for supported message IDs
FaultAnalyzer summary tracking
SignalStats min/max/average tracking
CounterTracker dropped-frame detection
StuckSensorTracker warning logic
STM32 FreeRTOS CAN telemetry sender
Direct hardware-generated CAN ingestion through Waveshare USB-CAN
```

---

## System Architecture

```text
STM32 FreeRTOS sender
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
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
terminal summary
```

CSV mode uses the same decoder path:

```text
CsvFrameSource
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
```

---

## Project Structure

```text
cpp-can-telemetry-decoder/
├── README.md
├── main.cpp
├── .gitignore
│
├── include/
│   ├── can_frame.hpp
│   ├── can_validation.hpp
│   ├── can_dispatcher.hpp
│   ├── can_log_parser.hpp
│   ├── circular_buffer.hpp
│   ├── frame_source.hpp
│   ├── csv_frame_source.hpp
│   ├── waveshare_serial_frame_source.hpp
│   ├── telemetry_decoder.hpp
│   ├── telemetry_data.hpp
│   ├── fault_analyzer.hpp
│   ├── fault_report.hpp
│   ├── frame_report.hpp
│   ├── decoder_stats.hpp
│   ├── signal_stats.hpp
│   ├── counter_tracker.hpp
│   ├── stuck_sensor_tracker.hpp
│   ├── bit_utils.hpp
│   └── parser_state.hpp
│
├── src/
│   ├── can_dispatcher.cpp
│   ├── can_log_parser.cpp
│   ├── circular_buffer.cpp
│   ├── csv_frame_source.cpp
│   ├── waveshare_serial_frame_source.cpp
│   ├── telemetry_decoder.cpp
│   ├── fault_analyzer.cpp
│   ├── decoder_stats.cpp
│   ├── signal_stats.cpp
│   ├── counter_tracker.cpp
│   ├── stuck_sensor_tracker.cpp
│   └── bit_utils.cpp
│
├── data/
│   ├── sample_can_log.csv
│   ├── good_frames.csv
│   ├── fault_frames.csv
│   ├── stuck_sensor_test.csv
│   └── freertos_captured_log.csv
│
├── docs/
│   ├── protocol.md
│   ├── fault_rules.md
│   ├── sample_output.md
│   ├── hardware_log_status.md
│   ├── can_wiring_plan.md
│   ├── stm32_fdcan_config.md
│   ├── can_debug_checklist.md
│   ├── usb_can_capture_notes.md
│   ├── real_time_cpp_ingestion.md
│   └── live_waveshare_test_day4.md
│
├── embedded/
│   └── can_hardware_bridge/
│       └── stm32_can_sender_freertos/
│           ├── Core/
│           ├── docs/
│           └── README.md
│
└── tools/
    └── diagnostic_report_agent/        # Week 9 Day 6
```

---

## CAN Protocol

The decoder supports four standard Classic CAN message IDs.

| CAN ID | Name | Purpose |
|---|---|---|
| `0x100` | Analog Inputs | ADC-style analog sensor values |
| `0x101` | Battery and Temperature | Battery voltage and temperature |
| `0x102` | Status Flags | Sensor validity flags, mode, and error code |
| `0x200` | Vehicle Telemetry | Speed, RPM, gear, throttle, brake, and counter |

All supported frames currently use:

```text
Standard CAN ID
Classic CAN
DLC = 8
Little-endian 16-bit signal packing
CAN bitrate = 500 kbps
```

Full protocol documentation is in `docs/protocol.md`.

---

## Fault Detection

The analyzer currently detects:

```text
Invalid DLC
Unknown CAN ID
Low battery voltage
High battery voltage
High temperature
Sensor-invalid status flags
Dropped frame counters
Possible stuck AIN1 sensor warning
```

Current thresholds:

```text
Battery voltage too low: below 10.5 V
Battery voltage too high: above 14.8 V
Temperature too high: above 80.0 C
Analog raw ADC range: 0 to 4095
Stuck AIN1 warning: same AIN1 value for 20 active frames
```

The stuck sensor check is treated as a warning because a stable sensor value can be valid depending on system conditions.

Full fault rule documentation is in `docs/fault_rules.md`.

---

## Build Instructions

### Requirements

```text
C++17-compatible compiler
PowerShell or terminal
g++ or another compatible C++ compiler
Windows for direct Waveshare serial mode
```

### Build on Windows with g++

From the repository root:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/csv_frame_source.cpp src/waveshare_serial_frame_source.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

---

## Running the Decoder

### Default CSV Mode

Run the default sample log:

```powershell
.\main.exe
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

### CSV Mode with a Specific File

```powershell
.\main.exe --csv data\sample_can_log.csv
```

Other useful test files:

```text
data/good_frames.csv
data/fault_frames.csv
data/stuck_sensor_test.csv
data/freertos_captured_log.csv
```

### Waveshare Live Serial Mode

Close the Waveshare receive software before running this mode because the C++ application needs to open the COM port directly.

Find the adapter in Windows Device Manager:

```text
Device Manager → Ports (COM & LPT) → USB-SERIAL / CH340 / Waveshare adapter
```

Then run:

```powershell
.\main.exe --waveshare-serial COM4 100
```

Replace `COM4` with the actual COM port assigned to the Waveshare adapter.

The final argument is the frame limit. For example, this command reads and decodes 2,000 live CAN frames:

```powershell
.\main.exe --waveshare-serial COM4 2000
```

---

## Live Waveshare CAN Ingestion Status

The project now includes a direct Waveshare USB-CAN live ingestion path on Windows.

The C++ application opens the Waveshare COM port, configures the adapter for the project CAN workflow, reads incoming CAN packets, converts them into `CanFrame` objects, and feeds them through the same dispatcher, telemetry decoder, and fault analyzer used by CSV mode.

Validated 100-frame live test:

```text
Frames processed: 100
Valid frames: 100
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

Because the STM32 firmware transmits four CAN frame types per cycle, 100 total frames represents 25 telemetry cycles.

Extended demo target:

```text
Frames processed: 2000
Valid frames: 2000
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

A 2,000-frame run represents 500 telemetry cycles.

---

## Live Demo Media

Recommended demo assets:

```text
media/hardware_setup.jpg
media/stm32_sn65hvd230_wiring.jpg
media/waveshare_usb_can.jpg
media/live_decoder_summary.png
README video link from GitHub issue/comment upload
```

Do not commit large raw video files directly. Upload short MP4 demo videos through GitHub's web UI and link them from this README.

Suggested demo caption:

```text
Live CAN test: decoded 2,000 consecutive STM32-generated CAN frames through a Waveshare USB-CAN live serial backend at 500 kbps, with 0 unknown IDs, 0 invalid DLC errors, 0 dropped-frame faults, and 0 diagnostic warnings.
```

---

## STM32 FreeRTOS Telemetry Sender

The STM32 firmware project is located at:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

The FreeRTOS sender uses:

```text
SignalGeneratorTask
ProcessingTask
CanTxTask
StatusLedTask
SensorSample queue
telemetry mutex
latestTelemetry shared structure
100 ms CAN transmit period
```

The sender transmits these CAN IDs repeatedly:

```text
0x100 Analog Inputs
0x101 Battery and Temperature
0x102 Status Flags
0x200 Vehicle Telemetry
```

`CanTxTask` owns the CAN transmit counter so the desktop decoder does not falsely detect dropped frames.

---

## Implemented

```text
C++ CAN frame structure
FrameSource interface
CsvFrameSource backend
WaveshareSerialFrameSource backend
Live-style decoder loop
CSV CAN log parser
CANDispatcher
TelemetryDecoder
FaultAnalyzer
FaultReport and FrameReport
SignalStats
CounterTracker
StuckSensorTracker
0x100 decoder
0x101 decoder
0x102 decoder
0x200 decoder
Dropped counter detection
Possible stuck sensor warning
STM32 FreeRTOS telemetry sender
FreeRTOS queue and mutex architecture
Direct Waveshare USB-CAN live ingestion on Windows
```

---

## Planned / Future Work

```text
fault_summary.json output
AI-assisted diagnostic report generator
Linux SocketCAN support
candump log parser
live can0 input backend
more realistic vehicle signal scaling
more fault injection patterns
real ADC or sensor-based telemetry on STM32
optional FreeRTOS FaultInjectTask
unit tests for parser, decoder, counters, and fault rules
production-grade serial reconnect/error recovery
```

---

## Current Limitations

```text
The live Waveshare serial reader is implemented for the current Windows/Waveshare workflow.
SocketCAN can0 support is not implemented yet.
fault_summary.json output is the next Week 9 step.
The AI diagnostic assistant is not implemented yet.
The STM32 firmware currently generates simulated telemetry values rather than real ADC sensor values.
Speed scaling for 0x200 is not implemented yet.
0x102 system fault byte rules are not fully implemented yet.
The serial backend works for the demo workflow but does not yet include production-grade reconnect/recovery behavior.
```

---

## Final Project Story

This project connects desktop C++ decoding, fault analysis, STM32 FreeRTOS firmware, and real CAN hardware into one end-to-end telemetry workflow.

The STM32 FreeRTOS firmware generates live simulated telemetry frames and sends them over CAN through an SN65HVD230 transceiver. The Waveshare USB-CAN adapter receives those frames, and the C++ application can now read them directly through a live serial backend. Each received packet becomes a `CanFrame` and flows through the same validation, dispatcher, decoder, statistics, and fault-analysis pipeline used for CSV logs.
