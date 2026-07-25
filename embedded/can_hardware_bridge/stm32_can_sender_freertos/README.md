# STM32 FreeRTOS CAN Telemetry Sender

## Overview

This firmware project runs on the STM32 NUCLEO-G431RB and transmits simulated telemetry data over CAN using FreeRTOS.

The firmware generates fake live sensor values, passes them through a FreeRTOS queue, stores the latest processed telemetry using a mutex-protected shared state, and transmits CAN frames through the STM32 FDCAN peripheral.

The physical CAN signal path is:

```text
NUCLEO-G431RB FDCAN
        ↓
SN65HVD230 CAN transceiver
        ↓
CANH/CANL bus
        ↓
Waveshare USB-CAN adapter
        ↓
PC receive software
```

The captured CAN frames can then be saved and analyzed by the desktop C++ CAN Telemetry Decoder and Fault Analyzer.

---

## Current Status

```text
FreeRTOS scheduler: working
SignalGeneratorTask: working
ProcessingTask: working
CanTxTask: working
StatusLedTask: working
FDCAN transmit: working
SN65HVD230 CAN bridge: working
Waveshare USB-CAN receive: working
Multi-frame CAN transmit: working
```

The firmware successfully transmits the following standard CAN IDs:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

All frames use:

```text
Classic CAN
Standard CAN ID
DLC = 8
Bitrate = 500 kbps
Little-endian 16-bit signal packing
```

---

## FreeRTOS Sender Summary

The FreeRTOS sender uses four tasks:

```text
SignalGeneratorTask
ProcessingTask
CanTxTask
StatusLedTask
```

It also uses:

```text
SensorSample queue
telemetry mutex
latestTelemetry shared structure
100 ms CAN transmit period
```

Task pipeline:

```text
SignalGeneratorTask
        ↓
sensorQueueHandle
        ↓
ProcessingTask
        ↓
telemetryMutexHandle protects latestTelemetry
        ↓
CanTxTask
        ↓
STM32 FDCAN
```

### SignalGeneratorTask

`SignalGeneratorTask` generates fake live telemetry values.

It runs about every:

```text
10 ms
```

It sends `SensorSample` messages into:

```text
sensorQueueHandle
```

---

### ProcessingTask

`ProcessingTask` waits on the `SensorSample` queue.

When a sample arrives, it updates:

```text
latestTelemetry
```

The update is protected by:

```text
telemetryMutexHandle
```

---

### CanTxTask

`CanTxTask` sends CAN frames about every:

```text
100 ms
```

It sends:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

`CanTxTask` owns the CAN transmit counter.

This prevents false dropped-frame detection in the desktop C++ decoder.

---

### StatusLedTask

`StatusLedTask` toggles the board LED as a heartbeat.

Current blink period:

```text
500 ms
```

Current LED function:

```c
BSP_LED_Toggle(LED_GREEN);
```

---

## Hardware Used

```text
NUCLEO-G431RB
SN65HVD230 CAN transceiver module
Waveshare USB-CAN adapter
120 ohm termination resistors
Jumper wires
Breadboard
PC running Waveshare receive software
```

The measured CANH-to-CANL termination resistance with power off was about:

```text
60 ohms
```

This indicates two 120 ohm terminations are active on the CAN bus.

---

## CubeMX / STM32CubeIDE Configuration

### FDCAN

```text
Peripheral: FDCAN1
Mode: Normal
Frame Format: Classic CAN
ID Type: Standard ID
DLC: 8 bytes
Bitrate: 500 kbps
TX Mode: Tx FIFO/Queue
```

Both the STM32 and Waveshare USB-CAN adapter must use the same bitrate:

```text
STM32 FDCAN bitrate = 500 kbps
Waveshare bitrate   = 500 kbps
```

### FreeRTOS

```text
Interface: CMSIS_V2
HAL Timebase Source: TIM6
USE_NEWLIB_REENTRANT: Enabled
```

### Tasks

| Task | Priority | Stack | Purpose |
|---|---:|---:|---|
| `SignalGeneratorTask` | `osPriorityNormal` | 128 words | Generates fake live telemetry samples |
| `ProcessingTask` | `osPriorityNormal` | 128 words | Receives samples and updates shared telemetry |
| `CanTxTask` | `osPriorityAboveNormal` | 256 words | Packages and transmits CAN frames |
| `StatusLedTask` | `osPriorityLow` | 128 words | Blinks heartbeat LED |

---

## FreeRTOS Architecture

```text
SignalGeneratorTask
        ↓ sends SensorSample
sensorQueueHandle
        ↓ receives SensorSample
ProcessingTask
        ↓ writes with mutex
latestTelemetry
        ↓ reads with mutex
CanTxTask
        ↓ sends CAN frames
STM32 FDCAN
```

### FreeRTOS Objects

```text
sensorQueueHandle
telemetryMutexHandle
latestTelemetry
```

The queue transfers data from `SignalGeneratorTask` to `ProcessingTask`.

The mutex protects `latestTelemetry`, which is written by `ProcessingTask` and read by `CanTxTask`.

Additional FreeRTOS architecture documentation is available in:

```text
docs/freertos_architecture.md
docs/task_table.md
docs/queue_mutex_notes.md
docs/live_reading_test.md
```

---

## Task Responsibilities

### SignalGeneratorTask

`SignalGeneratorTask` generates fake live telemetry every 10 ms.

It produces:

```text
AIN1 raw value
AIN2 raw value
AIN3 raw value
battery_mV
temperature_deciC
speed_raw
rpm
gear
throttle_percent
brake_percent
status
timestamp_ms
```

It does not transmit CAN directly.

---

### ProcessingTask

`ProcessingTask` waits on `sensorQueueHandle`.

When a `SensorSample` arrives, it copies the data into `latestTelemetry`.

The copy is protected by `telemetryMutexHandle`.

This prevents `CanTxTask` from reading partially updated data.

---

### CanTxTask

`CanTxTask` reads `latestTelemetry` using the mutex, copies it into a local variable, releases the mutex, and then transmits CAN frames.

This is important because the task should not hold the mutex while sending multiple CAN frames.

`CanTxTask` owns the CAN transmit counter.

This prevents false dropped-frame detection in the desktop decoder. `SignalGeneratorTask` runs faster than `CanTxTask`, so the CAN transmit counter should increment only once per transmit cycle.

---

### StatusLedTask

`StatusLedTask` toggles the board LED as a heartbeat.

```text
LED blink period: 500 ms
```

The project currently uses BSP LED control:

```c
BSP_LED_Toggle(LED_GREEN);
```

---

## CAN Frames Sent

### CAN ID `0x100` — Analog Inputs

Payload:

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

---

### CAN ID `0x101` — Battery and Temperature

Payload:

```text
Byte 0: battery_mV low byte
Byte 1: battery_mV high byte
Byte 2: temperature_deciC low byte
Byte 3: temperature_deciC high byte
Byte 4: reserved
Byte 5: reserved
Byte 6: reserved
Byte 7: reserved
```

---

### CAN ID `0x102` — Status Flags

Payload:

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4: reserved
Byte 5: reserved
Byte 6: reserved
Byte 7: reserved
```

Current normal status frame:

```text
07 00 01 00 00 00 00 00
```

Meaning:

```text
Sensor 1 valid
Sensor 2 valid
Sensor 3 valid
Mode = 1
Error code = 0
```

---

### CAN ID `0x200` — Vehicle Telemetry

Payload:

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

---

## Little-Endian Packing

The firmware sends 16-bit values in little-endian format.

Example:

```text
value = 0x1234
low byte  = 0x34
high byte = 0x12
```

So the payload stores:

```text
34 12
```

This matches the desktop C++ decoder’s little-endian unpacking logic.

---

## Expected Waveshare Receive Pattern

The Waveshare USB-CAN receive tool should repeatedly show:

```text
0x100 DLC 8
0x101 DLC 8
0x102 DLC 8
0x200 DLC 8
```

The sender transmits a burst of four CAN frames each transmit cycle.

The transmit cycle is approximately every 100 ms unless slowed for testing.

Because four frames are sent per cycle, the Waveshare receive window may show many rows quickly.

---

## Important Fixes Made

### Missing `0x200` Frame

At first, `0x200` was not appearing in the Waveshare receive window.

Cause:

```text
0x100, 0x101, and 0x102 were queued first.
0x200 could be skipped if the FDCAN TX FIFO was full.
```

Fix:

```text
Wait for TX FIFO space before sending.
Add small delays between frame sends.
```

After the fix, Waveshare received all four CAN IDs:

```text
0x100
0x101
0x102
0x200
```

---

### Counter Ownership

The CAN transmit counter is owned by `CanTxTask`.

Reason:

```text
SignalGeneratorTask runs every 10 ms.
CanTxTask transmits about every 100 ms.
```

If the generator owned the counter, transmitted frames could jump like:

```text
0, 10, 20, 30
```

The desktop C++ decoder would correctly interpret that as dropped frames.

So the transmit counter increments only in `CanTxTask`.

---

## Build and Flash

Open this project in STM32CubeIDE:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

Build:

```text
Project → Build Project
```

Flash/run:

```text
Run
```

or:

```text
Debug → Resume
```

---

## Test Procedure

1. Connect the NUCLEO-G431RB to the SN65HVD230 transceiver.
2. Connect SN65HVD230 CANH/CANL to the Waveshare USB-CAN adapter.
3. Confirm common ground.
4. Confirm CANH-to-CANL termination is about 60 ohms with power off.
5. Power the board.
6. Open the Waveshare receive software.
7. Set bitrate to 500 kbps.
8. Flash and run the STM32 firmware.
9. Confirm the receive window shows:

```text
0x100
0x101
0x102
0x200
```

Pass condition:

```text
All four CAN IDs are received repeatedly.
DLC is 8 for each frame.
Status LED continues blinking.
No hard fault occurs.
```

---

## Captured Log Workflow

After receiving frames in Waveshare, export or manually convert the capture into:

```text
data/freertos_captured_log.csv
```

Expected desktop decoder CSV format:

```csv
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
100,8,7A,0D,DE,0D,42,0E,07,4B
101,8,48,31,5F,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,80,02,0A,0F,03,29,00,4B
```

Then the desktop C++ decoder can process the captured frames as logged CAN data.

---

## Current Limitations

```text
Telemetry values are simulated, not real sensor readings.
The firmware transmits generated test data only.
The C++ app does not directly read live USB-CAN traffic yet.
Captured log conversion is currently manual.
Fault injection is not implemented in the firmware yet.
```

Important wording:

```text
The STM32 FreeRTOS firmware generates and transmits live CAN frames.
The Waveshare USB-CAN adapter receives those frames live.
The C++ decoder analyzes saved CSV captures from that live traffic.
Direct live USB-CAN reading inside the C++ app is planned future work.
```

---

## Future Work

```text
Add fault injection frames
Add optional FaultInjectTask
Add direct live USB-CAN reader on the PC side
Add Linux SocketCAN/can0 workflow
Add candump-style parser
Send real ADC or sensor values instead of simulated data
Add more realistic vehicle signal scaling
Add more fault injection patterns
Generate fault_summary.json from the desktop decoder
Add AI-assisted diagnostic report generation
```

---

## Project Role

This firmware is the embedded sender side of the larger C++ CAN telemetry project.

The full workflow is:

```text
STM32 FreeRTOS firmware
        ↓ sends CAN frames
SN65HVD230 transceiver
        ↓
Waveshare USB-CAN adapter
        ↓ captures frames
CSV log
        ↓
Desktop C++ CAN Decoder and Fault Analyzer
```

The purpose of this firmware is to prove that the desktop C++ decoder can be connected to a real embedded CAN telemetry source.