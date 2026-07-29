# STM32 FreeRTOS CAN Telemetry Sender

## Overview

This firmware runs on the STM32 NUCLEO-G431RB and transmits simulated telemetry data over CAN using FreeRTOS.

The firmware generates live simulated sensor values, passes them through a FreeRTOS queue, stores the latest processed telemetry in a mutex-protected shared state, and transmits CAN frames through the STM32 FDCAN peripheral. It also drives a 4-module MAX7219 LED matrix for a button-gated visual fault demo.

The desktop C++ project receives the hardware-generated CAN frames through the Waveshare USB-CAN serial workflow using `WaveshareSerialFrameSource`.

---

## Current Status

```text
FreeRTOS scheduler: working
SignalGeneratorTask: working
ProcessingTask: working
CanTxTask: working
StatusLedTask: working
DisplayTask: working
FaultInjectTask: working
FDCAN transmit: working
SN65HVD230 CAN bridge: working
Waveshare USB-CAN receive: working
MAX7219 LED matrix display: working
Button-gated LED fault demo: working
Desktop C++ live Waveshare ingestion: working
2,000-frame clean live desktop decode test: passed
Side-by-side LED fault demo video: committed in /media
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

The MAX7219 LED matrix is connected separately to the NUCLEO GPIO pins and does not communicate with the desktop decoder directly. The LED state and the injected CAN fault payloads are synchronized because both are driven by the same firmware demo mode.

The measured CANH-to-CANL termination resistance with power off was about:

```text
60 ohms
```

This indicates two 120 ohm terminations are active on the CAN bus.

---

## Wiring Summary

### CAN wiring

```text
NUCLEO 3.3V       -> SN65HVD230 VCC
NUCLEO GND        -> SN65HVD230 GND
PA12 / FDCAN1_TX  -> SN65HVD230 TXD
PA11 / FDCAN1_RX  <- SN65HVD230 RXD
SN65HVD230 CANH   -> Waveshare CANH
SN65HVD230 CANL   -> Waveshare CANL
SN65HVD230 GND    -> Waveshare GND
```

### MAX7219 LED matrix wiring

```text
NUCLEO 5V   -> MAX7219 VCC
NUCLEO GND  -> MAX7219 GND
PB6 / D10   -> MAX7219 DIN
PB7         -> MAX7219 CLK
PB8 / D15   -> MAX7219 CS / LOAD
```

Detailed wiring documentation is in:

```text
docs/week10_led_fault_demo_wiring.md
```

---

## FreeRTOS Sender Summary

The FreeRTOS sender uses six tasks:

```text
SignalGeneratorTask
ProcessingTask
CanTxTask
StatusLedTask
DisplayTask
FaultInjectTask
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
        ↓
Desktop C++ decoder
```

LED demo pipeline:

```text
NUCLEO user button
        ↓
demoStarted flag
        ↓
FaultInjectTask cycles demo mode
        ↓
DisplayTask updates MAX7219 pattern
        ↓
CanTxTask injects matching CAN payload values
        ↓
Desktop decoder reports matching faults
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

| Task | Priority | Purpose |
|---|---:|---|
| `SignalGeneratorTask` | `osPriorityNormal` | Generates simulated live telemetry samples |
| `ProcessingTask` | `osPriorityNormal` | Receives samples and updates shared telemetry |
| `CanTxTask` | `osPriorityAboveNormal` | Packages and transmits CAN frames |
| `StatusLedTask` | `osPriorityLow` | Blinks heartbeat LED |
| `DisplayTask` | `osPriorityLow` | Drives the MAX7219 LED matrix |
| `FaultInjectTask` | `osPriorityLow` | Cycles the button-gated demo fault modes |

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

`CanTxTask` reads `latestTelemetry` using the mutex, copies it into a local variable, releases the mutex, applies the current demo fault mode, and then transmits CAN frames.

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

### DisplayTask

`DisplayTask` initializes the MAX7219 LED matrix and controls the visual demo state.

Before the user button is pressed, it runs a scanner sweep startup animation. After the button is pressed, it displays the pattern that matches the current firmware demo mode.

### FaultInjectTask

`FaultInjectTask` waits until the NUCLEO user button starts the demo. After that, it cycles through normal, high-temperature, low-voltage, and invalid-sensor demo modes.

---

## Button-Gated LED Fault Demo

At startup, the LED matrix runs a scanner sweep while the firmware waits for the NUCLEO user button.

After the button is pressed, the firmware cycles through these modes:

| LED state | Firmware mode | CAN payload effect | Decoder result |
|---|---|---|---|
| Check mark | `DEMO_MODE_NORMAL` | Normal battery, temperature, and status values | Clean telemetry |
| Exclamation mark | `DEMO_MODE_HIGH_TEMP` | `temperature_deciC = 950` | High-temperature fault |
| X symbol | `DEMO_MODE_LOW_VOLTAGE` | `battery_mV = 9500` | Low-voltage fault |
| X symbol | `DEMO_MODE_SENSOR_INVALID` | `status = 0x06` | Sensor-invalid fault |

The LED matrix and decoder are not directly connected. The firmware demo mode drives both the displayed LED pattern and the CAN values that the desktop decoder receives.

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
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/csv_frame_source.cpp src/waveshare_serial_frame_source.cpp src/fault_summary_writer.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
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

Validated 2,000-frame live result:

```text
Frames processed: 2000
Valid frames: 2000
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

Because the firmware sends four frame types per transmit cycle, the 2,000-frame test represents 500 telemetry cycles.

```text
2000 total frames / 4 frame types = 500 telemetry cycles
```

---

## Demo Media

The root project README includes the current screenshots and muted demo video links.

Relevant media files:

```text
media/freertos_can_hardware_wiring.png
media/live_decoder_summary.png
media/waveshare_receive.png
media/powered_led_fault_demo_setup.jpg
media/decoder_demo.mp4
media/waveshare_demo.mp4
media/led_fault_demo_side_by_side.mp4
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
MAX7219 startup scanner sweep appears
NUCLEO user button starts the LED fault demo
Waveshare receives 0x100, 0x101, 0x102, and 0x200
Desktop C++ live reader can decode the frames and report demo faults
```

---

## Current Limitations

```text
The firmware currently generates simulated telemetry values rather than real ADC sensor values.
The desktop live reader is implemented for the current Windows/Waveshare workflow.
SocketCAN can0 support is not implemented yet.
Production-grade serial reconnect/recovery behavior is future polish.
```

---

## Summary

This FreeRTOS firmware provides the live embedded CAN source for the C++ CAN Telemetry Decoder and Fault Analyzer.

The firmware side generates and transmits CAN traffic through the SN65HVD230 transceiver while also driving a MAX7219 LED matrix. The desktop side receives the CAN frames through the Waveshare USB-CAN serial backend and feeds them through the existing C++ validation, decoding, statistics, and fault-analysis pipeline.
