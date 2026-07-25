# CAN Protocol

This document defines the CAN protocol used by the C++ CAN Telemetry Decoder and Fault Analyzer.

The project supports simulated CAN logs, captured Waveshare USB-CAN logs, and STM32 FreeRTOS-generated CAN frames.

---

## General Frame Format

Each supported CAN frame uses:

```text
Standard CAN ID
Classic CAN
DLC = 8 bytes
Little-endian 16-bit signal packing
```

The desktop decoder expects CSV input in this format:

```csv
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
100,8,00,08,10,00,FF,0A,07,01
101,8,38,31,59,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,D2,04,AC,0D,03,2D,00,04
```

CSV field meaning:

```text
id  = CAN ID, parsed as hexadecimal
dlc = data length code, parsed as decimal
b0-b7 = payload bytes, parsed as hexadecimal
```

Example:

```text
100 means CAN ID 0x100
8 means DLC 8
FF means payload byte 0xFF
```

---

## Byte Order

Multi-byte values are packed in little-endian format.

That means the low byte comes first and the high byte comes second.

Example:

```text
Raw value: 0x1234
Low byte:  0x34
High byte: 0x12
Payload:   34 12
```

The C++ decoder reconstructs 16-bit values like this:

```text
value = low_byte | (high_byte << 8)
```

---

## Supported CAN IDs

| CAN ID | Name | Purpose |
|---|---|---|
| `0x100` | Analog Inputs | ADC-style analog sensor values |
| `0x101` | Battery and Temperature | Battery voltage and temperature |
| `0x102` | Status Flags | Sensor validity, mode, and error code |
| `0x200` | Vehicle Telemetry | Speed, RPM, gear, throttle, brake, and counter |

Any other CAN ID is treated as an unknown ID fault.

---

# CAN ID `0x100` — Analog Inputs

CAN ID `0x100` carries analog input values, sensor status flags, and a frame counter.

## Payload Format

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

## Decoded Signals

| Field | Type | Description |
|---|---|---|
| `AIN1_RAW` | `uint16_t` | Analog input 1 raw ADC value |
| `AIN2_RAW` | `uint16_t` | Analog input 2 raw ADC value |
| `AIN3_RAW` | `uint16_t` | Analog input 3 raw ADC value |
| `status` | `uint8_t` | Sensor-valid flags and error flag |
| `counter` | `uint8_t` | Frame counter |

## Status Byte

```text
Bit 0 = Sensor 1 valid
Bit 1 = Sensor 2 valid
Bit 2 = Sensor 3 valid
Bit 7 = Error flag
```

Current masks:

```text
Sensor 1 valid mask = 0x01
Sensor 2 valid mask = 0x02
Sensor 3 valid mask = 0x04
Error flag mask      = 0x80
```

## Example Frame

```text
CAN ID: 0x100
DLC: 8
Data: 00 08 10 00 FF 0A 07 01
```

## Decoding

```text
AIN1 = 0x0800 = 2048
AIN2 = 0x0010 = 16
AIN3 = 0x0AFF = 2815
Status = 0x07
Counter = 1
```

Status decode:

```text
0x07 = 0000 0111

Sensor 1 valid = yes
Sensor 2 valid = yes
Sensor 3 valid = yes
Error flag = no
```

## Fault Rules

The decoder checks:

```text
AIN1_RAW > 4095
AIN2_RAW > 4095
AIN3_RAW > 4095
sensor valid flags
error flag
dropped counter
possible stuck AIN1 sensor
```

---

# CAN ID `0x101` — Battery and Temperature

CAN ID `0x101` carries battery voltage and temperature.

## Payload Format

```text
Byte 0: battery_mV low byte
Byte 1: battery_mV high byte
Byte 2: temperature_deciC low byte
Byte 3: temperature_deciC high byte
Byte 4: reserved
Byte 5: reserved
Byte 6: reserved
Byte 7: reserved
```

## Decoded Signals

| Field | Type | Description |
|---|---|---|
| `battery_mV` | `uint16_t` | Battery voltage in millivolts |
| `battery_V` | `double` | Battery voltage in volts |
| `temperature_deciC` | `uint16_t` | Temperature in tenths of a degree Celsius |
| `temperature_C` | `double` | Temperature in degrees Celsius |

## Scaling

```text
battery_V = battery_mV / 1000.0
temperature_C = temperature_deciC / 10.0
```

## Example Frame

```text
CAN ID: 0x101
DLC: 8
Data: 38 31 59 01 00 00 00 00
```

## Decoding

```text
battery_mV = 0x3138 = 12600
battery_V = 12.60 V

temperature_deciC = 0x0159 = 345
temperature_C = 34.5 C
```

## Fault Rules

The decoder checks:

```text
battery_V < 10.5
battery_V > 14.8
temperature_C > 80.0
```

---

# CAN ID `0x102` — Status Flags

CAN ID `0x102` carries status information.

## Payload Format

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4: reserved
Byte 5: reserved
Byte 6: reserved
Byte 7: reserved
```

## Decoded Signals

| Field | Type | Description |
|---|---|---|
| `sensor_valid_flags` | `uint8_t` | Sensor validity flags |
| `system_fault_flags` | `uint8_t` | System fault byte |
| `mode` | `uint8_t` | Current system mode |
| `error_code` | `uint8_t` | Current error code |

## Sensor Valid Flags

```text
Bit 0 = Sensor 1 valid
Bit 1 = Sensor 2 valid
Bit 2 = Sensor 3 valid
```

## Example Frame

```text
CAN ID: 0x102
DLC: 8
Data: 07 00 01 00 00 00 00 00
```

## Decoding

```text
Sensor valid flags = 0x07
System fault flags = 0x00
Mode = 1
Error code = 0
```

Status decode:

```text
Sensor 1 valid = YES
Sensor 2 valid = YES
Sensor 3 valid = YES
System fault byte = 0x00
Mode = 1
Error code = 0
```

## Current Behavior

The decoder currently prints the decoded status fields.

Additional fault rules for `system_fault_flags` and `error_code` are planned but not fully implemented yet.

---

# CAN ID `0x200` — Vehicle Telemetry

CAN ID `0x200` carries vehicle telemetry data.

## Payload Format

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

## Decoded Signals

| Field | Type | Description |
|---|---|---|
| `speed_raw` | `uint16_t` | Raw speed value |
| `rpm` | `uint16_t` | Engine/motor RPM |
| `gear` | `uint8_t` | Current gear |
| `throttle_percent` | `uint8_t` | Throttle percentage |
| `brake_percent` | `uint8_t` | Brake percentage |
| `counter` | `uint8_t` | Frame counter |

## Example Frame

```text
CAN ID: 0x200
DLC: 8
Data: D2 04 AC 0D 03 2D 00 04
```

## Decoding

```text
speed_raw = 0x04D2 = 1234
rpm = 0x0DAC = 3500
gear = 3
throttle_percent = 45
brake_percent = 0
counter = 4
```

## Current Speed Scaling

```text
not implemented yet
```

Future scaling could convert `speed_raw` into a real speed value such as miles per hour or kilometers per hour.

Example future scaling idea:

```text
speed_mph = speed_raw * scale_factor
```

## Fault Rules

The decoder currently checks the `0x200` counter for dropped frames.

Additional vehicle signal rules are planned.

---

# Counter Rules

The decoder tracks frame counters per CAN ID.

Current tracked IDs:

```text
0x100
0x200
```

Expected counter behavior:

```text
expected_counter = previous_counter + 1
```

Example normal sequence:

```text
1, 2, 3, 4
```

Example dropped-frame sequence:

```text
1, 2, 4
```

Result:

```text
FAULT: Dropped frame counter detected
```

## 8-bit Wrap-Around

The counter is an 8-bit value.

Valid wrap-around:

```text
254, 255, 0, 1
```

When the previous counter is `255`, the expected next counter is `0`.

---

# FreeRTOS Sender Compatibility

The STM32 FreeRTOS telemetry sender transmits the same CAN IDs that the desktop C++ decoder supports.

FreeRTOS sender path:

```text
SignalGeneratorTask
        ↓
sensorQueueHandle
        ↓
ProcessingTask
        ↓
mutex-protected latestTelemetry
        ↓
CanTxTask
        ↓
STM32 FDCAN
        ↓
SN65HVD230
        ↓
Waveshare USB-CAN
        ↓
PC captured CSV log
        ↓
C++ decoder
```

The FreeRTOS firmware sends:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

All frames use:

```text
Standard CAN ID
Classic CAN
DLC = 8
Bitrate = 500 kbps
Little-endian 16-bit packing
```

---

# Captured FreeRTOS Log Example

Example captured FreeRTOS CSV:

```csv
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
100,8,7A,0D,DE,0D,42,0E,07,4B
101,8,48,31,5F,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,80,02,0A,0F,03,29,00,4B
100,8,B0,04,14,05,78,05,07,4C
101,8,3F,31,60,01,00,00,00,00
102,8,07,00,01,00,00,00,00,00
200,8,96,02,1D,10,03,34,00,4C
```

Expected behavior:

```text
0x100 decodes as Analog Inputs
0x101 decodes as Battery and Temperature
0x102 decodes as Status Flags
0x200 decodes as Vehicle Telemetry
Dropped frames should be 0 if counters are sequential
```

---

# Current Limitations

```text
The desktop C++ decoder currently reads CSV/captured logs, not live USB-CAN frames directly.
Speed scaling for 0x200 is not implemented yet.
0x102 system fault byte rules are not fully implemented yet.
The FreeRTOS sender currently generates simulated telemetry, not real sensor ADC readings.
```

---

# Planned Protocol Extensions

```text
Add direct live USB-CAN reader
Add SocketCAN/can0 live reader
Add candump-style log parsing
Add speed scaling for 0x200
Add system fault flag rules for 0x102
Add real ADC sensor input on STM32
Add optional FreeRTOS FaultInjectTask
```