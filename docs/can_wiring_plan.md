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