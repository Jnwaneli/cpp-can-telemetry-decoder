# STM32 FDCAN Configuration Plan

This document records the planned STM32 FDCAN transmit configuration for the CAN telemetry project.

---

## Goal

The STM32 NUCLEO-G431RB will transmit a simulated telemetry frame over CAN.

First transmitted frame:

```text
CAN ID: 0x100
DLC: 8
Data: 00 08 10 00 FF 0A 07 01