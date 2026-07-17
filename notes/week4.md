# Week 4 Notes — Bit Utilities, Status Flags, and Fault Analysis

## Purpose of this file

These are the regular Week 4 learning notes for the C++ CAN telemetry decoder project.

Interview-style question/answer practice should stay in:

```text
inteview_prep/interview_questions.md
```

This file should focus on what was learned and built during Week 4.

---

# Week 4 Big Picture

Week 4 moved the project from basic decoding into a more realistic embedded telemetry pipeline.

Main project flow:

```text
Simulated CAN log
→ CircularBuffer RX queue
→ Frame validation
→ TelemetryDecoder
→ byte packing and status-mask decoding
→ decoded telemetry data structs
→ FaultAnalyzer
→ OK/fault output
```

The key idea is separation of responsibility:

```text
CanFrame          = stores raw CAN frame data
CircularBuffer    = stores received frames in FIFO order
can_validation    = checks frame shape, ID, and DLC
bit_utils         = provides byte packing and bit checking helpers
TelemetryDecoder  = turns CAN payload bytes into meaningful values
AnalogData        = stores decoded analog values and flags
FaultAnalyzer     = checks decoded values for abnormal conditions
```

---

# Day 1 — Bit Basics

## Main goals

```text
Understand bit masks.
Practice setting, clearing, toggling, and checking bits.
Create bit helper functions.
Connect bit operations to CAN status bytes.
Practice Number of 1 Bits.
```

---

## What is a bit?

A bit is a single binary digit.

It can only be:

```text
0 or 1
```

A byte contains 8 bits.

Example:

```text
0b00000101
```

Bit positions are counted from right to left:

```text
bit position: 7 6 5 4 3 2 1 0
value:        0 0 0 0 0 1 0 1
```

So in `0b00000101`:

```text
bit 0 = 1
bit 1 = 0
bit 2 = 1
```

---

## What is a bit mask?

A bit mask is a value used to target one or more specific bits.

Example:

```cpp
std::uint8_t mask = 0x04;
```

Binary:

```text
0x04 = 0000 0100
```

That targets bit position `2`.

Simple explanation:

```text
A bit mask lets us isolate, check, set, clear, or toggle specific bits.
```

---

## Setting a bit

To set a bit, use bitwise OR:

```cpp
value |= mask;
```

This forces the masked bit to become `1`.

Example:

```text
value: 0000 0000
mask:  0000 0100
----------------
result:0000 0100
```

Simple explanation:

```text
Set means force a bit to 1.
```

---

## Clearing a bit

To clear a bit, use bitwise AND with the inverted mask:

```cpp
value &= static_cast<std::uint8_t>(~mask);
```

This forces the masked bit to become `0`.

Example:

```text
value: 0000 0100
mask:  0000 0100
~mask: 1111 1011
----------------
result:0000 0000
```

Simple explanation:

```text
Clear means force a bit to 0.
```

---

## Toggling a bit

To toggle a bit, use XOR:

```cpp
value ^= mask;
```

This flips the masked bit.

```text
0 becomes 1
1 becomes 0
```

Simple explanation:

```text
Toggle means change 0 to 1 or 1 to 0.
```

---

## Checking a bit

To check a bit, use bitwise AND:

```cpp
(value & mask) != 0
```

If the result is not zero, the bit is set.

Simple explanation:

```text
Checking a bit means testing whether a specific bit is 1.
```

---

## `get_bit`

`get_bit` checks whether one bit position is set.

Example:

```cpp
bool bit_is_set = get_bit(value, 2);
```

Implementation idea:

```cpp
bool get_bit(std::uint8_t value, int bit_position) {
    if (bit_position < 0 || bit_position > 7) {
        return false;
    }

    std::uint8_t mask = static_cast<std::uint8_t>(1u << bit_position);

    return (value & mask) != 0;
}
```

Simple explanation:

```text
get_bit checks whether a selected bit position is 1.
```

---

## `is_mask_set`

`is_mask_set` checks whether all bits in a mask are set.

Implementation idea:

```cpp
bool is_mask_set(std::uint8_t value, std::uint8_t mask) {
    return (value & mask) == mask;
}
```

This is stricter than:

```cpp
(value & mask) != 0
```

because:

```text
(value & mask) != 0   checks if any masked bit is set
(value & mask) == mask checks if all masked bits are set
```

For single-bit masks, both usually behave the same.

Simple explanation:

```text
is_mask_set checks whether a selected mask is fully present in the value.
```

---

## Why flags are common in embedded systems

Flags are common because embedded systems often pack many true/false states into one byte or register.

Examples:

```text
sensor valid
error active
ADC complete
UART ready
CAN fault
low voltage
overtemperature
system mode
```

Simple explanation:

```text
Flags save space and match how hardware registers and protocol status bytes are designed.
```

---

## What is a status byte?

A status byte is one byte where individual bits represent different conditions.

Example:

```text
status = 0x85 = 1000 0101
```

This means bits `7`, `2`, and `0` are set.

Simple explanation:

```text
A status byte packs multiple flags into one byte.
```

---

# Day 2 — `constexpr` Masks and Status Flags

## Main goals

```text
Understand constexpr.
Use named masks for status flags.
Decode sensor-valid bits from the 0x100 status byte.
Understand raw byte values versus interpreted status.
Practice Counting Bits.
```

---

## What is `constexpr`?

`constexpr` means compile-time constant.

It is used for values that are known before the program runs.

Example:

```cpp
constexpr std::uint8_t SENSOR1_VALID_MASK = 0x01;
```

Simple explanation:

```text
constexpr creates a constant value known at compile time.
```

---

## Why use `constexpr` for bit masks?

Bit masks are fixed protocol rules.

Example:

```text
bit 0 = sensor 1 valid
bit 1 = sensor 2 valid
bit 2 = sensor 3 valid
bit 7 = error flag
```

These meanings should not change while the program runs.

So instead of writing:

```cpp
is_mask_set(status, 0x01);
```

write:

```cpp
is_mask_set(status, SENSOR1_VALID_MASK);
```

Simple explanation:

```text
constexpr makes fixed masks readable and prevents magic numbers.
```

---

## Status masks used in the decoder

The `0x100` analog-input frame uses a status byte.

Current mask layout:

```cpp
constexpr std::uint8_t SENSOR1_VALID_MASK = 0x01; // bit 0
constexpr std::uint8_t SENSOR2_VALID_MASK = 0x02; // bit 1
constexpr std::uint8_t SENSOR3_VALID_MASK = 0x04; // bit 2
constexpr std::uint8_t ERROR_FLAG_MASK    = 0x80; // bit 7
```

Meaning:

```text
0x01 = 0000 0001 = bit 0
0x02 = 0000 0010 = bit 1
0x04 = 0000 0100 = bit 2
0x80 = 1000 0000 = bit 7
```

---

## How `0x07` is interpreted as flags

`0x07` in binary is:

```text
0000 0111
```

This means bits `0`, `1`, and `2` are set.

If those bits mean sensor validity, then:

```text
sensor 1 valid = yes
sensor 2 valid = yes
sensor 3 valid = yes
error flag      = no
```

Simple explanation:

```text
0x07 means the first three flag bits are set.
```

---

## Raw status value versus interpreted status

The raw value is the byte from the CAN payload.

Example:

```text
Status: 0x07
```

The interpreted status explains what the bits mean.

Example:

```text
Sensor1_VALID: yes
Sensor2_VALID: yes
Sensor3_VALID: yes
Error_Flag: no
```

Simple explanation:

```text
Raw value is the data byte. Interpreted status is the meaning of the bits inside it.
```

---

## Where masks fit in the decoder pipeline

For the `0x100` frame:

```text
data[0] + data[1] = AIN1 raw value
data[2] + data[3] = AIN2 raw value
data[4] + data[5] = AIN3 raw value
data[6]           = status byte
data[7]           = counter
```

The first six bytes are packed into raw ADC values:

```cpp
std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);
```

The status byte is decoded with masks:

```cpp
std::uint8_t status = frame.data[6];

bool sensor1_valid  = is_mask_set(status, SENSOR1_VALID_MASK);
bool sensor2_valid  = is_mask_set(status, SENSOR2_VALID_MASK);
bool sensor3_valid  = is_mask_set(status, SENSOR3_VALID_MASK);
bool error_flag_set = is_mask_set(status, ERROR_FLAG_MASK);
```

Simple explanation:

```text
Masks turn packed status bits into readable boolean variables.
```

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

## Why XOR finds the single number

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

## What is `AnalogData`?

`AnalogData` is a struct that holds decoded analog frame data.

Fields:

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

Simple explanation:

```text
AnalogData stores decoded values from the 0x100 CAN frame in one object.
```

---

## What should `FaultAnalyzer` do?

`FaultAnalyzer` checks decoded telemetry values against fault rules.

It checks things like:

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

## What should `FaultAnalyzer` not do?

`FaultAnalyzer` should not decode raw CAN bytes.

That is the decoder's job.

The fault analyzer should receive already-decoded values.

Simple separation:

```text
TelemetryDecoder = convert raw bytes into meaningful values
FaultAnalyzer    = check meaningful values for faults
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

# Full Decoder Path So Far

## 1. Sensor creates an electrical signal

A physical sensor outputs a voltage.

Example:

```text
sensor voltage = 2.5 V
```

The sensor itself does not create CAN data. It creates an electrical signal.

---

## 2. Microcontroller ADC converts voltage into a raw count

A 12-bit ADC converts voltage into a number from:

```text
0 to 4095
```

because:

```text
2^12 = 4096 possible values
```

Counting starts at zero, so the largest valid value is `4095`.

Example:

```text
0 V   -> 0
2.5 V -> about 2048
5 V   -> 4095
```

The `0–4095` range comes from the ADC resolution, not from CAN.

---

## 3. Sender packs ADC values into CAN bytes

CAN payload elements are bytes.

```text
1 byte = 8 bits
```

A 12-bit ADC value does not fit in one byte, so the sender stores it using two bytes.

Example:

```text
AIN1_RAW = 2048 decimal = 0x0800
```

Little-endian byte layout:

```text
low byte  = 0x00
high byte = 0x08
```

So the CAN payload contains:

```cpp
data[0] = 0x00;
data[1] = 0x08;
```

---

## 4. Program receives a `CanFrame`

Example frame:

```cpp
{0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x07, 0x05}}
```

Breakdown:

```text
ID       = 0x100
DLC      = 8
data[0]  = 0x00
data[1]  = 0x08
data[2]  = 0x10
data[3]  = 0x00
data[4]  = 0xFF
data[5]  = 0x0A
data[6]  = 0x07
data[7]  = 0x05
```

---

## 5. Frame validation checks the message shape

Before decoding, the program checks:

```cpp
if (!is_known_id(frame.id)) {
    faults.push_back("Unknown CAN ID");
}

if (!has_valid_dlc(frame)) {
    faults.push_back("Invalid DLC");
}
```

This asks:

```text
Is this a CAN ID we understand?
Does it have the expected number of bytes?
```

This is frame-level validation.

It does not check whether the sensor data is healthy yet.

---

## 6. `TelemetryDecoder` routes by CAN ID

The decoder chooses the correct helper based on the CAN ID.

Example:

```cpp
switch (frame.id) {
    case 0x100:
        decode_0x100(frame);
        break;

    case 0x101:
        decode_0x101(frame);
        break;
}
```

This matters because different CAN IDs have different payload layouts.

---

## 7. Bytes are packed back into raw values

For `0x100`:

```cpp
std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);
```

Example:

```cpp
pack_u16(0x00, 0x08)
```

becomes:

```text
0x0800 = 2048
```

Important:

```text
uint16_t is just the container.
The actual ADC value is still expected to be 0–4095 because the ADC is 12-bit.
```

---

## 8. Status byte is decoded with masks

The decoder reads:

```cpp
std::uint8_t status = frame.data[6];
```

Example:

```text
status = 0x07 = 0000 0111
```

Then masks turn the bits into booleans:

```cpp
bool sensor1_valid  = is_mask_set(status, SENSOR1_VALID_MASK);
bool sensor2_valid  = is_mask_set(status, SENSOR2_VALID_MASK);
bool sensor3_valid  = is_mask_set(status, SENSOR3_VALID_MASK);
bool error_flag_set = is_mask_set(status, ERROR_FLAG_MASK);
```

Result:

```text
sensor1_valid  = true
sensor2_valid  = true
sensor3_valid  = true
error_flag_set = false
```

---

## 9. Decoded values are stored in `AnalogData`

Conceptually:

```cpp
AnalogData data{
    ain1,
    ain2,
    ain3,
    status,
    counter,
    sensor1_valid,
    sensor2_valid,
    sensor3_valid,
    error_flag_set
};
```

Now the raw CAN bytes have become meaningful decoded data.

---

## 10. `FaultAnalyzer` checks the decoded data

Analog checks include:

```cpp
if (data.ain1_raw > 4095) {
    std::cout << "Fault: AIN1 raw value out of 12-bit ADC range" << std::endl;
}

if (!data.sensor1_valid) {
    std::cout << "Fault: Sensor 1 invalid" << std::endl;
}

if (data.error_flag_set) {
    std::cout << "Fault: Error flag set in analog status byte" << std::endl;
}
```

Battery and temperature checks include:

```cpp
if (voltage < 10.5) {
    std::cout << "Fault: Battery voltage too low" << std::endl;
}

if (voltage > 14.8) {
    std::cout << "Fault: Battery voltage too high" << std::endl;
}

if (temperature > 80.0) {
    std::cout << "Fault: Temperature too high" << std::endl;
}
```

This is data-level fault analysis.

---

# Week 4 Main Takeaway

```text
Week 4 introduced the bit-level and fault-analysis layer of the CAN decoder. The project now validates frame shape, decodes raw bytes into meaningful values, extracts status flags with masks, stores decoded analog data in a struct, and checks the decoded values with a FaultAnalyzer.
```
---

# Day 4 — Decode 0x102 and Reverse Bits

## Main goals

```text
Study reverse bits.
Solve Reverse Bits.
Decode CAN ID 0x102.
Interpret sensor valid flags, system fault byte, mode, and error code.
```

---

## What does 0x102 represent?

CAN ID `0x102` represents a status frame.

It carries system state information rather than raw analog measurements.

Payload format:

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4-7: reserved
```

Simple explanation:

```text
0x102 tells me the system status, sensor validity, mode, and error state.
```

---

## 0x102 payload format

```text
Byte 0: sensor valid flags
Byte 1: system fault flags
Byte 2: mode
Byte 3: error code
Byte 4-7: reserved
```

Example:

```text
0x07 0x00 0x01 0x00 0x00 0x00 0x00 0x00
```

Meaning:

```text
sensor valid flags = 0x07
system fault byte = 0x00
mode = 1
error code = 0
```

---

## Sensor valid flags

The sensor valid flags byte uses individual bits.

```text
bit 0 = sensor 1 valid
bit 1 = sensor 2 valid
bit 2 = sensor 3 valid
```

If byte 0 is:

```text
0x07 = 0000 0111
```

then bits 0, 1, and 2 are set.

So:

```text
Sensor1 Valid: YES
Sensor2 Valid: YES
Sensor3 Valid: YES
```

---

## How are status flags different from analog values?

Analog values are numeric measurements.

Examples:

```text
AIN1_RAW: 2048
Battery: 12.60 V
Temperature: 34.5 C
```

Status flags are true/false meanings stored in bits.

Examples:

```text
Sensor1 Valid: YES
Sensor2 Valid: YES
System Fault Active: NO
```

Simple explanation:

```text
Analog values measure quantities, while status flags describe conditions.
```

---

## System fault byte

The system fault byte stores fault bits.

For now:

```text
System Fault Byte: 0x00
```

means no system fault bits are set.

Later this could represent:

```text
low voltage
overtemperature
communication fault
sensor fault
internal error
```

---

## Mode

The mode byte tells what state the embedded system is in.

Example modes could be:

```text
0 = idle
1 = running
2 = calibration
3 = fault
```

In the sample frame:

```text
Mode: 1
```

This can be interpreted as the system running.

---

## Error code

The error code gives a compact reason for a fault or abnormal state.

Example error codes could be:

```text
0 = no error
1 = low voltage
2 = high temperature
3 = sensor failure
4 = CAN timeout
```

In the sample frame:

```text
Error Code: 0
```

This means no specific error code is active.

---

## Why are modes and error codes common in embedded systems?

Modes and error codes are common because embedded systems need compact ways to report system state and problems.

A mode tells what the system is currently doing.

An error code tells what went wrong.

Simple explanation:

```text
Modes describe system state, and error codes describe specific problems.
```

---

## Reverse Bits

LeetCode 190 asks me to reverse all 32 bits of an unsigned integer.

Main idea:

```text
Take the lowest bit from n.
Shift result left.
Put that bit into result.
Shift n right.
Repeat 32 times.
```

Code idea:

```cpp
uint32_t result = 0;

for (int i = 0; i < 32; i++) {
    result <<= 1;
    result |= (n & 1);
    n >>= 1;
}
```

---

## How Reverse Bits works

`n & 1` gets the lowest bit of `n`.

`result <<= 1` makes room in the result.

`result |= (n & 1)` inserts the extracted bit.

`n >>= 1` moves to the next bit.

Simple explanation:

```text
Reverse Bits reads bits from the right side of n and builds the reversed result from left to right.
```

---

## Day 4 main interview idea

```text
0x102 is a status frame. Unlike analog frames that carry numeric measurements, status frames carry bit-level flags, mode, and error code information. This is common in embedded systems because status bytes compactly represent system state and fault conditions.
```
---

# Day 5 — volatile Concept and Sensor Faults

## Main goals

```text
Understand embedded-style volatile.
Understand why volatile is used for hardware registers.
Understand why volatile is not thread safety.
Add sensor-invalid faults.
Solve Sum of Two Integers.
```

---

## What is volatile?

`volatile` tells the compiler that a value can change outside normal program flow.

Example:

```cpp
volatile std::uint32_t GPIO_ODR;
```

This means the compiler should not assume `GPIO_ODR` only changes when normal C++ code assigns to it.

Simple explanation:

```text
volatile tells the compiler to actually read or write the variable because something outside normal code may change it.
```

---

## Why is volatile used for registers?

Hardware registers can change because of hardware events.

Examples:

```text
GPIO input pin changes voltage
UART receives data
ADC conversion completes
timer counter increments
interrupt flag gets set
CAN message arrives
```

The compiler might normally optimize repeated reads if it thinks a value has not changed.

`volatile` prevents that optimization for the marked variable.

Simple explanation:

```text
volatile is used for registers because hardware can change register values without normal C++ assignment.
```

---

## GPIO register example

Example:

```cpp
volatile std::uint32_t GPIO_ODR;

GPIO_ODR |= (1U << 5);
GPIO_ODR &= ~(1U << 5);
```

This simulates setting and clearing bit 5 of a GPIO output register.

Meaning:

```text
set bit 5
clear bit 5
```

This is similar to turning an LED pin on and off.

---

## Common volatile examples

Common embedded uses:

```text
hardware registers
ISR-shared flags
memory-mapped peripheral registers
status registers
timer counters
GPIO input/output registers
```

Example ISR-shared flag:

```cpp
volatile bool button_pressed = false;
```

An interrupt might set this flag, while the main loop reads it.

---

## Why is volatile not enough for thread safety?

`volatile` does not make operations atomic.

It does not prevent race conditions.

Example:

```cpp
counter++;
```

This can involve multiple steps:

```text
read counter
add one
write counter
```

If an interrupt or another thread changes `counter` during those steps, the result can still be wrong.

Simple explanation:

```text
volatile controls compiler optimization, but it does not protect shared data.
```

---

## What should be used for thread safety?

Depending on the system, thread safety may require:

```text
atomic variables
mutexes
critical sections
interrupt disabling
RTOS synchronization
```

`volatile` alone is not enough.

---

## Sensor-valid flags

The `0x100` status byte contains sensor-valid flags.

Masks:

```cpp
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

---

## How do sensor-valid flags produce faults?

If a sensor-valid bit is not set, that sensor is considered invalid.

Example:

```cpp
if (!data.sensor1_valid) {
    std::cout << "FAULT: Sensor 1 invalid" << std::endl;
}
```

If status byte is:

```text
0x00 = 0000 0000
```

then no sensor-valid bits are set.

So the output is:

```text
FAULT: Sensor 1 invalid
FAULT: Sensor 2 invalid
FAULT: Sensor 3 invalid
```

Simple explanation:

```text
Missing valid bits become sensor-invalid faults.
```

---

## Sum of Two Integers

LeetCode 371 asks me to add two integers without using `+` or `-`.

The bit idea:

```text
XOR gives the sum without carry.
AND gives the carry.
Shift the carry left.
Repeat until carry is zero.
```

Code idea:

```cpp
while (b != 0) {
    unsigned carry = static_cast<unsigned>(a & b) << 1;
    a = a ^ b;
    b = static_cast<int>(carry);
}
```

---

## Why XOR helps with addition

XOR behaves like addition without carry.

Example:

```text
0 ^ 0 = 0
1 ^ 0 = 1
0 ^ 1 = 1
1 ^ 1 = 0
```

The `1 ^ 1 = 0` case needs a carry.

That carry is found using AND.

---

## Day 5 main interview idea

```text
volatile tells the compiler that a variable can change outside normal program flow, which is important for hardware registers and ISR-shared flags. It does not provide thread safety. In the CAN decoder, missing sensor-valid bits are interpreted as sensor faults and reported by FaultAnalyzer.
```z