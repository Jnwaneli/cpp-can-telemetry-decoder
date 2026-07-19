# CAN Wiring Plan

Status: Planned, not fully wired yet.  
Current week: Week 5  
Current task: CAN wiring plan and hardware bridge setup.

This document describes the planned CAN hardware chain for the STM32 CAN telemetry project.

---

## Hardware Chain

```text
NUCLEO-G431RB FDCAN TX/RX
        ↓
SN65HVD230 TXD/RXD
        ↓
SN65HVD230 CANH/CANL
        ↓
CANH/CANL bus
        ↓
Waveshare USB-CAN
        ↓
PC

## Power Safety Note

The SN65HVD230 module should be powered from 3.3 V.

Do not connect the SN65HVD230 VCC pin to 5 V unless the exact module documentation confirms that the module supports 5 V input.

For this project, the planned connection is:

```text
NUCLEO 3.3 V -> SN65HVD230 VCC
NUCLEO GND   -> SN65HVD230 GND