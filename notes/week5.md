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
---

# Day 2 — Invert Tree and Wire CAN Bridge

## Main goals

```text
Draw recursion stack diagrams.
Solve Invert Binary Tree.
Confirm DecoderStats increments during frame processing.
Wire the CAN bridge between NUCLEO, SN65HVD230, Waveshare USB-CAN, and PC.
Use a multimeter to check wiring.
```

---

## Notes questions

```text
1. What did Invert Tree teach me about recursion?
2. What is termination?
3. Why must CAN nodes share ground?
4. What wiring mistake would break CAN?
```

---

## What did Invert Tree teach me about recursion?

Invert Tree teaches that recursion works well on tree structures because each subtree is a smaller version of the whole tree.

The function recursively inverts the left subtree and right subtree, then swaps them.

Simple explanation:

```text
A tree is naturally recursive because every child is the root of a smaller tree.
```

---

## Invert Tree recursion pattern

LeetCode 226 uses this pattern:

```cpp
TreeNode* invertTree(TreeNode* root) {
    if (root == nullptr) {
        return nullptr;
    }

    TreeNode* left_subtree = invertTree(root->left);
    TreeNode* right_subtree = invertTree(root->right);

    root->left = right_subtree;
    root->right = left_subtree;

    return root;
}
```

The base case is:

```cpp
if (root == nullptr) return nullptr;
```

The recursive case is:

```cpp
invertTree(root->left);
invertTree(root->right);
```

---

## Recursion stack diagram

Example tree:

```text
    A
   / \
  B   C
```

Call stack idea:

```text
invertTree(A)
    invertTree(B)
        invertTree(nullptr)
        invertTree(nullptr)
        swap B's children
    invertTree(C)
        invertTree(nullptr)
        invertTree(nullptr)
        swap C's children
    swap A's children
```

Simple explanation:

```text
The recursive calls go down the tree first, then swaps happen as the calls return.
```

---

## DecoderStats increments

The project now increments stats during frame processing.

Conceptually:

```cpp
stats.record_frame_received();

if (!is_known_id(frame.id)) {
    stats.record_unknown_id();
}

if (!has_valid_dlc(frame)) {
    stats.record_invalid_dlc();
}
```

The project uses member functions instead of direct public increments because the stats fields are private.

Simple explanation:

```text
DecoderStats records what happens to each frame as it is processed.
```

---

## Current DecoderStats fields

DecoderStats tracks:

```text
total frames
valid frames
invalid DLC frames
unknown ID frames
fault count
```

Example output:

```text
Decoder Stats:
Total frames: 5
Valid frames: 3
Invalid DLC frames: 1
Unknown ID frames: 1
Fault count: 5
```

---

## What is termination?

Termination means placing resistors across CANH and CANL at the ends of the CAN bus.

The typical value is:

```text
120 ohms at each end
```

If two 120 ohm terminations are installed, measuring CANH to CANL with power off should show about:

```text
60 ohms
```

Simple explanation:

```text
Termination helps prevent signal reflections on the CAN bus.
```

---

## Why must CAN nodes share ground?

CAN uses differential signaling between CANH and CANL, but the devices still need a shared electrical reference.

The NUCLEO, SN65HVD230, and USB-CAN adapter should share ground.

```text
NUCLEO GND
SN65HVD230 GND
USB-CAN GND
```

Simple explanation:

```text
Common ground helps all devices agree on voltage levels.
```

---

## CAN bridge wiring

Planned wiring:

```text
NUCLEO FDCAN_TX -> SN65HVD230 TXD
NUCLEO FDCAN_RX <- SN65HVD230 RXD
NUCLEO 3.3 V    -> SN65HVD230 VCC
NUCLEO GND      -> SN65HVD230 GND

SN65HVD230 CANH -> CANH bus wire
SN65HVD230 CANL -> CANL bus wire

CANH bus wire   -> Waveshare CANH
CANL bus wire   -> Waveshare CANL
GND             -> Waveshare GND
```

---

## Multimeter checks

Before running CAN traffic, check:

```text
CANH continuity
CANL continuity
GND continuity
3.3 V at the transceiver VCC pin
termination resistance across CANH and CANL
```

Expected termination readings with power off:

```text
two 120 ohm terminations = about 60 ohms
one 120 ohm termination = about 120 ohms
no termination = open or very high resistance
```

---

## What wiring mistake would break CAN?

Common mistakes include:

```text
CANH and CANL swapped
missing common ground
missing termination
too much termination
wrong bitrate
transceiver not powered
wrong FDCAN pins selected
connecting STM32 TX/RX directly to CANH/CANL
```

Simple explanation:

```text
CAN needs correct differential wiring, common ground, termination, matching bitrate, and a powered transceiver.
```

---

## Day 2 main interview idea

```text
Invert Tree shows that recursion works well when a structure is made of smaller versions of itself. For the CAN hardware, the STM32 does not connect directly to CANH/CANL. It uses a transceiver, shared ground, matching bitrate, and proper termination to communicate with a USB-CAN adapter and PC.
``` 
---

# Day 3 — Same Tree, Symmetric Tree, and STM32 FDCAN Configuration

## Main goals

```text
Compare trees recursively.
Solve Same Tree.
Solve Symmetric Tree.
Confirm battery voltage fault rules.
Configure STM32 FDCAN to send CAN frame 0x100.
```

---

## Notes questions

```text
1. How does recursive comparison work?
2. What is FDCAN/CAN configuration?
3. What bitrate am I using?
4. What frame ID am I sending first?
```

---

## How does recursive comparison work?

Recursive comparison checks two nodes at the same time.

The logic is:

```text
if both nodes are null, they match
if only one node is null, they do not match
if values are different, they do not match
otherwise compare the child nodes recursively
```

Simple explanation:

```text
Recursive comparison breaks a tree comparison into smaller node comparisons.
```

---

## Same Tree

Same Tree compares two trees in the same direction.

It checks:

```text
p node vs q node
p left vs q left
p right vs q right
```

Code idea:

```cpp
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (p == nullptr && q == nullptr) {
        return true;
    }

    if (p == nullptr || q == nullptr) {
        return false;
    }

    if (p->val != q->val) {
        return false;
    }

    return isSameTree(p->left, q->left) &&
           isSameTree(p->right, q->right);
}
```

Simple explanation:

```text
Same Tree compares matching positions in both trees.
```

---

## Symmetric Tree

Symmetric Tree compares the left side of a tree against the right side as a mirror.

It checks:

```text
left node vs right node
left outer child vs right outer child
left inner child vs right inner child
```

Code idea:

```cpp
bool isMirror(TreeNode* left, TreeNode* right) {
    if (left == nullptr && right == nullptr) {
        return true;
    }

    if (left == nullptr || right == nullptr) {
        return false;
    }

    if (left->val != right->val) {
        return false;
    }

    return isMirror(left->left, right->right) &&
           isMirror(left->right, right->left);
}
```

Simple explanation:

```text
Symmetric Tree compares opposite sides of the tree like a mirror.
```

---

## Difference between Same Tree and Symmetric Tree

Same Tree compares:

```text
left with left
right with right
```

Symmetric Tree compares:

```text
left with right
right with left
```

Simple explanation:

```text
Same Tree checks identical structure, while Symmetric Tree checks mirror structure.
```

---

## Battery voltage faults

The `FaultAnalyzer` checks battery voltage after `0x101` is decoded.

Rules:

```text
battery < 10.5 V = low voltage fault
battery > 14.8 V = high voltage fault
```

Output examples:

```text
FAULT: Battery voltage too low
FAULT: Battery voltage too high
```

Simple explanation:

```text
The decoder converts battery_mV to volts, then FaultAnalyzer checks the voltage limits.
```

---

## What is FDCAN/CAN configuration?

FDCAN/CAN configuration means setting up the STM32 CAN peripheral so it can transmit and receive CAN frames correctly.

Configuration includes:

```text
FDCAN pins
bitrate
frame format
standard or extended ID
DLC
transmit mode
filter settings
```

Simple explanation:

```text
CAN configuration makes sure the STM32 and USB-CAN adapter speak the same CAN format.
```

---

## What bitrate am I using?

The planned bitrate is:

```text
500 kbps
```

Both sides must match:

```text
STM32 FDCAN = 500 kbps
Waveshare USB-CAN = 500 kbps
```

Simple explanation:

```text
If the bitrates do not match, the PC will not correctly receive the frames.
```

---

## What frame ID am I sending first?

The first transmitted frame ID is:

```text
0x100
```

Payload:

```text
00 08 10 00 FF 0A 07 01
```

Meaning:

```text
AIN1_RAW = 2048
AIN2_RAW = 16
AIN3_RAW = 2815
Status = 0x07
Counter = 1
```

Simple explanation:

```text
The first hardware test sends the same kind of analog input frame that the desktop decoder already understands.
```

---

## Day 3 main interview idea

```text
Recursive comparison works by checking matching or mirrored nodes and then applying the same logic to smaller subtrees. On the hardware side, STM32 FDCAN must be configured with the correct pins, bitrate, frame type, ID, and payload format so it can transmit frame 0x100 through the CAN transceiver to the USB-CAN adapter.
```
---

# Day 4 — BFS and Send First CAN Frame

## Main goals

```text
Learn BFS with a queue.
Solve Binary Tree Level Order Traversal.
Confirm temperature high fault rule.
Transmit CAN frame 0x100 from STM32 every 100 ms.
Try receiving the frame on the PC through Waveshare USB-CAN.
```

---

## Notes questions

```text
1. What is BFS?
2. Why does BFS use a queue?
3. What did I transmit over CAN?
4. Did the PC receive it?
5. If not, what did I check?
```

---

## What is BFS?

BFS means Breadth-First Search.

BFS visits nodes level by level.

For a binary tree:

```text
Level 0: root
Level 1: root's children
Level 2: grandchildren
```

Simple explanation:

```text
BFS explores everything at the current level before moving deeper.
```

---

## Why does BFS use a queue?

BFS uses a queue because a queue is first in, first out.

That means the first node discovered is the first node processed.

This keeps traversal level-by-level.

Simple explanation:

```text
A queue preserves the order nodes are discovered, which is what BFS needs.
```

---

## Binary Tree Level Order Traversal

LeetCode 102 uses BFS.

Code idea:

```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;

    if (root == nullptr) {
        return result;
    }

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int level_size = q.size();
        vector<int> level;

        for (int i = 0; i < level_size; i++) {
            TreeNode* current = q.front();
            q.pop();

            level.push_back(current->val);

            if (current->left != nullptr) {
                q.push(current->left);
            }

            if (current->right != nullptr) {
                q.push(current->right);
            }
        }

        result.push_back(level);
    }

    return result;
}
```

The important line is:

```cpp
int level_size = q.size();
```

That captures how many nodes are in the current level before adding the next level.

---

## Temperature high fault

The `FaultAnalyzer` checks decoded temperature after CAN ID `0x101` is decoded.

Rule:

```text
temperature > 80 C = high temperature fault
```

Output:

```text
FAULT: Temperature too high
```

Simple explanation:

```text
The decoder converts temperature_deciC to Celsius, then FaultAnalyzer checks if it is above 80 C.
```

---

## What did I transmit over CAN?

The STM32 transmitted a standard CAN frame.

```text
ID: 0x100
DLC: 8
Data: 00 08 10 00 FF 0A 07 01
```

Decoded meaning:

```text
AIN1_RAW = 2048
AIN2_RAW = 16
AIN3_RAW = 2815
Status = 0x07
Counter = 1
```

Simple explanation:

```text
I transmitted the analog input frame that the desktop decoder already understands.
```

---

## CAN transmit period

The STM32 sends the frame every:

```text
100 ms
```

This means the PC should receive about:

```text
10 frames per second
```

Simple explanation:

```text
A 100 ms delay creates a repeating CAN test frame.
```

---

## Did the PC receive it?

The expected receive frame is:

```text
0x100   8   00 08 10 00 FF 0A 07 01
```

Record the result:

```text
PC receive result:
[Write YES or NO here]
```

If the frame appears repeatedly in the Waveshare USB-CAN receive window, the basic CAN transmit path is working.

---

## If not, what did I check?

If the PC did not receive the CAN frame, check:

```text
STM32 bitrate
Waveshare bitrate
FDCAN_TX to TXD
FDCAN_RX to RXD
CANH to CANH
CANL to CANL
common ground
3.3 V power to transceiver
termination across CANH/CANL
CANH/CANL swapped
transceiver standby/silent mode
whether STM32 code reaches the while loop
```

Simple explanation:

```text
CAN receive problems usually come from bitrate mismatch, wiring mistakes, missing ground, missing termination, or transceiver configuration.
```

---

## Day 4 main interview idea

```text
BFS uses a queue to process nodes in the order they are discovered, which allows level-by-level traversal. On the hardware side, the STM32 sends standard CAN frame 0x100 every 100 ms through the transceiver, and the PC should receive it through the Waveshare USB-CAN adapter if bitrate, wiring, ground, and termination are correct.
```