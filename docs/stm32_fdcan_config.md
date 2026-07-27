# STM32 FDCAN Configuration

This document records the tested STM32 FDCAN transmit configuration for the CAN telemetry project.

---

## Goal

The STM32 NUCLEO-G431RB transmits simulated telemetry over Classic CAN through FDCAN1, an SN65HVD230 CAN transceiver, and a Waveshare USB-CAN adapter.

The desktop C++ decoder can receive those frames directly through the Waveshare serial backend or process equivalent CSV logs.

---

## Hardware

```text
Board: NUCLEO-G431RB
Transceiver: SN65HVD230
USB-CAN adapter: Waveshare USB-CAN
PC input path: Windows COM port
```

---

## FDCAN Settings

```text
Peripheral: FDCAN1
Mode: Normal
Frame format: Classic CAN
ID type: Standard 11-bit ID
DLC: 8 bytes
Bitrate: 500 kbps
TX mode: Tx FIFO/Queue
```

The Waveshare USB-CAN adapter must also be set to:

```text
500 kbps
```

---

## FreeRTOS Configuration

```text
FreeRTOS interface: CMSIS_V2
HAL timebase source: TIM6
USE_NEWLIB_REENTRANT: Enabled
```

The HAL timebase uses TIM6 so FreeRTOS can own SysTick for scheduler timing.

---

## Tasks

| Task | Priority | Stack | Purpose |
|---|---:|---:|---|
| `SignalGeneratorTask` | `osPriorityNormal` | 128 words | Generates simulated live telemetry samples |
| `ProcessingTask` | `osPriorityNormal` | 128 words | Receives samples and updates shared telemetry |
| `CanTxTask` | `osPriorityAboveNormal` | 256 words | Packages and transmits CAN frames |
| `StatusLedTask` | `osPriorityLow` | 128 words | Blinks heartbeat LED |

---

## FreeRTOS Data Flow

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
STM32 FDCAN1
        ↓
SN65HVD230
        ↓
Waveshare USB-CAN
```

---

## CAN Frames Transmitted

The firmware transmits four standard Classic CAN frame types repeatedly:

```text
0x100 Analog Inputs
0x101 Battery and Temperature
0x102 Status Flags
0x200 Vehicle Telemetry
```

All four frames use DLC 8.

---

## Counter Ownership

`CanTxTask` owns the CAN transmit counter.

Reason:

```text
SignalGeneratorTask runs faster than CanTxTask.
If the generator owned the counter, transmitted counters could appear to jump.
The desktop decoder would then correctly report dropped-frame faults.
```

The transmit counter increments only when the CAN transmit cycle runs.

---

## Verified Result

The STM32 FreeRTOS firmware successfully transmitted all four supported CAN IDs through the SN65HVD230 and Waveshare USB-CAN adapter.

The desktop C++ decoder successfully processed a 2,000-frame live run with:

```text
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```
