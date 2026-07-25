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
---

# Day 3 — Dropped Counter Detection and Queue Setup

## Main goals

```text
Study why greedy does not always work.
Solve House Robber.
Add dropped counter detection to the C++ decoder.
Handle 8-bit counter wrap-around from 255 to 0.
Define SensorSample and ProcessedTelemetry in the FreeRTOS firmware.
Create sensorQueueHandle.
Create telemetryMutexHandle.
Create latestTelemetry shared state.
Start the FreeRTOS queue/mutex architecture.
```

---

## Notes questions

```text
1. Why do telemetry protocols use counters?
2. How does dropped counter detection work?
3. How does counter wrap-around work?
4. What is a queue?
5. Why use a queue instead of a global variable?
6. What is a mutex?
7. What shared data needs protection?
```

---

## Why greedy does not always work

Greedy means making the best-looking choice right now.

Greedy does not always work because a good immediate choice can block a better future choice.

House Robber is an example because robbing one house prevents robbing its neighbors.

Simple explanation:

```text
Sometimes the best local choice is not the best total choice.
```

---

## House Robber DP idea

For House Robber:

```text
state:
dp[i] = maximum money from houses 0 through i

transition:
dp[i] = max(dp[i - 1], nums[i] + dp[i - 2])

base case:
dp[0] = nums[0]
dp[1] = max(nums[0], nums[1])
```

Simple explanation:

```text
At each house, choose between skipping it or robbing it.
```

---

## Why do telemetry protocols use counters?

Telemetry protocols use counters to show whether messages are arriving in order.

Counters can help detect:

```text
dropped frames
repeated frames
stale data
communication gaps
```

Simple explanation:

```text
A counter helps prove that the message stream is updating correctly.
```

---

## How does dropped counter detection work?

The decoder remembers the previous counter value for each CAN ID.

Then it computes:

```text
expected_counter = previous_counter + 1
```

If the actual counter does not match the expected counter, the decoder reports a dropped counter fault.

Example:

```text
Previous counter: 5
Expected counter: 6
Actual counter: 8
Result: dropped counter fault
```

Simple explanation:

```text
If the counter skips, the decoder assumes one or more frames may have been missed.
```

---

## How does counter wrap-around work?

The counter is 8-bit.

So it can store:

```text
0 through 255
```

After `255`, the next valid value is `0`.

Example:

```text
Previous counter: 255
Expected counter: 0
Actual counter: 0
Result: OK
```

Simple explanation:

```text
255 to 0 is normal for an 8-bit counter.
```

---

## What is a queue?

A queue is an RTOS communication object that lets one task send data to another task.

In this firmware:

```text
SignalGeneratorTask sends SensorSample
ProcessingTask receives SensorSample
```

Simple explanation:

```text
A queue safely moves data from one task to another.
```

---

## Why use a queue instead of a global variable?

A queue is safer than a plain global variable because it stores messages in order and lets the receiving task block until data arrives.

A global variable can be overwritten before another task reads it.

Simple explanation:

```text
A queue prevents missed handoffs between producer and consumer tasks.
```

---

## What is a mutex?

A mutex is a lock used to protect shared data.

Only one task can hold the mutex at a time.

Simple explanation:

```text
A mutex prevents two tasks from touching shared data at the same time.
```

---

## What shared data needs protection?

The shared data is:

```text
latestTelemetry
```

It is shared because:

```text
ProcessingTask writes latestTelemetry
CanTxTask reads latestTelemetry
```

So it must be protected with:

```text
telemetryMutexHandle
```

Simple explanation:

```text
latestTelemetry needs a mutex because one task writes it and another task reads it.
```

---

## FreeRTOS Day 3 architecture

```text
SignalGeneratorTask
        ↓ SensorSample
sensorQueueHandle
        ↓
ProcessingTask
        ↓ mutex-protected write
latestTelemetry
        ↓ mutex-protected read
CanTxTask
```

---

## Day 3 status

```text
Dropped counter detection added.
Wrap-around from 255 to 0 handled.
SensorSample struct created.
ProcessedTelemetry struct created.
sensorQueueHandle created.
telemetryMutexHandle created.
latestTelemetry shared state created.
FreeRTOS queue/mutex architecture started.
```

---

## Day 3 main interview idea

```text
Telemetry counters help detect dropped or stale messages by checking whether each received counter follows the previous value. The decoder handles normal 8-bit wrap-around from 255 to 0. On the FreeRTOS side, a queue safely transfers SensorSample messages from SignalGeneratorTask to ProcessingTask, while a mutex protects latestTelemetry because ProcessingTask writes it and CanTxTask reads it.
```
---

# Day 5 — Fault Summary and ProcessingTask

## Main goals

```text
Study Longest Increasing Subsequence.
Understand the DP pattern without needing full mastery yet.
Create FaultAnalyzer::print_summary().
Group faults by category.
Implement ProcessingTask.
ProcessingTask blocks on sensorQueueHandle.
ProcessingTask receives SensorSample.
ProcessingTask updates latestTelemetry with mutex protection.
```

---

## Notes questions

```text
1. What makes a good fault summary?
2. How would a technician use this output?
3. What does blocking on a queue mean?
4. Why does ProcessingTask use a mutex?
5. What could happen without a mutex?
```

---

## Longest Increasing Subsequence DP idea

For LIS:

```text
state:
dp[i] = length of the longest increasing subsequence ending at index i

base case:
dp[i] starts at 1

transition:
if nums[j] < nums[i]:
    dp[i] = max(dp[i], dp[j] + 1)
```

Simple explanation:

```text
For each number, look backward and see if it can extend a previous increasing subsequence.
```

---

## What makes a good fault summary?

A good fault summary groups problems by category.

Instead of only showing individual frame messages, it shows totals like:

```text
Invalid DLC
Unknown IDs
Voltage faults
Temperature faults
Dropped frames
Possible stuck sensors
```

Simple explanation:

```text
A good fault summary tells the user what kinds of problems happened and how often.
```

---

## How would a technician use this output?

A technician could use the summary to decide where to debug first.

Examples:

```text
Many invalid DLC faults -> check frame formatting or transmitter payload size.
Many unknown IDs -> check CAN ID map or unexpected device traffic.
Many voltage faults -> check battery signal, scaling, or power system.
Many temperature faults -> check temperature sensor, scaling, or thermal issue.
Many dropped frames -> check bus load, timing, or receiver reliability.
Possible stuck sensors -> check sensor wiring, ADC input, or constant test data.
```

Simple explanation:

```text
The summary points the technician toward the most likely debug area.
```

---

## What does blocking on a queue mean?

Blocking on a queue means a task waits until data is available.

In this project:

```text
ProcessingTask waits on sensorQueueHandle.
When SignalGeneratorTask sends SensorSample, ProcessingTask wakes up.
```

Simple explanation:

```text
The task sleeps until there is work to do.
```

---

## Why does ProcessingTask use a mutex?

`ProcessingTask` writes to:

```text
latestTelemetry
```

`CanTxTask` reads from:

```text
latestTelemetry
```

Since the data is shared, a mutex protects it.

Simple explanation:

```text
The mutex prevents simultaneous read/write access to shared telemetry.
```

---

## What could happen without a mutex?

Without a mutex, `CanTxTask` could read while `ProcessingTask` is halfway through updating the struct.

This could create mixed data.

Example:

```text
AIN1 from new sample
battery from old sample
counter from new sample
rpm from old sample
```

Simple explanation:

```text
Without a mutex, shared data can become inconsistent.
```

---

## Day 5 status

```text
Fault summary added.
Fault categories added.
FaultAnalyzer::print_summary() implemented.
ProcessingTask receives SensorSample from sensorQueueHandle.
ProcessingTask updates latestTelemetry.
latestTelemetry is protected with telemetryMutexHandle.
```

---

## Day 5 main interview idea

```text
A useful diagnostic system should summarize faults by category so a technician can quickly see the main problems. In the C++ decoder, FaultAnalyzer::print_summary() groups invalid DLC, unknown IDs, voltage faults, temperature faults, dropped frames, and possible stuck sensors. In the FreeRTOS firmware, ProcessingTask blocks on a queue, receives SensorSample messages, and updates latestTelemetry while holding a mutex so CanTxTask does not read inconsistent data.
```