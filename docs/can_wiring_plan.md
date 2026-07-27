# CAN Wiring Plan

Status: wired, tested, photographed, and documented.

This document describes the tested CAN hardware chain for the STM32 CAN telemetry project.

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
```

The STM32 FDCAN pins do not connect directly to CANH and CANL. The SN65HVD230 transceiver converts STM32 logic-level CAN signals into the physical differential CAN bus signals.

---

## Tested Wiring

```text
PA12 / FDCAN1_TX -> SN65HVD230 TXD
PA11 / FDCAN1_RX <- SN65HVD230 RXD
NUCLEO 3.3 V     -> SN65HVD230 VCC
NUCLEO GND       -> SN65HVD230 GND
SN65HVD230 CANH  -> Waveshare CANH
SN65HVD230 CANL  -> Waveshare CANL
SN65HVD230 GND   -> Waveshare GND
```

---

## Power Safety Note

The SN65HVD230 module is powered from 3.3 V.

```text
NUCLEO 3.3 V -> SN65HVD230 VCC
NUCLEO GND   -> SN65HVD230 GND
```

Do not connect the SN65HVD230 VCC pin to 5 V unless the exact module documentation confirms that the module supports 5 V input.

---

## CAN Bus Settings

```text
CAN mode: Classic CAN
CAN ID type: Standard 11-bit ID
Bitrate: 500 kbps
DLC: 8 bytes
```

Both the STM32 FDCAN peripheral and the Waveshare USB-CAN adapter must use the same CAN bitrate.

---

## Termination Check

With power off, CANH-to-CANL measured about:

```text
60 ohms
```

This indicates two active 120 ohm CAN terminations in parallel.

---

## Verified Result

The wiring successfully supports the full live CAN path:

```text
STM32 FreeRTOS telemetry sender
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
WaveshareSerialFrameSource
        ↓
Desktop C++ decoder
```

The desktop decoder processed 2,000 live STM32-generated CAN frames with zero unknown IDs, zero invalid DLC errors, zero dropped-frame faults, and zero diagnostic warnings.
