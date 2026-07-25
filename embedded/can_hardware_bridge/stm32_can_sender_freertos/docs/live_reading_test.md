# FreeRTOS Live Reading Test

This document records the FreeRTOS live CAN capture test.

This is a live-capture workflow, not a direct live USB-CAN reader inside the C++ application.

---

## Test Purpose

The goal was to verify that the STM32 FreeRTOS CAN sender transmits frames that match the desktop C++ decoder protocol.

The test path was:

```text
STM32 FreeRTOS firmware
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
PC receive software
        ↓
CSV capture
        ↓
C++ decoder
```

---

## Firmware Used

Firmware path:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

Main firmware file:

```text
Core/Src/main.c
```

FreeRTOS interface:

```text
CMSIS_V2
```

---

## Hardware Used

```text
NUCLEO-G431RB
SN65HVD230 CAN transceiver
Waveshare USB-CAN adapter
CANH/CANL wiring
Common ground
120 ohm termination resistors
```

Measured CANH-to-CANL resistance with power off:

```text
about 60 ohms
```

This indicates two 120 ohm terminations on the CAN bus.

---

## CAN Settings

```text
Bitrate: 500 kbps
Frame type: Standard Classic CAN
DLC: 8 bytes
```

Both the STM32 FDCAN peripheral and the Waveshare USB-CAN adapter used:

```text
500 kbps
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
SignalGeneratorTask generates fake telemetry.
ProcessingTask receives samples from the queue.
ProcessingTask updates latestTelemetry using a mutex.
CanTxTask reads latestTelemetry and sends CAN frames.
StatusLedTask toggles the LED heartbeat.
```

---

## Frames Sent

The STM32 FreeRTOS sender transmitted:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

All frames used:

```text
Standard CAN ID
Classic CAN
DLC = 8
Little-endian signal packing
```

---

## Waveshare Receive Result

The Waveshare USB-CAN receive tool showed all four IDs repeatedly:

```text
0x100
0x101
0x102
0x200
```

The sender sends four frames per transmit cycle.

The transmit cycle is about every:

```text
100 ms
```

So the Waveshare receive window may show about:

```text
40 rows per second
```

---

## Captured Log

A short capture was saved as:

```text
data/freertos_captured_log.csv
```

Example:

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

To test the FreeRTOS capture:

```powershell
Copy-Item data\sample_can_log.csv data\sample_can_log_backup.csv
Copy-Item data\freertos_captured_log.csv data\sample_can_log.csv
.\main.exe
Copy-Item data\sample_can_log_backup.csv data\sample_can_log.csv
```

---

## Expected Decoder Result

Expected frame behavior:

```text
0x100 decodes as Analog Inputs
0x101 decodes as Battery and Temperature
0x102 decodes as Status Flags
0x200 decodes as Vehicle Telemetry
```

Expected fault behavior:

```text
Unknown IDs: 0
Invalid DLC: 0
Dropped frames: 0
Faults: 0
Warnings: 0
```

---

## Result Summary

```text
The STM32 FreeRTOS sender transmitted all four expected CAN IDs.
The Waveshare adapter received all four CAN IDs.
The captured CSV matched the desktop decoder protocol.
The C++ decoder can process the captured hardware-generated frames.
```

---

## Current Limitation

This is not direct live reading inside the C++ application.

Current workflow:

```text
Live STM32 CAN transmission
        ↓
Waveshare live receive
        ↓
Saved CSV capture
        ↓
C++ decoder reads CSV
```

Planned future workflow:

```text
Live STM32 CAN transmission
        ↓
Waveshare or SocketCAN interface
        ↓
C++ decoder reads frames directly
```