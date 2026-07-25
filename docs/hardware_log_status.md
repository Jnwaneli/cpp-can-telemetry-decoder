# Hardware and Log Status

This document records the current hardware, firmware, and captured-log status for the C++ CAN Telemetry Decoder and Fault Analyzer project.

---

## Current Status Summary

```text
Desktop C++ decoder: Working with CSV/log input
STM32 simple CAN sender: Removed/archived from repo
STM32 FreeRTOS CAN sender: Working
SN65HVD230 CAN transceiver path: Tested
Waveshare USB-CAN receive workflow: Tested
FreeRTOS captured log file: Created
Direct live USB-CAN reader in C++ app: Planned
```

---

## Desktop Decoder Status

The desktop C++ decoder currently reads CAN frames from CSV/log files.

Current supported input file:

```text
data/sample_can_log.csv
```

Additional test files:

```text
data/stuck_sensor_test.csv
data/freertos_captured_log.csv
```

The decoder supports:

```text
0x100 Analog Inputs
0x101 Battery and Temperature
0x102 Status Flags
0x200 Vehicle Telemetry
```

The decoder validates:

```text
Known CAN IDs
DLC values
Little-endian payload fields
Status flags
Dropped counters
Possible stuck sensor values
Voltage and temperature thresholds
```

---

## STM32 FreeRTOS Hardware Sender Status

The STM32 FreeRTOS CAN sender is implemented in:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

This firmware is the current active embedded hardware sender for the project.

The old simple non-FreeRTOS sender was removed to keep the repo focused and professional.

---

## Hardware Chain Tested

The tested hardware chain is:

```text
NUCLEO-G431RB
        ↓ FDCAN TX/RX
SN65HVD230 CAN transceiver
        ↓ CANH/CANL
Waveshare USB-CAN adapter
        ↓ USB
PC receive software
```

The STM32 does not connect directly to CANH/CANL.

The STM32 FDCAN TX/RX pins connect to the SN65HVD230 transceiver.

The SN65HVD230 converts STM32 logic-level CAN signals into physical CANH/CANL bus signals.

---

## Wiring Status

Hardware wiring has been completed and tested.

Important wiring checks:

```text
NUCLEO FDCAN_TX -> SN65HVD230 TXD
NUCLEO FDCAN_RX <- SN65HVD230 RXD
NUCLEO 3.3 V    -> SN65HVD230 VCC
NUCLEO GND      -> SN65HVD230 GND
SN65HVD230 CANH -> Waveshare CANH
SN65HVD230 CANL -> Waveshare CANL
Common GND shared between devices
```

Termination check:

```text
CANH to CANL measured about 60 ohms with power off
```

This indicates two 120 ohm terminations are active on the CAN bus.

---

## STM32CubeIDE / CubeMX Status

The FreeRTOS firmware was configured in STM32CubeIDE/CubeMX.

Important settings:

```text
Board: NUCLEO-G431RB
Peripheral: FDCAN1
CAN mode: Normal
Frame format: Classic CAN
ID type: Standard ID
DLC: 8 bytes
Bitrate: 500 kbps
TX mode: Tx FIFO/Queue
FreeRTOS interface: CMSIS_V2
HAL timebase source: TIM6
USE_NEWLIB_REENTRANT: Enabled
```

The Waveshare USB-CAN software was also set to:

```text
500 kbps
```

Both STM32 and Waveshare must use the same CAN bitrate.

---

## FreeRTOS Task Status

The FreeRTOS firmware uses four tasks:

| Task | Status | Purpose |
|---|---|---|
| `SignalGeneratorTask` | Working | Generates fake live telemetry values |
| `ProcessingTask` | Working | Receives samples and updates shared telemetry |
| `CanTxTask` | Working | Packages and transmits CAN frames |
| `StatusLedTask` | Working | Blinks heartbeat LED |

Current architecture:

```text
SignalGeneratorTask
        ↓ SensorSample
sensorQueueHandle
        ↓
ProcessingTask
        ↓ mutex-protected latestTelemetry
CanTxTask
        ↓
STM32 FDCAN transmit
```

FreeRTOS objects:

```text
sensorQueueHandle
telemetryMutexHandle
latestTelemetry
```

---

## CAN Frames Transmitted by STM32

The STM32 FreeRTOS sender transmits all four supported CAN IDs:

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
Bitrate = 500 kbps
```

The Waveshare USB-CAN receive tool confirmed all four IDs repeatedly.

---

## Important Firmware Fixes

### Missing 0x200 Frame

At first, the Waveshare tool showed:

```text
0x100
0x101
0x102
```

but not:

```text
0x200
```

Likely cause:

```text
The FDCAN TX FIFO filled after 0x100, 0x101, and 0x102 were queued.
The 0x200 frame was skipped.
```

Fix:

```text
Wait for TX FIFO space before sending.
Add small delays between frame sends.
```

After the fix, Waveshare received:

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

If the generator counter were used as the CAN frame counter, transmitted frames could appear to jump by 10.

That would make the desktop decoder falsely report dropped frames.

Current decision:

```text
SignalGeneratorTask may use a fake internal sample counter.
CanTxTask owns the CAN transmit counter.
```

---

## Captured FreeRTOS Log Status

A captured FreeRTOS log file has been created:

```text
data/freertos_captured_log.csv
```

Short proof version:

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

This log proves that the STM32 FreeRTOS sender can generate all four CAN frame types and that the captured frames can be converted into the desktop decoder’s CSV input format.

---

## C++ Decoder Integration Status

The desktop decoder can process CSV logs.

Current workflow:

```text
STM32 FreeRTOS firmware
        ↓
Waveshare USB-CAN capture
        ↓
data/freertos_captured_log.csv
        ↓
Desktop C++ decoder
```

To test the FreeRTOS capture with the current decoder:

```powershell
Copy-Item data\sample_can_log.csv data\sample_can_log_backup.csv
Copy-Item data\freertos_captured_log.csv data\sample_can_log.csv
.\main.exe
Copy-Item data\sample_can_log_backup.csv data\sample_can_log.csv
```

Expected decoder behavior:

```text
0x100 decodes as Analog Inputs
0x101 decodes as Battery and Temperature
0x102 decodes as Status Flags
0x200 decodes as Vehicle Telemetry
Dropped frames should be 0 if counters are sequential
```

---

## Implemented

```text
Desktop CSV/log decoder
STM32 FreeRTOS CAN sender
SN65HVD230 physical CAN bridge
Waveshare USB-CAN receive workflow
Multi-frame STM32 CAN transmission
Captured FreeRTOS CSV log
Queue and mutex FreeRTOS architecture
Transmit counter owned by CanTxTask
```

---

## In Progress

```text
Final README polish
Final documentation cleanup
Testing freertos_captured_log.csv through the C++ decoder
Cleaning build artifacts from repo
```

---

## Planned Future Work

```text
Direct live USB-CAN reader inside the C++ app
SocketCAN/can0 Linux workflow
candump-style log parser
fault_summary.json output
AI-assisted diagnostic report generator
Real ADC or sensor-based telemetry instead of simulated values
Optional FreeRTOS FaultInjectTask
```

---

## Final Hardware Status

The hardware bridge is working.

The project now has a verified embedded-to-PC CAN workflow:

```text
STM32 FreeRTOS telemetry sender
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
Captured CSV log
        ↓
C++ CAN Telemetry Decoder and Fault Analyzer
```

Current limitation:

```text
The desktop C++ app still reads captured logs, not live USB-CAN traffic directly.
```