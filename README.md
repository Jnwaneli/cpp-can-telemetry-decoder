# C++ CAN Telemetry Decoder and Fault Analyzer

## Overview

This project is a C++ CAN telemetry decoder and fault analyzer for simulated, logged, and STM32-generated CAN frames.

The desktop C++ application reads CAN frame logs, validates message IDs and DLC values, decodes little-endian payloads, extracts status flags with bit masks, tracks signal statistics, detects faults, and prints a structured diagnostic summary.

The project also includes an STM32 FreeRTOS CAN telemetry sender that generates live simulated telemetry values and transmits CAN frames through an SN65HVD230 CAN transceiver to a Waveshare USB-CAN adapter.

Current PC-side input is log/captured-frame based. A direct live USB-CAN reader inside the C++ application is planned as future work.

---

## Motivation

The goal of this project is to connect desktop-side C++ systems programming with embedded CAN telemetry and real hardware testing.

This project demonstrates:

```text
CAN frame parsing
byte-level payload decoding
little-endian signal extraction
bit-mask status flag decoding
dispatcher-based message routing
fault detection and fault summaries
running signal statistics
FreeRTOS task design
queue and mutex usage
STM32 FDCAN transmission
USB-CAN hardware capture workflow
```

---

## Features

```text
CSV CAN log parser
CAN frame validation
CANDispatcher routing by CAN ID
CircularBuffer receive pipeline
0x100 Analog Inputs decoder
0x101 Battery and Temperature decoder
0x102 Status Flags decoder
0x200 Vehicle Telemetry decoder
FaultReport and FrameReport structures
SignalStats min/max/average tracking
Dropped counter detection with 8-bit wrap-around
Possible stuck sensor warning
Fault summary output
STM32 FreeRTOS CAN telemetry sender
Waveshare USB-CAN receive workflow
Captured hardware-generated CAN log analysis
```

---

## Project Structure

```text
cpp-can-telemetry-decoder/
├── README.md
├── main.cpp
│
├── include/
│   ├── can_frame.hpp
│   ├── can_validation.hpp
│   ├── can_dispatcher.hpp
│   ├── can_log_parser.hpp
│   ├── circular_buffer.hpp
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
│   └── live_reading_test.md
│
├── embedded/
│   └── can_hardware_bridge/
│       └── stm32_can_sender_freertos/
│           ├── Core/
│           │   ├── Inc/
│           │   └── Src/
│           │       └── main.c
│           ├── docs/
│           │   ├── freertos_architecture.md
│           │   ├── task_table.md
│           │   ├── queue_mutex_notes.md
│           │   └── live_reading_test.md
│           └── README.md
│
└── .gitignore
```

---

## CAN Protocol

The decoder currently supports four standard CAN message IDs.

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
```

Full protocol documentation is available in:

```text
docs/protocol.md
```

---

## CAN ID `0x100` — Analog Inputs

Payload format:

```text
Byte 0: AIN1 low byte
Byte 1: AIN1 high byte
Byte 2: AIN2 low byte
Byte 3: AIN2 high byte
Byte 4: AIN3 low byte
Byte 5: AIN3 high byte
Byte 6: status flags
Byte 7: counter
```

Status byte:

```text
Bit 0 = Sensor 1 valid
Bit 1 = Sensor 2 valid
Bit 2 = Sensor 3 valid
Bit 7 = Error flag
```

Example:

```text
0x100 DLC 8 Data: 00 08 10 00 FF 0A 07 01
```

Decoded:

```text
AIN1_RAW = 2048
AIN2_RAW = 16
AIN3_RAW = 2815
Status = 0x07
Counter = 1
```

---

## CAN ID `0x101` — Battery and Temperature

Payload format:

```text
Byte 0: battery_mV low byte
Byte 1: battery_mV high byte
Byte 2: temperature_deciC low byte
Byte 3: temperature_deciC high byte
Byte 4-7: reserved
```

Scaling:

```text
battery_V = battery_mV / 1000.0
temperature_C = temperature_deciC / 10.0
```

Example:

```text
0x101 DLC 8 Data: 38 31 59 01 00 00 00 00
```

Decoded:

```text
Battery_mV = 12600
Battery = 12.60 V
Temperature_deciC = 345
Temperature = 34.5 C
```

---

## CAN ID `0x102` — Status Flags

Payload format:

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4-7: reserved
```

Example:

```text
0x102 DLC 8 Data: 07 00 01 00 00 00 00 00
```

Decoded:

```text
Sensor1 Valid = YES
Sensor2 Valid = YES
Sensor3 Valid = YES
System Fault Byte = 0x00
Mode = 1
Error Code = 0
```

---

## CAN ID `0x200` — Vehicle Telemetry

Payload format:

```text
Byte 0: speed_raw low byte
Byte 1: speed_raw high byte
Byte 2: rpm low byte
Byte 3: rpm high byte
Byte 4: gear
Byte 5: throttle_percent
Byte 6: brake_percent
Byte 7: counter
```

Example:

```text
0x200 DLC 8 Data: D2 04 AC 0D 03 2D 00 04
```

Decoded:

```text
Speed_RAW = 1234
RPM = 3500
Gear = 3
Throttle_Percent = 45%
Brake_Percent = 0%
Counter = 4
```

Speed scaling is planned but not implemented yet.

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

Current fault thresholds:

```text
Battery voltage too low: below 10.5 V
Battery voltage too high: above 14.8 V
Temperature too high: above 80.0 C
Analog raw ADC range: 0 to 4095
Stuck AIN1 warning: same AIN1 value for 20 active frames
```

The stuck sensor check is treated as a warning, not a guaranteed fault, because a stable sensor value can be valid depending on system conditions.

Full fault rule documentation is available in:

```text
docs/fault_rules.md
```

---

## Dropped Counter Detection

The decoder tracks counters per CAN ID.

Expected behavior:

```text
expected_counter = previous_counter + 1
```

If the current counter skips the expected value, the decoder reports a dropped counter fault.

Example:

```text
Previous counter: 5
Expected counter: 6
Actual counter: 8
Result: dropped counter fault
```

The decoder also handles 8-bit wrap-around:

```text
Previous counter: 255
Current counter: 0
Result: OK
```

---

## Signal Statistics

The decoder tracks running statistics for telemetry signals.

Tracked values:

```text
AIN1 min/max/average
AIN2 min/max/average
AIN3 min/max/average
Battery min/max/average
Temperature min/max/average
```

The running average is updated without storing every previous sample.

Formula:

```text
average = previous_average + (new_value - previous_average) / count
```

---

## Sample Output

When running the decoder with `data/sample_can_log.csv`, the expected final summary is:

```text
Summary:
Decoder Stats:
Frames processed: 10
Valid frames: 8
Unknown IDs: 1
Invalid DLC: 1
Faults: 3
Warnings: 0

Fault Summary:
Invalid DLC: 1
Unknown IDs: 1
Voltage faults: 0
Temperature faults: 0
Sensor invalid faults: 0
Dropped frames: 1
Possible stuck sensors: 0
Other faults: 0
```

The three expected faults are:

```text
1 dropped counter fault
1 unknown CAN ID fault
1 invalid DLC fault
```

The dropped counter fault comes from a `0x100` counter sequence that skips a value.

Example:

```text
Previous counter: 2
Expected counter: 3
Actual counter: 4
```

The decoder reports:

```text
Dropped frame counter detected for ID 0x100: expected 3, got 4
```

A full sample output explanation is available in:

```text
docs/sample_output.md
```

---

## Build Instructions

### Requirements

```text
C++17-compatible compiler
PowerShell or terminal
g++ or another compatible C++ compiler
```

This project was developed and tested using C++17.

---

### Build on Windows with g++

From the repository root:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

Run:

```powershell
.\main.exe
```

---

### Input File

The current desktop decoder reads:

```text
data/sample_can_log.csv
```

To test another log, temporarily copy it over `data/sample_can_log.csv`.

Example:

```powershell
Copy-Item data\sample_can_log.csv data\sample_can_log_backup.csv
Copy-Item data\good_frames.csv data\sample_can_log.csv
.\main.exe
Copy-Item data\sample_can_log_backup.csv data\sample_can_log.csv
```

---

### Test Files

Current test files:

```text
data/sample_can_log.csv
data/good_frames.csv
data/fault_frames.csv
data/stuck_sensor_test.csv
data/freertos_captured_log.csv
```

Expected purpose:

```text
sample_can_log.csv:
    Main decoder demonstration log.

good_frames.csv:
    Valid frames with no expected faults.

fault_frames.csv:
    Intentional fault coverage.

stuck_sensor_test.csv:
    Possible stuck sensor warning test.

freertos_captured_log.csv:
    Captured STM32 FreeRTOS CAN frames from Waveshare.
```

---

## Hardware Integration

The STM32 hardware bridge is working and successfully sends STM32-generated CAN frames to the PC through a USB-CAN adapter.

Hardware path:

```text
NUCLEO-G431RB
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
PC CAN capture
        ↓
C++ CAN Telemetry Decoder and Fault Analyzer
```

Hardware status:

```text
NUCLEO-G431RB wired to SN65HVD230 CAN transceiver
SN65HVD230 wired to Waveshare USB-CAN adapter
CANH/CANL termination measured about 60 ohms with power off
Waveshare USB-CAN successfully receives frames from STM32
CAN bitrate: 500 kbps
Frame type: Standard Classic CAN
DLC: 8 bytes
```

Current hardware status:

```text
STM32 FreeRTOS CAN telemetry sender: Implemented
SN65HVD230 CAN transceiver path: Tested
Waveshare USB-CAN receive: Tested
Captured/logged frame analysis in C++ decoder: Implemented
Direct live USB-CAN reader inside C++ app: Planned
```

---

## STM32 FreeRTOS Telemetry Sender

The STM32 FreeRTOS telemetry sender is located at:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

The firmware generates simulated live telemetry values, processes them through FreeRTOS tasks, and transmits CAN frames through STM32 FDCAN.

STM32CubeIDE / CubeMX setup:

```text
FDCAN1 enabled
Mode: Normal
Frame format: Classic CAN
ID type: Standard ID
CAN bitrate: 500 kbps
FreeRTOS enabled with CMSIS_V2
HAL timebase changed to TIM6
USE_NEWLIB_REENTRANT enabled
BSP LED control kept
Status LED uses BSP_LED_Toggle(LED_GREEN)
```

The physical path is:

```text
NUCLEO-G431RB
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
PC receive software
        ↓
Captured CSV log
        ↓
C++ CAN Telemetry Decoder
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

---

## FreeRTOS Task Architecture

Current RTOS architecture:

```text
SignalGeneratorTask
        ↓
sensorQueueHandle
        ↓
ProcessingTask
        ↓
mutex-protected latestTelemetry
        ↓
CanTxTask
        ↓
STM32 FDCAN
        ↓
SN65HVD230
        ↓
Waveshare USB-CAN
        ↓
PC
```

### Tasks

| Task | Priority | Stack | Responsibility |
|---|---:|---:|---|
| `SignalGeneratorTask` | `osPriorityNormal` | 128 words | Generates fake live telemetry every 10 ms |
| `ProcessingTask` | `osPriorityNormal` | 128 words | Waits on `sensorQueueHandle` and copies `SensorSample` into `latestTelemetry` using `telemetryMutexHandle` |
| `CanTxTask` | `osPriorityAboveNormal` | 256 words | Reads `latestTelemetry`, owns the transmit counter, and sends CAN frames through FDCAN |
| `StatusLedTask` | `osPriorityLow` | 128 words | Blinks heartbeat LED every 500 ms |

### FreeRTOS Objects

```text
sensorQueueHandle
telemetryMutexHandle
latestTelemetry
```

The queue transfers generated telemetry samples from `SignalGeneratorTask` to `ProcessingTask`.

The mutex protects `latestTelemetry`, which is written by `ProcessingTask` and read by `CanTxTask`.

---

## CAN Frames Sent by FreeRTOS Firmware

The STM32 FreeRTOS sender transmits all four CAN IDs supported by the C++ decoder:

| CAN ID | Name |
|---|---|
| `0x100` | Analog Inputs |
| `0x101` | Battery and Temperature |
| `0x102` | Status Flags |
| `0x200` | Vehicle Telemetry |

Waveshare confirms all four IDs are received repeatedly:

```text
0x100
0x101
0x102
0x200
```

Current send behavior:

```text
CanTxTask sends a burst of 0x100, 0x101, 0x102, and 0x200.
Small delays are inserted between individual CAN sends.
The transmit cycle runs around every 100 ms unless slowed for testing.
Because each cycle sends 4 frames, this appears as about 40 rows/sec in Waveshare.
```

---

## FreeRTOS CAN Counter Design

`CanTxTask` owns the CAN transmit counter.

`SignalGeneratorTask` may have its own fake sample counter, but that counter is not used as the CAN frame counter.

This avoids false dropped-frame reports in the desktop C++ decoder.

Reason:

```text
SignalGeneratorTask runs every 10 ms.
CanTxTask transmits every 100 ms.
If SignalGeneratorTask owned the CAN counter, the transmitted counter could jump by about 10 each cycle.
The C++ decoder would incorrectly report dropped frames.
```

Correct behavior:

```text
SignalGeneratorTask generates sample values.
ProcessingTask updates latestTelemetry.
CanTxTask sends CAN frames and increments the transmit counter once per transmit cycle.
```

---

## FreeRTOS Debug Note

During testing, `0x200` was initially missing from the Waveshare receive output.

Likely cause:

```text
The FDCAN Tx FIFO was filling after 0x100, 0x101, and 0x102 were queued.
```

Fix:

```text
Wait for transmit FIFO space and/or add small delays between CAN sends.
```

After this fix, Waveshare received all four IDs repeatedly:

```text
0x100
0x101
0x102
0x200
```

---

## Captured FreeRTOS Log Integration

Captured Waveshare frames are stored in:

```text
data/freertos_captured_log.csv
```

Example CSV format:

```csv
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
100,8,7A,0D,DE,0D,42,0E,07,4B
101,8,48,31,5F,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,80,02,0A,0F,03,29,00,4B
```

The C++ decoder verifies:

```text
0x100 decodes as Analog Inputs
0x101 decodes as Battery and Temperature
0x102 decodes as Status Flags
0x200 decodes as Vehicle Telemetry
Dropped frames should be 0 if counters are sequential
```

---

## Implemented / Planned

### Implemented

```text
C++ CAN frame structure
CSV CAN log parser
CircularBuffer receive pipeline
CANDispatcher
TelemetryDecoder
FaultAnalyzer
FaultReport
FrameReport
SignalStats
CounterTracker
StuckSensorTracker
0x100 decoder
0x101 decoder
0x102 decoder
0x200 decoder
Dropped counter detection
Possible stuck sensor warning
Fault summary output
STM32 FreeRTOS telemetry sender
FreeRTOS queue and mutex architecture
CanTxTask CAN transmission
StatusLedTask heartbeat
Waveshare receive workflow
Captured FreeRTOS CSV log workflow
```

### Planned

```text
Direct live USB-CAN reader in the C++ application
Linux SocketCAN support
candump log parser
live can0 input backend
fault_summary.json output
AI-assisted diagnostic report generator
more realistic vehicle signal scaling
more fault injection patterns
real ADC or sensor-based telemetry on STM32
optional FreeRTOS FaultInjectTask
additional automated tests
```

---

## Limitations

Current limitations:

```text
The desktop C++ application reads CSV/captured logs, not live USB-CAN traffic directly.
The STM32 FreeRTOS sender currently generates simulated telemetry, not real ADC sensor readings.
Speed scaling for 0x200 is not implemented yet.
0x102 system fault byte rules are not fully implemented yet.
Testing alternate input files currently requires copying them over data/sample_can_log.csv.
The AI diagnostic assistant is planned but not implemented yet.
Linux SocketCAN support is planned but not implemented yet.
```

Important wording:

```text
The project currently supports live CAN transmission from STM32 and live receive through Waveshare.
The C++ decoder currently analyzes saved captures from that live traffic.
Direct live USB-CAN reading inside the C++ application is future work.
```

---

## Future Work

Planned extensions:

```text
Linux SocketCAN support
candump log parser
live can0 input backend
AI-assisted diagnostic report generator
more realistic vehicle signal scaling
more fault injection patterns
direct Waveshare USB-CAN reader
fault_summary.json output
unit tests for parser, decoder, counters, and fault rules
real ADC or sensor-based telemetry on STM32
optional FreeRTOS FaultInjectTask
```

### Linux SocketCAN Support

Add support for Linux SocketCAN so the decoder can eventually read from a native CAN interface such as:

```text
can0
```

Planned workflow:

```text
STM32 CAN sender
        ↓
CAN adapter on Linux
        ↓
can0
        ↓
C++ live SocketCAN backend
        ↓
CanFrame
        ↓
CanDispatcher
        ↓
TelemetryDecoder
```

---

### candump Log Parser

Add support for Linux `candump` logs.

Example future input format:

```text
can0 100 [8] 00 08 10 00 FF 0A 07 01
```

This would make the decoder more compatible with common Linux CAN workflows.

---

### Live can0 Input Backend

Add a live input backend that reads CAN frames directly from `can0`.

This would move the project from:

```text
captured CSV decoding
```

to:

```text
direct live CAN decoding
```

---

### AI-Assisted Diagnostic Report

Add an AI-assisted report generator that reads deterministic decoder output and produces a human-readable diagnostic report.

Important design rule:

```text
The C++ decoder remains the source of truth.
The AI assistant explains the results but does not replace the fault logic.
```

Possible workflow:

```text
C++ decoder
        ↓
fault_summary.json
        ↓
AI-assisted report generator
        ↓
diagnostic_report.md
```

---

### More Realistic Vehicle Signal Scaling

Future versions can add realistic scaling for:

```text
speed
rpm limits
throttle range
brake range
gear range
temperature ranges
battery operating states
```

Example:

```text
speed_mph = speed_raw * scale_factor
```

---

### More Fault Injection Patterns

Additional simulated fault patterns could include:

```text
intermittent sensor invalid flags
repeated dropped counters
slowly rising temperature
low battery during high load
random corrupted DLC
burst traffic overload
stuck throttle
invalid gear value
```

These patterns would improve test coverage and make the demo more realistic.

---

## Final Project Story

This project implements a C++ CAN Telemetry Decoder and Fault Analyzer that decodes simulated, logged, and STM32-generated CAN telemetry frames.

The decoder parses little-endian payloads, extracts bit-masked status flags, detects invalid frames and signal faults, tracks running statistics, detects dropped counters, and produces a structured fault summary.

The embedded side uses an STM32 FreeRTOS CAN telemetry sender to generate live simulated telemetry, pass data through a queue and mutex-protected shared state, and transmit CAN frames through an SN65HVD230 transceiver to a Waveshare USB-CAN adapter.

The current workflow supports hardware-generated CAN capture and PC-side log analysis, with direct live reading, SocketCAN support, and AI-assisted diagnostic reporting planned as future extensions.