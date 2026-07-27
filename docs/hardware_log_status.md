# Hardware and Log Status

This document records the current hardware, firmware, live-ingestion, and generated-output status for the C++ CAN Telemetry Decoder and Fault Analyzer project.

---

## Current Status Summary

```text
Desktop C++ decoder: working with CSV and live Waveshare input
STM32 FreeRTOS CAN sender: working
SN65HVD230 CAN transceiver path: tested
Waveshare USB-CAN receive workflow: tested
Direct live USB-CAN reader in C++ app: working on Windows
Structured JSON output: working
Diagnostic report agent: working after JSON generation
FreeRTOS captured log file: available
```

---

## Desktop Decoder Status

The desktop C++ decoder supports two input paths:

```text
CSV/log input through CsvFrameSource
Live Waveshare USB-CAN serial input through WaveshareSerialFrameSource
```

Useful test files:

```text
data/sample_can_log.csv
data/good_frames.csv
data/fault_frames.csv
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

The decoder validates and analyzes:

```text
Known CAN IDs
DLC values
Little-endian payload fields
Status flags
Dropped counters
Possible stuck sensor values
Voltage and temperature thresholds
```

After a normal decoder run, the program writes:

```text
output/fault_summary.json
```

The diagnostic report agent converts that JSON summary into:

```text
output/diagnostic_report.md
```

---

## STM32 FreeRTOS Hardware Sender Status

The STM32 FreeRTOS CAN sender is implemented in:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

This firmware is the active embedded hardware sender for the project.

The FreeRTOS firmware generates simulated telemetry values, passes them through a queue and mutex-protected shared state, and transmits CAN frames through FDCAN1.

---

## Hardware Chain Tested

```text
NUCLEO-G431RB
        ↓ FDCAN TX/RX
SN65HVD230 CAN transceiver
        ↓ CANH/CANL
Waveshare USB-CAN adapter
        ↓ USB
PC receive software / PC C++ live reader
```

The STM32 does not connect directly to CANH/CANL. The STM32 FDCAN TX/RX pins connect to the SN65HVD230 transceiver, and the transceiver converts STM32 logic-level CAN signals into physical CANH/CANL bus signals.

---

## Wiring Status

Hardware wiring has been completed, photographed, documented, and tested.

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

The Waveshare USB-CAN software also uses:

```text
500 kbps
```

Both STM32 and Waveshare must use the same CAN bitrate.

---

## FreeRTOS Task Status

The FreeRTOS firmware uses four tasks:

| Task | Status | Purpose |
|---|---|---|
| `SignalGeneratorTask` | Working | Generates simulated live telemetry values |
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

At first, the Waveshare tool showed `0x100`, `0x101`, and `0x102`, but not `0x200`.

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

After the fix, Waveshare received all four IDs repeatedly.

### Counter Ownership

The CAN transmit counter is owned by `CanTxTask`.

Reason:

```text
SignalGeneratorTask runs every 10 ms.
CanTxTask transmits about every 100 ms.
```

If the generator counter were used as the CAN frame counter, transmitted frames could appear to jump. That would make the desktop decoder falsely report dropped frames.

Current decision:

```text
SignalGeneratorTask may use a fake internal sample counter.
CanTxTask owns the CAN transmit counter.
```

---

## Captured FreeRTOS Log Status

A captured FreeRTOS log file is available:

```text
data/freertos_captured_log.csv
```

This log proves that the STM32 FreeRTOS sender can generate all four CAN frame types and that captured frames can be processed by the desktop decoder's CSV input path.

---

## Live C++ Decoder Integration Status

The desktop C++ application can read live Waveshare USB-CAN traffic directly through:

```text
WaveshareSerialFrameSource
```

The live test command is:

```powershell
.\main.exe --waveshare-serial COM4 2000
```

The validated live result was:

```text
Frames processed: 2000
Valid frames: 2000
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

---

## Implemented

```text
Desktop CSV/log decoder
Desktop live Waveshare serial decoder path
STM32 FreeRTOS CAN sender
SN65HVD230 physical CAN bridge
Waveshare USB-CAN receive workflow
Multi-frame STM32 CAN transmission
Captured FreeRTOS CSV log
Queue and mutex FreeRTOS architecture
Transmit counter owned by CanTxTask
Structured fault_summary.json output
Diagnostic report agent
```

---

## Future Work

```text
Linux SocketCAN/can0 workflow
candump-style log parser
Real ADC or sensor-based telemetry instead of simulated values
Optional FreeRTOS FaultInjectTask
Production-grade serial reconnect/error recovery
```

---

## Final Hardware Status

The hardware bridge is working.

The project has a verified embedded-to-PC CAN workflow:

```text
STM32 FreeRTOS telemetry sender
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
Desktop C++ decoder
        ↓
output/fault_summary.json
        ↓
Diagnostic report agent
        ↓
output/diagnostic_report.md
```
