## CAN ID 0x200 — Vehicle Telemetry

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

Example payload:

```text
D2 04 AC 0D 03 2D 00 04
```

Decoded values:

```text
speed_raw = 1234
rpm = 3500
gear = 3
throttle_percent = 45
brake_percent = 0
counter = 4
```

Current speed scaling:

```text
not implemented yet
```

Future scaling could convert `speed_raw` into a real speed value such as miles per hour or kilometers per hour.

Example future scaling idea:

```text
speed_mph = speed_raw * scale_factor
```