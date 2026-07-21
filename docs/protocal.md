# CAN Protocol

This document describes the simulated CAN telemetry protocol used by the C++ CAN Telemetry Decoder and Fault Analyzer.

---

## Overview

The project processes standard CAN frames with 8-byte payloads.

The decoder currently recognizes these CAN IDs:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

Any frame outside this supported ID set is treated as an unknown ID fault.

---

## General Frame Rules

Current frame rules:

```text
ID format: Standard CAN ID
Expected DLC: 8 bytes
Payload size: 8 bytes
Byte order for 16-bit values: little-endian
```

Little-endian means the low byte comes first.

Example:

```text
Bytes: 00 08
Value: 0x0800 = 2048
```

---

## CAN ID 0x100 — Analog Inputs

CAN ID `0x100` carries three analog input readings, a status byte, and a counter.

Payload format:

```text
Byte 0: AIN1 low byte
Byte 1: AIN1 high byte
Byte 2: AIN2 low byte
Byte 3: AIN2 high byte
Byte 4: AIN3 low byte
Byte 5: AIN3 high byte
Byte 6: status byte
Byte 7: counter
```

Example payload:

```text
00 08 10 00 FF 0A 07 01
```

Decoded values:

```text
AIN1_RAW = 2048
AIN2_RAW = 16
AIN3_RAW = 2815
Status = 0x07
Counter = 1
```

---

## 0x100 Status Byte

The `0x100` status byte contains sensor-valid flags.

Masks:

```text
SENSOR1_VALID_MASK = 0x01
SENSOR2_VALID_MASK = 0x02
SENSOR3_VALID_MASK = 0x04
ERROR_FLAG_MASK    = 0x80
```

Meaning:

```text
bit 0 = sensor 1 valid
bit 1 = sensor 2 valid
bit 2 = sensor 3 valid
bit 7 = error flag
```

Example:

```text
0x07 = 0000 0111
```

This means:

```text
Sensor 1 valid
Sensor 2 valid
Sensor 3 valid
Error flag not set
```

---

## CAN ID 0x101 — Battery and Temperature

CAN ID `0x101` carries battery voltage and temperature.

Payload format:

```text
Byte 0: battery_mV low byte
Byte 1: battery_mV high byte
Byte 2: temperature_deciC low byte
Byte 3: temperature_deciC high byte
Byte 4-7: reserved
```

Example payload:

```text
38 31 59 01 00 00 00 00
```

Decoded values:

```text
battery_mV = 12600
battery_V = 12.60 V

temperature_deciC = 345
temperature_C = 34.5 C
```

Scaling:

```text
battery_V = battery_mV / 1000.0
temperature_C = temperature_deciC / 10.0
```

---

## CAN ID 0x102 — Status Flags

CAN ID `0x102` carries system status information.

Payload format:

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4-7: reserved
```

Example payload:

```text
07 00 01 00 00 00 00 00
```

Decoded values:

```text
Sensor1 Valid: YES
Sensor2 Valid: YES
Sensor3 Valid: YES
System Fault Byte: 0x00
Mode: 1
Error Code: 0
```

---

## CAN ID 0x200 — Vehicle Telemetry

CAN ID `0x200` is reserved for future vehicle telemetry expansion.

Planned use:

```text
speed
RPM
wheel data
vehicle state
```

The ID is recognized by validation, but full decoding is not implemented yet.

---

## Fault Rules Summary

The main documented fault rules are:

```text
Unknown ID:
Any frame not in {0x100, 0x101, 0x102, 0x200}

Invalid DLC:
Any supported frame where DLC != 8

Sensor invalid:
Status flag bit not set

Voltage low:
battery < 10.5 V

Voltage high:
battery > 14.8 V

Temperature high:
temperature > 80 C
```

Full fault rule details are documented in:

```text
docs/fault_rules.md
```