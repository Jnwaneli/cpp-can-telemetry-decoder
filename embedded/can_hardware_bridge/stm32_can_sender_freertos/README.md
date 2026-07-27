# STM32 FreeRTOS CAN Telemetry Sender

## Overview

This firmware runs on the STM32 NUCLEO-G431RB and transmits simulated telemetry data over CAN using FreeRTOS.

The firmware generates live simulated sensor values, passes them through a FreeRTOS queue, stores the latest processed telemetry in a mutex-protected shared state, and transmits CAN frames through the STM32 FDCAN peripheral.

The desktop C++ project can now receive these hardware-generated frames directly through the Waveshare USB-CAN serial workflow using `WaveshareSerialFrameSource`.

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
Desktop C++ live Waveshare ingestion: working
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

## Hardware Path

```text
NUCLEO-G431RB FDCAN
        ↓
SN65HVD230 CAN transceiver
        ↓
CANH/CANL bus
        ↓
Waveshare USB-CAN adapter
        ↓
PC COM port
        ↓
WaveshareSerialFrameSource
        ↓
Desktop C++ decoder
```

The measured CANH-to-CANL termination resistance with power off was about:

```text
60 ohms
```

This indicates two 120 ohm terminations are active on the CAN bus.

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
        ↓
SN65HVD230
        ↓
Waveshare USB-CAN
```

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

Both the STM32 and Waveshare USB-CAN adapter must use the same CAN bitrate:

```text
STM32 FDCAN bitrate = 500 kbps
Waveshare CAN bitrate = 500 kbps
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
| `SignalGeneratorTask` | `osPriorityNormal` | 128 words | Generates simulated live telemetry samples |
| `ProcessingTask` | `osPriorityNormal` | 128 words | Receives samples and updates shared telemetry |
| `CanTxTask` | `osPriorityAboveNormal` | 256 words | Packages and transmits CAN frames |
| `StatusLedTask` | `osPriorityLow` | 128 words | Blinks heartbeat LED |

---

## Task Responsibilities

### SignalGeneratorTask

`SignalGeneratorTask` generates simulated telemetry values about every 10 ms.

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

### ProcessingTask

`ProcessingTask` waits on `sensorQueueHandle`.

When a `SensorSample` arrives, it copies the data into `latestTelemetry`. The copy is protected by `telemetryMutexHandle`, which prevents `CanTxTask` from reading partially updated data.

### CanTxTask

`CanTxTask` reads `latestTelemetry` using the mutex, copies it into a local variable, releases the mutex, and then transmits CAN frames.

This is important because the task should not hold the mutex while sending multiple CAN frames.

`CanTxTask` owns the CAN transmit counter. This prevents false dropped-frame detection in the desktop decoder. `SignalGeneratorTask` runs faster than `CanTxTask`, so the CAN transmit counter should increment only once per transmit cycle.

### StatusLedTask

`StatusLedTask` toggles the board LED as a heartbeat.

```text
LED blink period: 500 ms
```

Current LED function:

```c
BSP_LED_Toggle(LED_GREEN);
```

---

## CAN Frames Sent

### CAN ID `0x100` — Analog Inputs

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

### CAN ID `0x101` — Battery and Temperature

```text
Byte 0: battery_mV low byte
Byte 1: battery_mV high byte
Byte 2: temperature_deciC low byte
Byte 3: temperature_deciC high byte
Byte 4-7: reserved
```

### CAN ID `0x102` — Status Flags

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4-7: reserved
```

Current normal status frame:

```text
07 00 01 00 00 00 00 00
```

### CAN ID `0x200` — Vehicle Telemetry

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
payload stores: 34 12
```

This matches the desktop C++ decoder's little-endian unpacking logic.

---

## Expected Waveshare Receive Pattern

The Waveshare USB-CAN receive path should repeatedly show:

```text
0x100 DLC 8
0x101 DLC 8
0x102 DLC 8
0x200 DLC 8
```

The sender transmits a burst of four CAN frames each transmit cycle.

The transmit cycle is approximately every 100 ms unless slowed for testing. Because four frames are sent per cycle, a 100-frame PC-side test represents 25 telemetry cycles.

---

## Desktop C++ Live Reader Test

From the desktop project root, build the C++ decoder with the Waveshare serial backend included:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/csv_frame_source.cpp src/waveshare_serial_frame_source.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

Close the Waveshare receive software before running live serial mode because the C++ application needs to open the COM port directly.

Run:

```powershell
.\main.exe --waveshare-serial COM4 100
```

Replace `COM4` with the actual COM port assigned to the Waveshare adapter.

Validated 100-frame live result:

```text
Frames processed: 100
Valid frames: 100
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

---

## Important Fixes Made

### Missing `0x200` Frame

At first, `0x200` was not appearing in the Waveshare receive output.

Likely cause:

```text
0x100, 0x101, and 0x102 were queued first.
0x200 could be skipped if the FDCAN TX FIFO was full.
```

Fix:

```text
Wait for TX FIFO space before sending.
Add small delays between frame sends.
```

After the fix, Waveshare received all four CAN IDs repeatedly.

### Counter Ownership

The CAN transmit counter is owned by `CanTxTask`.

Reason:

```text
SignalGeneratorTask runs every 10 ms.
CanTxTask transmits about every 100 ms.
```

If the generator owned the counter, transmitted frames could jump and the desktop C++ decoder would correctly interpret that as dropped frames.

So the transmit counter increments only in `CanTxTask`.

---

## Build and Flash Firmware

Open this project in STM32CubeIDE:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

Build:

```text
Project → Build Project
```

Flash:

```text
Run → Debug or Run → Run
```

Confirm:

```text
Status LED blinks
Waveshare receives 0x100, 0x101, 0x102, and 0x200
Desktop C++ live reader can decode the frames
```

---

## Current Limitations

```text
The firmware currently generates simulated telemetry values rather than real ADC sensor values.
The desktop live reader is implemented for the current Windows/Waveshare workflow.
SocketCAN can0 support is not implemented yet.
The AI diagnostic assistant is not implemented yet.
Production-grade serial reconnect/recovery behavior is future polish.
```

---

## Summary

This FreeRTOS firmware provides the live embedded CAN source for the C++ CAN Telemetry Decoder and Fault Analyzer.

The firmware side generates and transmits the CAN traffic. The desktop side receives it through the Waveshare USB-CAN serial backend and feeds it through the existing C++ validation, decoding, statistics, and fault-analysis pipeline.
