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
│   └── usb_can_capture_notes.md
│
├── embedded/
│   └── can_hardware_bridge/
│       ├── stm32_can_sender/
│       │   ├── Core/
│       │   └── README.md
│       │
│       └── stm32_can_sender_freertos/
│           ├── Core/
│           │   ├── Inc/
│           │   └── Src/
│           │       └── main.c
│           ├── docs/
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

## Example Output

Example decoded frame report:

```text
Decoded Frame Report:
Frame 1: 0x100 Analog Inputs - OK
Frame 2: 0x101 Battery and Temperature - OK
Frame 3: 0x102 Status Flags - OK
Frame 4: 0x100 Analog Inputs - OK
Frame 5: 0x101 Battery and Temperature - OK
Frame 6: 0x100 Analog Inputs - FAULT: Dropped frame counter detected for ID 0x100: expected 3, got 4
Frame 7: 0x200 Vehicle Telemetry - OK
Frame 8: 0x101 Battery and Temperature - OK
Frame 9: 0x999 Unknown - FAULT: Unknown CAN ID
Frame 10: 0x100 Analog Inputs - FAULT: Invalid DLC
```

Example signal statistics:

```text
Signal Stats:
AIN1_RAW: min=800, max=3124, avg=1990.67, count=3
AIN2_RAW: min=16, max=1600, avg=1005.33, count=3
AIN3_RAW: min=2337, max=2815, avg=2528.00, count=3
Battery_mV: min=12000, max=13000, avg=12533.33, count=3
Temperature_deciC: min=300, max=400, avg=348.33, count=3
Battery average: 12.53 V
Temperature average: 34.8 C
```

Example summary:

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

---

## Build Instructions

### Requirements

```text
C++17-compatible compiler
PowerShell or terminal
```

Tested with `g++` using C++17.

### Build

From the repo root:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

### Run

```powershell
.\main.exe
```

The program currently reads:

```text
data/sample_can_log.csv
```

---

## Input Log Format

CSV format:

```csv
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
100,8,00,08,10,00,FF,0A,07,01
101,8,38,31,59,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,D2,04,AC,0D,03,2D,00,04
```

Notes:

```text
CAN IDs are parsed as hexadecimal.
DLC is parsed as decimal.
Payload bytes are parsed as hexadecimal.
Each row must contain 10 comma-separated fields.
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
STM32 simple CAN sender: Implemented
STM32 FreeRTOS CAN telemetry sender: Implemented
SN65HVD230 CAN transceiver path: Tested
Waveshare USB-CAN receive: Tested
Captured/logged frame analysis in C++ decoder: In Progress
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

The next desktop integration step is to export captured Waveshare frames into:

```text
data/freertos_captured_log.csv
```

Expected CSV format:

```csv
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
100,8,00,08,10,00,FF,0A,07,01
101,8,38,31,59,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,D2,04,AC,0D,03,2D,00,04
```

The C++ decoder should verify:

```text
0x100 decodes as Analog Inputs
0x101 decodes as Battery and Temperature
0x102 decodes as Status Flags
0x200 decodes as Vehicle Telemetry
Dropped frames should be 0 if counters are sequential
```

---

## Implemented / In Progress / Planned

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
STM32 simple CAN sender
STM32 FreeRTOS telemetry sender
FreeRTOS queue and mutex architecture
CanTxTask CAN transmission
StatusLedTask heartbeat
Waveshare receive workflow
```

### In Progress

```text
Captured FreeRTOS log export
data/freertos_captured_log.csv
Desktop decoder verification using captured FreeRTOS frames
README polish
Final documentation cleanup
FreeRTOS documentation files
Public repo cleanup
```

### Planned

```text
Direct live USB-CAN reader in the C++ application
SocketCAN/can0 Linux workflow
candump-style log parser
fault_summary.json output
AI-assisted diagnostic report generator
additional automated tests
```

---

## Limitations

```text
The C++ decoder currently reads CSV/captured logs, not live USB-CAN frames directly.
The FreeRTOS telemetry values are simulated, not real sensor ADC readings yet.
Captured log workflow is currently manual.
Speed scaling for 0x200 is not implemented yet.
Stuck sensor detection is a warning and may produce false positives.
Direct live USB-CAN reading is planned as future work.
```

---

## Future Work

Planned extensions:

```text
Add direct Waveshare USB-CAN live reader
Add Linux SocketCAN/can0 workflow
Add candump-style parser
Generate output/fault_summary.json
Create AI-assisted diagnostic report generator
Add more sample logs for good and faulty frames
Add unit tests for parser, decoder, counters, and fault rules
Add optional FaultInjectTask in FreeRTOS firmware
```

---

## Final Project Story

This project implements a C++ CAN Telemetry Decoder and Fault Analyzer that decodes simulated, logged, and STM32-generated CAN telemetry frames.

The decoder parses little-endian payloads, extracts bit-masked status flags, detects invalid frames and signal faults, tracks running statistics, detects dropped counters, and produces a structured fault summary.

The embedded side uses an STM32 FreeRTOS CAN telemetry sender to generate live simulated telemetry, pass data through a queue and mutex-protected shared state, and transmit CAN frames through an SN65HVD230 transceiver to a Waveshare USB-CAN adapter.

The current workflow supports hardware-generated CAN capture and PC-side log analysis, with direct live reading, SocketCAN support, and AI-assisted diagnostic reporting planned as future extensions.