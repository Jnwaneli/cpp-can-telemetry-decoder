# Sample Output

This document shows the expected output summary when running the decoder with `data/sample_can_log.csv`.

---

## Input Log

Input file:

```text
data/sample_can_log.csv
```

The sample log contains:

```text
10 total frames
8 valid frames
1 unknown ID frame
1 invalid DLC frame
```

---

## Example Command

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp -o main
.\main.exe
```

---

## Expected Final Summary

The expected final decoder summary is:

```text
Decoder frames seen: 8
Decoder Stats:
Frames processed: 10
Valid frames: 8
Unknown IDs: 1
Invalid DLC: 1
Faults: 2
```

---

## Expected Faults

The sample log is expected to produce two validation faults:

```text
FAULT: Unknown CAN ID
FAULT: Invalid DLC
```

The unknown ID comes from this frame:

```text
0x999,8,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08
```

The invalid DLC comes from this frame:

```text
0x100,4,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00
```

---

## Why Decoder Frames Seen Is 8

`Decoder frames seen` counts frames that passed validation and reached the `TelemetryDecoder`.

The sample log has:

```text
10 total frames
2 rejected validation frames
8 decoded frames
```

So:

```text
Decoder frames seen: 8
```

---

## Why Faults Is 2

The total fault count includes validation faults and decoded telemetry faults.

In this sample log:

```text
1 unknown ID fault
1 invalid DLC fault
0 decoded telemetry faults
```

Total:

```text
Faults: 2
```