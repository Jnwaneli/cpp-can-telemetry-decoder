# STM32 CAN Sender FreeRTOS Variant

This folder contains the FreeRTOS variant of the STM32 CAN telemetry sender.

## Current Status

```text
Status: In Progress
```

## Week 7 Day 1 Status

```text
Duplicated from the working stm32_can_sender project.
CAN settings unchanged.
FDCAN pins unchanged.
FreeRTOS not enabled yet.
Build status: to be confirmed.
```

## Goal

This project will become an STM32 FreeRTOS CAN telemetry sender that transmits simulated live telemetry frames to the PC-side C++ decoder.

## Planned CAN IDs

```text
0x100 = Analog / ADC values
0x101 = Battery voltage / temperature
0x102 = Status flags
0x200 = Vehicle telemetry
```

## Planned FreeRTOS Architecture

```text
SignalGeneratorTask
        ↓ SensorSample queue
ProcessingTask
        ↓ mutex-protected latestTelemetry
CanTxTask
        ↓ FDCAN TX
SN65HVD230
        ↓
Waveshare USB-CAN
        ↓
PC CAN log / C++ decoder
```

## Day 1 Rule

```text
Do not change working FDCAN settings yet.
Do not change working CAN pins yet.
Do not enable FreeRTOS yet.
First confirm the duplicated project builds.
```