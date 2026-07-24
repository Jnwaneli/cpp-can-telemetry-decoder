# Week 7 Notes

---

# Day 1 — SignalStats and FreeRTOS Project Copy

## Main goals

```text
Study basic dynamic programming using Climbing Stairs.
Solve Climbing Stairs three ways: recursion, memoization, and bottom-up DP.
Create SignalStats for future telemetry min/max/average tracking.
Duplicate the working STM32 CAN sender project into stm32_can_sender_freertos.
Do not change FDCAN settings yet.
Do not change working CAN pins yet.
Confirm the duplicated project builds.
```

---

## Notes questions

```text
1. What is dynamic programming?
2. What is the difference between recursion and memoization?
3. What is SignalStats supposed to track?
4. What project did I duplicate?
5. What CAN settings must I preserve?
6. Why should I not change working FDCAN settings yet?
```

---

## What is dynamic programming?

Dynamic programming is a way to solve problems by breaking them into smaller overlapping subproblems.

It stores previous answers so the same work is not repeated.

Simple explanation:

```text
Dynamic programming saves answers to repeated subproblems.
```

---

## Climbing Stairs DP idea

For Climbing Stairs:

```text
ways(n) = ways(n - 1) + ways(n - 2)
```

Reason:

```text
To reach stair n, I can come from stair n - 1 by taking one step.
I can also come from stair n - 2 by taking two steps.
```

Simple explanation:

```text
The answer for each stair depends on the previous two answers.
```

---

## What is the difference between recursion and memoization?

Recursion solves a problem by calling the same function on smaller inputs.

Memoization is recursion with saved results.

Plain recursion can repeat the same work many times.

Memoization stores answers so repeated subproblems are only solved once.

Simple explanation:

```text
Memoization makes recursion faster by remembering old answers.
```

---

## Three Climbing Stairs solutions

The three versions are:

```text
plain recursion
memoization
bottom-up dynamic programming
```

Plain recursion is easiest to understand but slow.

Memoization avoids repeated work.

Bottom-up DP builds the answer from small cases to the final case.

---

## What is SignalStats supposed to track?

`SignalStats` tracks basic telemetry statistics.

It stores:

```text
min_value
max_value
average
count
```

Later, it will help summarize signals like:

```text
AIN1
AIN2
AIN3
battery_mV
temperature_deciC
speed_raw
rpm
```

Simple explanation:

```text
SignalStats helps the decoder understand signal behavior over time, not just one frame.
```

---

## SignalStats structure

```cpp
struct SignalStats {
    std::uint16_t min_value;
    std::uint16_t max_value;
    double average;
    std::uint32_t count;

    SignalStats();

    void update(std::uint16_t new_value);
    void reset();
    bool has_samples() const;
};
```

---

## Why use update(uint16_t new_value)?

The `update()` function receives a new telemetry value and updates:

```text
minimum value
maximum value
average value
sample count
```

Simple explanation:

```text
Each new signal value updates the running stats.
```

---

## What project did I duplicate?

I duplicated:

```text
embedded/can_hardware_bridge/stm32_can_sender/
```

into:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

Simple explanation:

```text
The FreeRTOS sender starts as a safe copy of the working CAN sender.
```

---

## What CAN settings must I preserve?

I must preserve:

```text
FDCAN bitrate
FDCAN pins
FDCAN mode
CAN frame format
clock settings
GPIO alternate functions
working CAN transmit setup
known-good CAN payload/settings
```

Simple explanation:

```text
Do not touch the known-good CAN configuration yet.
```

---

## Why should I not change working FDCAN settings yet?

The first goal is to prove the duplicated project builds.

If I change CAN settings immediately and something breaks, I will not know what caused the issue.

The problem could be:

```text
copy/import problem
CubeIDE project name problem
FreeRTOS configuration problem
FDCAN configuration problem
pin configuration problem
```

Simple explanation:

```text
Confirm the copy works before changing anything.
```

---

## Day 1 deliverable status

```text
SignalStats started.
FreeRTOS sender project folder created.
Duplicated STM32 project build status recorded.
FDCAN settings unchanged.
CAN pins unchanged.
FreeRTOS not enabled yet.
```

---

## Day 1 main interview idea

```text
Dynamic programming avoids repeated work by storing answers to overlapping subproblems. Climbing Stairs can be solved with recursion, memoization, or bottom-up DP using the recurrence ways(n) = ways(n - 1) + ways(n - 2). In the CAN decoder project, SignalStats starts signal-level analysis by tracking min, max, average, and count. On the firmware side, the working STM32 CAN sender is duplicated before enabling FreeRTOS so the original known-good CAN configuration remains protected.
```