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
---

# Day 2 — Running Stats and FreeRTOS Tasks Compile

## Main goals

```text
Study DP state, transition, and base case.
Solve Min Cost Climbing Stairs.
Implement min/max/average tracking with SignalStats.
Track AIN1, AIN2, AIN3, battery, and temperature values.
Enable FreeRTOS/CMSIS-RTOS in CubeMX.
Create SignalGeneratorTask, ProcessingTask, CanTxTask, and StatusLedTask.
Keep tasks minimal first.
Confirm the FreeRTOS project builds.
Blink heartbeat LED if possible.
```

---

## Notes questions

```text
1. How do I compute running average?
2. Why are min and max useful for telemetry?
3. What tasks did I create?
4. What is a task?
5. What does task priority mean?
6. Did the scheduler run?
```

---

## DP state, transition, and base case

Dynamic programming problems usually have three parts:

```text
state
transition
base case
```

State means what each DP value represents.

Transition means how the next value is calculated from previous values.

Base case means the starting known answers.

Simple explanation:

```text
State defines the meaning, transition defines the formula, and base case starts the solution.
```

---

## Min Cost Climbing Stairs

For Min Cost Climbing Stairs:

```text
state:
dp[i] = minimum cost to reach step i

transition:
dp[i] = cost[i] + min(dp[i - 1], dp[i - 2])

base case:
dp[0] = cost[0]
dp[1] = cost[1]
```

Simple explanation:

```text
To reach one step, choose the cheaper of the two previous steps.
```

---

## How do I compute running average?

A running average updates the average every time a new sample arrives.

Formula:

```text
average = previous_average + (new_value - previous_average) / count
```

This avoids storing every previous sample.

Simple explanation:

```text
Each new value nudges the average toward the new sample.
```

---

## Why are min and max useful for telemetry?

Min and max show the range of a signal over time.

They can help detect:

```text
spikes
drops
out-of-range values
signals that are not changing
unexpected sensor behavior
```

Simple explanation:

```text
Min and max show the lowest and highest values seen so far.
```

---

## SignalStats tracked values

The decoder now tracks:

```text
AIN1 min/max/average
AIN2 min/max/average
AIN3 min/max/average
battery min/max/average
temperature min/max/average
```

Simple explanation:

```text
SignalStats turns decoded frame values into signal history.
```

---

## What tasks did I create?

The FreeRTOS project now has these basic tasks:

```text
SignalGeneratorTask
ProcessingTask
CanTxTask
StatusLedTask
```

For Day 2, these tasks can be minimal loops.

Simple explanation:

```text
Today creates the FreeRTOS skeleton before adding real queue/mutex/CAN behavior.
```

---

## What is a task?

A FreeRTOS task is an independent function managed by the scheduler.

Each task usually has:

```text
its own loop
its own stack
a priority
a delay or blocking call
```

Simple explanation:

```text
A task is a scheduled piece of firmware work.
```

---

## What does task priority mean?

Task priority tells the scheduler which ready task should run first.

If two tasks are ready, the higher-priority task runs before the lower-priority task.

Simple explanation:

```text
Priority controls which ready task gets CPU time first.
```

---

## Did the scheduler run?

The easiest Day 2 proof is the heartbeat LED.

If `StatusLedTask` toggles the LED every 500 ms, then the scheduler is running.

Simple explanation:

```text
A blinking heartbeat LED proves at least one FreeRTOS task is executing.
```

---

## Day 2 status

```text
SignalStats update function working.
AIN1/AIN2/AIN3 stats started.
Battery and temperature stats started.
FreeRTOS enabled.
Basic task skeletons created.
StatusLedTask added.
Build and LED status recorded honestly.
```

---

## Day 2 main interview idea

```text
Min Cost Climbing Stairs uses dynamic programming with a state, transition, and base case. In the decoder, SignalStats computes running min, max, average, and count for telemetry signals without storing every sample. In the firmware, FreeRTOS tasks split the program into scheduled units of work, and the StatusLedTask heartbeat helps confirm that the scheduler is running.
```