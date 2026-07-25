# FreeRTOS Live CAN Capture Test

## Purpose

This document records the Week 8 Day 2 FreeRTOS live CAN capture test.

The goal was to verify that the STM32 FreeRTOS firmware sends CAN frames that match the desktop C++ decoder protocol.

---

## Firmware Used

The firmware project used was:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

The FreeRTOS task code is implemented in:

```text
Core/Src/main.c
```

---

## Hardware Setup

Hardware chain:

```text
NUCLEO-G431RB
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
PC receive software
```

CAN settings:

```text
Bitrate: 500 kbps
Frame type: Standard Classic CAN
DLC: 8 bytes
```

---

## FreeRTOS Tasks Running

```text
SignalGeneratorTask
ProcessingTask
CanTxTask
StatusLedTask
```

Task behavior:

```text
SignalGeneratorTask generates fake live telemetry.
ProcessingTask receives SensorSample messages from the queue.
ProcessingTask writes latestTelemetry using a mutex.
CanTxTask reads latestTelemetry and sends CAN frames.
StatusLedTask blinks the board LED as a heartbeat.
```

---

## FreeRTOS Objects Used

```text
sensorQueueHandle
telemetryMutexHandle
latestTelemetry
```

---

## CAN Frames Sent

The STM32 FreeRTOS sender transmits:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

All frames use:

```text
Standard CAN ID
Classic CAN
DLC = 8
Little-endian 16-bit packing
```

---

## Waveshare Receive Result

The Waveshare USB-CAN tool received all four CAN IDs repeatedly:

```text
0x100
0x101
0x102
0x200
```

The sender transmits a burst of four frames about every 100 ms.

Because each transmit cycle sends four frames, the receive tool shows about 40 rows per second.

---

## Captured CSV Log

A short proof capture was saved as:

```text
data/freertos_captured_log.csv
```

Example captured frames:

```csv
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
100,8,7A,0D,DE,0D,42,0E,07,4B
101,8,48,31,5F,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,80,02,0A,0F,03,29,00,4B
100,8,B0,04,14,05,78,05,07,4C
101,8,3F,31,60,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,96,02,1D,10,03,34,00,4C
```

---

## Decoder Test Method

The desktop decoder currently reads:

```text
data/sample_can_log.csv
```

To test the FreeRTOS capture, the captured file was temporarily copied over the sample log:

```powershell
Copy-Item data\sample_can_log.csv data\sample_can_log_backup.csv
Copy-Item data\freertos_captured_log.csv data\sample_can_log.csv
.\main.exe
Copy-Item data\sample_can_log_backup.csv data\sample_can_log.csv
```

---

## Expected Decoder Output

Expected frame results:

```text
0x100 decodes as Analog Inputs
0x101 decodes as Battery and Temperature
0x102 decodes as Status Flags
0x200 decodes as Vehicle Telemetry
```

Expected fault results:

```text
Unknown IDs: 0
Invalid DLC: 0
Dropped frames: 0
Faults: 0
Warnings: 0
```

---

## Result

```text
FreeRTOS CAN output matched the desktop decoder protocol.
The Waveshare adapter received all four expected CAN IDs.
The captured CSV file can be processed by the C++ decoder.
Direct live USB-CAN reading inside the C++ app is still planned future work.
```

---

## Current Limitation

This test proves the live-capture workflow:

```text
STM32 FreeRTOS live CAN
        ↓
Waveshare receive
        ↓
CSV capture
        ↓
C++ decoder
```

It does not yet implement direct live USB-CAN reading inside the C++ application.