# USB-CAN Capture Notes

This document records the USB-CAN capture status for the STM32 CAN telemetry project.

---

## Capture Status

Current status:

```text
Hardware capture confirmed.
```

The STM32 NUCLEO successfully transmitted CAN traffic through the SN65HVD230 CAN transceiver to the Waveshare USB-CAN adapter.

The PC received the transmitted CAN frame using the Waveshare USB-CAN software/tool.

---

## Confirmed Hardware Frame

The first confirmed hardware transmit frame was:

```text
ID: 0x100
DLC: 8
Data: 00 08 10 00 FF 0A 07 01
```

Transmit period:

```text
100 ms
```

This confirms that the basic hardware path works.

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

---

## USB-CAN Receive Settings

Working receive setup:

```text
Adapter: Waveshare USB-CAN
Mode: Normal
Frame type: Standard CAN
CAN format: Classic CAN
Bitrate: 500 kbps
Receive: enabled
Received ID: 0x100
```

---

## Received Frame Meaning

Payload:

```text
00 08 10 00 FF 0A 07 01
```

Decoded meaning:

```text
AIN1_RAW = 2048
AIN2_RAW = 16
AIN3_RAW = 2815
Status = 0x07
Counter = 1
```

Status byte:

```text
0x07 = 0000 0111
```

Meaning:

```text
Sensor 1 valid
Sensor 2 valid
Sensor 3 valid
```

---

## Decoder Integration Note

The hardware currently confirms transmission of CAN ID `0x100`.

For full decoder integration testing, `data/sample_can_log.csv` includes captured-style `0x100` frame data plus additional test frames for:

```text
0x101 battery and temperature decoding
0x102 status frame decoding
unknown ID fault testing
invalid DLC fault testing
```

This allows the desktop decoder to test the full validation, decoding, fault analysis, and statistics pipeline.

---

## Debug Result

The hardware CAN bridge worked after checking the core CAN requirements:

```text
matching bitrate
CANH connected to CANH
CANL connected to CANL
common ground
3.3 V transceiver power
termination across CANH/CANL
Waveshare receive mode
```

---

## Current Status Summary

```text
Hardware CAN transmit path: working
Waveshare USB-CAN receive: working
Desktop decoder log integration: using sample_can_log.csv
Next step: feed captured or captured-style frames into the C++ decoder
```