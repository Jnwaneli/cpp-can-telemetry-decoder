# Week 5 Notes

---

# Day 1 — Recursion, DecoderStats, and CAN Wiring Plan

## Main goals

```text
Learn recursion.
Understand base case and recursive case.
Understand the call stack.
Solve Maximum Depth of Binary Tree.
Add DecoderStats to the CAN decoder project.
Create CAN wiring plan documentation.
```

---

## Notes questions

```text
1. What is recursion?
2. What is a base case?
3. What is CANH/CANL?
4. Why does CAN need a transceiver?
5. Why does the PC need the USB-CAN adapter?
```

---

## What is recursion?

Recursion is when a function calls itself to solve a smaller version of the same problem.

Example:

```cpp
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
```

Simple explanation:

```text
Recursion means solving a problem by breaking it into smaller versions of itself.
```

---

## What is a base case?

A base case is the condition that stops recursion.

In factorial:

```cpp
if (n <= 1) return 1;
```

This stops the function from calling itself forever.

Simple explanation:

```text
The base case is the stopping point of recursion.
```

---

## What is a recursive case?

The recursive case is the part where the function calls itself.

In factorial:

```cpp
return n * factorial(n - 1);
```

This reduces the problem from `factorial(n)` to `factorial(n - 1)`.

Simple explanation:

```text
The recursive case keeps breaking the problem down.
```

---

## What is the call stack?

The call stack stores unfinished function calls.

For:

```cpp
factorial(4)
```

the calls build up like this:

```text
factorial(4)
factorial(3)
factorial(2)
factorial(1)
```

Then the base case is reached, and the calls return back up.

Simple explanation:

```text
The call stack keeps track of recursive calls that have not finished yet.
```

---

## Maximum Depth of Binary Tree

LeetCode 104 uses recursion.

Main idea:

```text
depth of empty tree = 0
depth of non-empty tree = 1 + max(left depth, right depth)
```

Code idea:

```cpp
int maxDepth(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    int left_depth = maxDepth(root->left);
    int right_depth = maxDepth(root->right);

    return 1 + max(left_depth, right_depth);
}
```

Simple explanation:

```text
The function finds the depth of each subtree, then adds 1 for the current node.
```

---

## What is DecoderStats?

`DecoderStats` tracks high-level statistics about the decoder run.

It tracks:

```text
total frames
valid frames
invalid DLC frames
unknown ID frames
fault count
```

Simple explanation:

```text
DecoderStats summarizes what happened while processing frames.
```

---

## Why add DecoderStats?

DecoderStats makes the tool feel more like a real diagnostic program.

Instead of only printing each frame, the project can summarize the whole run.

Example:

```text
Decoder Stats:
Total frames: 5
Valid frames: 3
Invalid DLC frames: 1
Unknown ID frames: 1
Fault count: 5
```

---

## What is CANH/CANL?

CANH and CANL are the two physical CAN bus wires.

```text
CANH = CAN high
CANL = CAN low
```

CAN uses the voltage difference between CANH and CANL to transmit data.

Simple explanation:

```text
CANH and CANL are the differential signal wires used by the CAN bus.
```

---

## Why does CAN need a transceiver?

The STM32 FDCAN peripheral uses digital TX/RX logic signals.

The CAN bus uses differential CANH/CANL electrical signals.

The transceiver converts between them.

```text
STM32 FDCAN TX/RX
        ↓
CAN transceiver
        ↓
CANH/CANL
```

Simple explanation:

```text
The transceiver converts microcontroller logic signals into physical CAN bus signals.
```

---

## Why does the PC need the USB-CAN adapter?

A normal PC does not directly understand CANH/CANL.

The USB-CAN adapter converts between:

```text
CAN bus messages
USB communication with the PC
```

This lets the PC send, receive, and log CAN frames.

Simple explanation:

```text
The USB-CAN adapter lets the PC communicate with the CAN bus.
```

---

## CAN wiring plan

Planned hardware chain:

```text
NUCLEO-G431RB FDCAN TX/RX
        ↓
SN65HVD230 TXD/RXD
        ↓
SN65HVD230 CANH/CANL
        ↓
CANH/CANL bus
        ↓
Waveshare USB-CAN
        ↓
PC
```

Important wiring items:

```text
CANH
CANL
GND
3.3 V
120 ohm termination
bitrate
common ground
```

---

## Why common ground matters

The STM32 board, transceiver, and USB-CAN adapter should share a common ground.

This gives the devices a shared electrical reference.

Simple explanation:

```text
Common ground helps the devices agree on voltage levels.
```

---

## Why termination matters

A CAN bus normally needs 120 ohm termination at both ends.

Termination helps reduce signal reflections on the bus.

Simple explanation:

```text
Termination helps the CAN signal stay clean.
```

---

## Day 1 main interview idea

```text
Recursion solves problems by breaking them into smaller versions of the same problem until a base case is reached. In the CAN project, DecoderStats summarizes frame processing results, while the CAN wiring plan explains how STM32 FDCAN logic connects through a transceiver to the physical CAN bus and then to the PC through a USB-CAN adapter.
```