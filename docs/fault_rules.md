# Fault Rules

This document defines the current fault rules used by the C++ CAN Telemetry Decoder and Fault Analyzer.

The project decodes simulated CAN frames and checks for validation faults, sensor status faults, battery voltage faults, and temperature faults.

---

## Supported CAN IDs

The currently recognized CAN IDs are:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

---

## Unknown ID

A frame has an unknown ID if its CAN ID is not in the supported ID list.

Rule:

```text
Any frame not in {0x100, 0x101, 0x102, 0x200}
```

Example:

```text
Frame ID: 0x999
```

Result:

```text
FAULT: Unknown CAN ID
```

---

## Invalid DLC

A frame has an invalid DLC if it is a supported frame but its DLC is not 8.

Rule:

```text
Any supported frame where DLC != 8
```

Example:

```text
Frame ID: 0x100
DLC: 4
```

Result:

```text
FAULT: Invalid DLC
```

---

## Sensor Invalid

Sensor-valid flags are stored in a status byte.

Current masks:

```text
SENSOR1_VALID_MASK = 0x01
SENSOR2_VALID_MASK = 0x02
SENSOR3_VALID_MASK = 0x04
```

Meaning:

```text
bit 0 = sensor 1 valid
bit 1 = sensor 2 valid
bit 2 = sensor 3 valid
```

Rule:

```text
If a sensor-valid bit is not set, that sensor is considered invalid.
```

Example:

```text
Status byte: 0x00
Binary: 0000 0000
```

Result:

```text
FAULT: Sensor 1 invalid
FAULT: Sensor 2 invalid
FAULT: Sensor 3 invalid
```

---

## Voltage Low

Battery voltage is decoded from CAN ID `0x101`.

Raw field:

```text
battery_mV
```

Scaled value:

```text
battery_V = battery_mV / 1000.0
```

Rule:

```text
battery < 10.5 V
```

Result:

```text
FAULT: Battery voltage too low
```

---

## Voltage High

Battery voltage is decoded from CAN ID `0x101`.

Rule:

```text
battery > 14.8 V
```

Result:

```text
FAULT: Battery voltage too high
```

---

## Temperature High

Temperature is decoded from CAN ID `0x101`.

Raw field:

```text
temperature_deciC
```

Scaled value:

```text
temperature_C = temperature_deciC / 10.0
```

Rule:

```text
temperature > 80 C
```

Result:

```text
FAULT: Temperature too high
```

---

## Fault Detection Flow

The project separates decoding from fault checking.

```text
CAN frame bytes
        ↓
TelemetryDecoder
        ↓
Decoded values
        ↓
FaultAnalyzer
        ↓
Fault messages
```

`TelemetryDecoder` converts raw CAN bytes into meaningful values.

`FaultAnalyzer` checks those decoded values against fault rules.

---

## Current Fault Rule Summary

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