# Fault Rules

This document defines the current fault rules used by the C++ CAN Telemetry Decoder and Fault Analyzer.

The decoder validates CAN frames, decodes supported message IDs, checks signal values, tracks counters, and reports faults or warnings through structured `FaultReport` objects.

---

## Supported CAN IDs

The currently recognized CAN IDs are:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

Any frame outside this supported ID list is treated as an unknown CAN ID fault.

---

## Frame Validation Rules

Before a frame is decoded, the desktop decoder validates:

```text
CAN ID
DLC
```

A frame must have a supported CAN ID and a DLC of 8 bytes.

---

## Unknown CAN ID

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

Fault summary category:

```text
Unknown IDs
```

---

## Invalid DLC

A frame has an invalid DLC if its DLC is not 8.

Rule:

```text
DLC != 8
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

Fault summary category:

```text
Invalid DLC
```

---

## CAN ID `0x100` — Analog Inputs

CAN ID `0x100` carries analog input values and status flags.

Payload format:

```text
Byte 0: AIN1 low byte
Byte 1: AIN1 high byte
Byte 2: AIN2 low byte
Byte 3: AIN2 high byte
Byte 4: AIN3 low byte
Byte 5: AIN3 high byte
Byte 6: status flags
Byte 7: counter
```

The analog values are decoded as little-endian 16-bit values.

---

## Analog ADC Range

The analog input values are expected to stay within a 12-bit ADC range.

Rule:

```text
AIN1_RAW > 4095 -> fault
AIN2_RAW > 4095 -> fault
AIN3_RAW > 4095 -> fault
```

Expected valid range:

```text
0 to 4095
```

Example:

```text
AIN1_RAW = 5000
```

Result:

```text
FAULT: AIN1 raw value out of 12-bit ADC range
```

Fault summary category:

```text
Other faults
```

---

## Sensor Invalid Flags

Sensor-valid flags are stored in the status byte of CAN ID `0x100`.

Current masks:

```text
SENSOR1_VALID_MASK = 0x01
SENSOR2_VALID_MASK = 0x02
SENSOR3_VALID_MASK = 0x04
```

Meaning:

```text
Bit 0 = Sensor 1 valid
Bit 1 = Sensor 2 valid
Bit 2 = Sensor 3 valid
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

Fault summary category:

```text
Sensor invalid faults
```

---

## Error Flag

The analog status byte also contains an error flag.

Current mask:

```text
ERROR_FLAG_MASK = 0x80
```

Meaning:

```text
Bit 7 = Error flag
```

Rule:

```text
If bit 7 is set, the analog status byte reports an error.
```

Example:

```text
Status byte: 0x87
```

Result:

```text
FAULT: Error flag set in analog status byte
```

Fault summary category:

```text
Other faults
```

---

## CAN ID `0x101` — Battery and Temperature

CAN ID `0x101` carries battery voltage and temperature.

Payload format:

```text
Byte 0: battery_mV low byte
Byte 1: battery_mV high byte
Byte 2: temperature_deciC low byte
Byte 3: temperature_deciC high byte
Byte 4-7: reserved
```

Scaling:

```text
battery_V = battery_mV / 1000.0
temperature_C = temperature_deciC / 10.0
```

---

## Battery Voltage Low

Battery voltage is decoded from CAN ID `0x101`.

Rule:

```text
battery_V < 10.5
```

Result:

```text
FAULT: Battery voltage too low
```

Fault summary category:

```text
Voltage faults
```

---

## Battery Voltage High

Battery voltage is decoded from CAN ID `0x101`.

Rule:

```text
battery_V > 14.8
```

Result:

```text
FAULT: Battery voltage too high
```

Fault summary category:

```text
Voltage faults
```

---

## Temperature High

Temperature is decoded from CAN ID `0x101`.

Rule:

```text
temperature_C > 80.0
```

Result:

```text
FAULT: Temperature too high
```

Fault summary category:

```text
Temperature faults
```

---

## CAN ID `0x102` — Status Flags

CAN ID `0x102` carries status information.

Payload format:

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4-7: reserved
```

Current behavior:

```text
The decoder prints the sensor-valid flags, system fault byte, mode, and error code.
Fault rules for 0x102 system fault flags are planned but not fully implemented yet.
```

---

## CAN ID `0x200` — Vehicle Telemetry

CAN ID `0x200` carries vehicle telemetry data.

Payload format:

```text
Byte 0: speed_raw low byte
Byte 1: speed_raw high byte
Byte 2: rpm low byte
Byte 3: rpm high byte
Byte 4: gear
Byte 5: throttle_percent
Byte 6: brake_percent
Byte 7: counter
```

Current behavior:

```text
The decoder prints speed_raw, RPM, gear, throttle_percent, brake_percent, and counter.
Speed scaling is not implemented yet.
```

---

## Dropped Counter Detection

The decoder tracks counters per CAN ID.

Current tracked IDs:

```text
0x100
0x200
```

Rule:

```text
expected_counter = previous_counter + 1
```

If the current counter does not match the expected counter, the decoder reports a dropped counter fault.

Example:

```text
Previous counter: 2
Expected counter: 3
Current counter: 4
```

Result:

```text
FAULT: Dropped frame counter detected for ID 0x100: expected 3, got 4
```

Fault summary category:

```text
Dropped frames
```

---

## 8-bit Counter Wrap-Around

The counter is an 8-bit value.

Because it is stored as `std::uint8_t`, wrap-around from 255 to 0 is expected.

Example:

```text
Previous counter: 255
Expected counter: 0
Current counter: 0
```

Result:

```text
OK
```

---

## Possible Stuck Sensor Warning

The decoder checks whether AIN1 stays at the same value for too many active frames.

Rule:

```text
AIN1 stays at the same value for 20 active frames
```

System active condition:

```text
Sensor 1 valid = true
Sensor 2 valid = true
Sensor 3 valid = true
Error flag = false
```

Example:

```text
AIN1_RAW = 2048 for 20 active frames
```

Result:

```text
WARNING: Possible stuck sensor warning: AIN1 stayed at 2048 for 20 active frames
```

Fault summary category:

```text
Possible stuck sensors
```

This is a warning instead of a hard fault because a real sensor can sometimes remain steady during normal system operation.

---

## Fault Severity

The project separates faults from warnings.

Faults indicate a stronger error condition.

Warnings indicate suspicious behavior that may need investigation but is not guaranteed to be a real fault.

Current warning rule:

```text
Possible stuck AIN1 sensor
```

Current fault rules:

```text
Unknown CAN ID
Invalid DLC
Analog raw value out of range
Sensor invalid flag
Analog error flag
Battery voltage too low
Battery voltage too high
Temperature too high
Dropped counter
```

---

## Fault Summary Categories

The final summary groups detected issues into categories.

Current summary output includes:

```text
Invalid DLC
Unknown IDs
Voltage faults
Temperature faults
Sensor invalid faults
Dropped frames
Possible stuck sensors
Other faults
```

Example:

```text
Fault Summary:
Invalid DLC: 1
Unknown IDs: 1
Voltage faults: 0
Temperature faults: 0
Sensor invalid faults: 0
Dropped frames: 1
Possible stuck sensors: 0
Other faults: 0
```

---

## Signal Statistics

The decoder also tracks running min, max, average, and count for selected signals.

Tracked signals:

```text
AIN1_RAW
AIN2_RAW
AIN3_RAW
Battery_mV
Temperature_deciC
```

Example:

```text
Signal Stats:
AIN1_RAW: min=800, max=3124, avg=1990.67, count=3
AIN2_RAW: min=16, max=1600, avg=1005.33, count=3
AIN3_RAW: min=2337, max=2815, avg=2528.00, count=3
Battery_mV: min=12000, max=13000, avg=12533.33, count=3
Temperature_deciC: min=300, max=400, avg=348.33, count=3
Battery average: 12.53 V
Temperature average: 34.8 C
```

---

## Fault Detection Flow

The project separates parsing, validation, decoding, and fault analysis.

```text
CSV CAN log
        ↓
CanFrame parser
        ↓
CircularBuffer
        ↓
CAN ID / DLC validation
        ↓
CanDispatcher
        ↓
TelemetryDecoder
        ↓
FaultAnalyzer
        ↓
FrameReport
        ↓
DecoderStats + Fault Summary + Signal Stats
```

`TelemetryDecoder` converts raw CAN bytes into meaningful values.

`FaultAnalyzer` checks decoded values against fault rules.

`CounterTracker` detects skipped counters.

`StuckSensorTracker` detects possible repeated-value sensor behavior.

---

## Current Fault Rule Summary

```text
Unknown ID:
Any frame not in {0x100, 0x101, 0x102, 0x200}

Invalid DLC:
Any frame where DLC != 8

Analog raw out of range:
AIN1_RAW, AIN2_RAW, or AIN3_RAW > 4095

Sensor invalid:
Status flag bit not set

Analog error flag:
Status byte bit 7 set

Voltage low:
battery_V < 10.5

Voltage high:
battery_V > 14.8

Temperature high:
temperature_C > 80.0

Dropped counter:
Current counter does not equal previous counter + 1

Possible stuck sensor:
AIN1 repeats for 20 active frames
```