# Queue and Mutex Notes

This document explains how the STM32 FreeRTOS CAN telemetry sender uses a queue and mutex.

---

## FreeRTOS Objects Used

```text
sensorQueueHandle
telemetryMutexHandle
latestTelemetry
```

---

## Why a Queue Is Used

The queue connects:

```text
SignalGeneratorTask
        ↓
ProcessingTask
```

`SignalGeneratorTask` creates fake telemetry samples.

`ProcessingTask` consumes those samples.

The queue allows the generator and processor to run independently.

---

## Queue Data Type

The queue stores:

```text
SensorSample
```

A `SensorSample` contains generated telemetry fields such as:

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

---

## Queue Behavior

`SignalGeneratorTask` uses the queue to send generated samples.

`ProcessingTask` waits on the queue.

This means `ProcessingTask` does not waste CPU time constantly checking for data.

It blocks until a sample is available.

---

## Why a Mutex Is Used

The mutex protects:

```text
latestTelemetry
```

`ProcessingTask` writes to `latestTelemetry`.

`CanTxTask` reads from `latestTelemetry`.

Without a mutex, `CanTxTask` could read the structure while `ProcessingTask` is halfway through updating it.

That could create mixed data, such as:

```text
new AIN1 value
old battery value
old speed value
new status value
```

The mutex prevents this race condition.

---

## Correct Shared Data Pattern

The project uses this pattern:

```text
ProcessingTask:
    acquire mutex
    write latestTelemetry
    release mutex

CanTxTask:
    acquire mutex
    copy latestTelemetry into local variable
    increment transmit counter
    release mutex
    send CAN frames using local copy
```

---

## Why CanTxTask Copies Data Locally

`CanTxTask` sends multiple CAN frames:

```text
0x100
0x101
0x102
0x200
```

It should not hold the mutex while sending all of those frames.

So it copies `latestTelemetry` into a local variable first.

Then it releases the mutex.

Then it sends CAN frames using the local copy.

This keeps the shared data protected without blocking `ProcessingTask` longer than necessary.

---

## Counter Ownership

The CAN transmit counter is owned by:

```text
CanTxTask
```

Reason:

```text
SignalGeneratorTask runs every 10 ms.
CanTxTask transmits about every 100 ms.
```

If `SignalGeneratorTask` controlled the CAN counter, the counter might jump like:

```text
0, 10, 20, 30
```

The desktop decoder would interpret that as dropped frames.

By letting `CanTxTask` own the counter, the transmitted sequence stays consistent:

```text
0, 1, 2, 3
```

---

## Summary

```text
Queue:
Moves SensorSample data from generator to processor.

Mutex:
Protects latestTelemetry between processor and transmitter.

Local copy:
Lets CanTxTask release the mutex before sending CAN frames.

Counter ownership:
Prevents false dropped-frame detection in the desktop decoder.
```