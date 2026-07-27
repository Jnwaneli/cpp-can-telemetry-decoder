# FreeRTOS Live CAN Capture Test

## Purpose

This document records the FreeRTOS live CAN capture and decoder integration test.

The goal is to verify that the STM32 FreeRTOS firmware sends CAN frames that match the desktop C++ decoder protocol and can be processed through both CSV and live-input paths.

---

## Firmware Used

The firmware project is:

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
PC receive software / desktop C++ live reader
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
SignalGeneratorTask generates simulated live telemetry.
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

The sender transmits a burst of four frames about every 100 ms. Because each transmit cycle sends four frames, the receive tool shows about 40 rows per second.

---

## Captured CSV Log

A short proof capture is saved as:

```text
data/freertos_captured_log.csv
```

The file can be processed with:

```powershell
.\main.exe --csv data\freertos_captured_log.csv
```

---

## Direct Live Decoder Test

The desktop C++ decoder can also read the Waveshare USB-CAN adapter directly through the Windows COM port.

```powershell
.\main.exe --waveshare-serial COM4 2000
```

Replace `COM4` with the actual COM port assigned to the Waveshare adapter.

Validated result:

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

## Generated Outputs

The C++ decoder writes:

```text
output/fault_summary.json
```

The diagnostic report agent then writes:

```text
output/diagnostic_report.md
```

---

## Result

```text
FreeRTOS CAN output matches the desktop decoder protocol.
The Waveshare adapter receives all four expected CAN IDs.
The captured CSV file can be processed by the C++ decoder.
The desktop C++ app can directly read live Waveshare USB-CAN traffic on Windows.
The structured JSON output and diagnostic report generation are working.
```
