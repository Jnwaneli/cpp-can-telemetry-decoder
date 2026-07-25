# FreeRTOS Task Table

This document summarizes the FreeRTOS tasks used by the STM32 CAN telemetry sender.

---

## Task Summary

| Task | Priority | Stack Size | Period / Blocking Behavior | Main Responsibility |
|---|---:|---:|---|---|
| `SignalGeneratorTask` | `osPriorityNormal` | 128 words | Runs about every 10 ms | Generates fake live telemetry samples |
| `ProcessingTask` | `osPriorityNormal` | 128 words | Blocks on `sensorQueueHandle` | Copies `SensorSample` data into `latestTelemetry` |
| `CanTxTask` | `osPriorityAboveNormal` | 256 words | Runs about every 100 ms | Sends CAN frames through STM32 FDCAN |
| `StatusLedTask` | `osPriorityLow` | 128 words | Runs about every 500 ms | Toggles the board heartbeat LED |

---

## SignalGeneratorTask

Purpose:

```text
Generate fake live telemetry values.
```

Output:

```text
SensorSample
```

Destination:

```text
sensorQueueHandle
```

Example generated fields:

```text
AIN1 raw
AIN2 raw
AIN3 raw
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

This task does not transmit CAN frames.

---

## ProcessingTask

Purpose:

```text
Receive generated telemetry samples and update the shared latestTelemetry structure.
```

Input:

```text
sensorQueueHandle
```

Protected output:

```text
latestTelemetry
```

Synchronization object:

```text
telemetryMutexHandle
```

Why this matters:

```text
ProcessingTask writes latestTelemetry.
CanTxTask reads latestTelemetry.
The mutex prevents CanTxTask from reading partially updated data.
```

---

## CanTxTask

Purpose:

```text
Transmit CAN frames using the latest processed telemetry.
```

Input:

```text
latestTelemetry
```

Synchronization object:

```text
telemetryMutexHandle
```

CAN frames sent:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

This task owns the CAN transmit counter.

Reason:

```text
The generator runs faster than the transmitter.
If the generator owned the CAN counter, transmitted frames could appear to skip counter values.
```

---

## StatusLedTask

Purpose:

```text
Blink the board LED to show that the scheduler is running.
```

LED function:

```c
BSP_LED_Toggle(LED_GREEN);
```

Period:

```text
500 ms
```

---

## Priority Reasoning

`CanTxTask` uses a slightly higher priority because CAN transmission timing is more important than the heartbeat LED.

`StatusLedTask` uses low priority because blinking an LED is not timing-critical.

`SignalGeneratorTask` and `ProcessingTask` use normal priority because they form the basic telemetry generation and processing pipeline.