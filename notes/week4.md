---

# Day 3 — XOR and FaultAnalyzer

## Main goals

```text
Study XOR.
Solve Single Number.
Solve Missing Number.
Create FaultAnalyzer class.
Separate decoded telemetry data from fault checking logic.
```

---

## What is XOR?

XOR is a bitwise operation written with:

```cpp
^
```

Important rules:

```text
x ^ x = 0
x ^ 0 = x
```

This means:

```text
a value XOR itself cancels to zero
a value XOR zero stays unchanged
```

---

## XOR for toggling bits

XOR can toggle bits.

Example:

```cpp
value ^= mask;
```

If the masked bit is `0`, it becomes `1`.

If the masked bit is `1`, it becomes `0`.

Simple explanation:

```text
XOR flips selected bits.
```

---

## Why does XOR find the single number?

In Single Number, every value appears twice except one.

Duplicate values cancel because:

```text
x ^ x = 0
```

The remaining value stays because:

```text
x ^ 0 = x
```

Example:

```text
4 ^ 1 ^ 2 ^ 1 ^ 2

1 cancels with 1
2 cancels with 2

leftover = 4
```

Simple explanation:

```text
XOR finds the single number because pairs cancel out and the unpaired value remains.
```

---

## Single Number code idea

```cpp
int result = 0;

for (int num : nums) {
    result ^= num;
}

return result;
```

The result starts at zero because:

```text
x ^ 0 = x
```

---

## Missing Number with XOR

Missing Number can also use XOR.

Main idea:

```text
XOR all numbers from 0 to n.
XOR all numbers in the input.
Everything that appears in both cancels.
The missing number remains.
```

This works because XOR cancellation removes matching pairs.

---

## What is AnalogData?

`AnalogData` is a struct that holds decoded analog frame data.

Example fields:

```text
ain1_raw
ain2_raw
ain3_raw
status
counter
sensor1_valid
sensor2_valid
sensor3_valid
error_flag_set
```

This lets the decoder pass meaningful data to the fault analyzer.

---

## What should a FaultAnalyzer class do?

A `FaultAnalyzer` should check decoded telemetry values against fault rules.

It should check things like:

```text
sensor invalid flags
analog values out of ADC range
battery voltage too low
battery voltage too high
temperature too high
error flag set
```

Simple explanation:

```text
FaultAnalyzer checks decoded values and reports problems.
```

---

## What should FaultAnalyzer not do?

`FaultAnalyzer` should not decode raw CAN bytes.

That is the decoder's job.

The fault analyzer should receive already-decoded values.

Simple separation:

```text
TelemetryDecoder = convert raw bytes into meaningful values
FaultAnalyzer = check meaningful values for faults
```

---

## Why separate decoding from fault checking?

Decoding and fault checking are different responsibilities.

The decoder answers:

```text
What do these bytes mean?
```

The fault analyzer answers:

```text
Do these decoded values indicate a problem?
```

Separating them makes the project:

```text
cleaner
easier to test
easier to expand
easier to explain
more professional
```

Simple explanation:

```text
Separating decoding from fault checking keeps each class focused on one job.
```

---

## Day 3 main interview idea

```text
XOR is useful because duplicate values cancel out, which solves problems like Single Number. In the CAN decoder project, FaultAnalyzer separates fault rules from byte decoding, so TelemetryDecoder converts raw CAN payloads into values while FaultAnalyzer checks those values for abnormal conditions.
```