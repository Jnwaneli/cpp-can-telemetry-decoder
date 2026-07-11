# Week 3 Notes

---

# Day 1 — Class Syntax

## Main goals

```text
Learn class syntax.
Understand public and private.
Create TelemetryDecoder.
Move decode_analog_inputs into TelemetryDecoder::decode_0x100.
Solve Valid Parentheses.
```

---

## What is a class?

A class groups related data and functions together.

Example:

```cpp
class TelemetryDecoder {
public:
    void decode(const CanFrame& frame);

private:
    void decode_0x100(const CanFrame& frame);
};
```

Simple explanation:

```text
A class is a custom type that groups behavior and data into one unit.
```

For this project, `TelemetryDecoder` groups the CAN decoding logic together.

---

## Why use public and private?

`public` means code outside the class can access it.

Example:

```cpp
decoder.decode(frame);
```

`private` means only the class itself can access it.

Example:

```cpp
void decode_0x100(const CanFrame& frame);
```

Simple explanation:

```text
public is the outside interface.
private is the internal implementation.
```

---

## Why should decoder logic live inside `TelemetryDecoder`?

The decoder logic should live inside `TelemetryDecoder` because decoding is its responsibility.

Before:

```text
main.cpp had too much logic.
```

After:

```text
main.cpp controls program flow.
TelemetryDecoder handles decoding.
```

This makes the project cleaner and easier to expand.

Later, the class can add:

```text
decode_0x101
decode_0x102
decode_0x200
scaling
status flags
fault analysis connection
```

Simple explanation:

```text
TelemetryDecoder keeps all CAN decoding behavior in one place instead of scattering it across main.cpp.
```

---

## `decode()` vs `decode_0x100()`

`decode()` is public.

It is the main function outside code should call.

```cpp
decoder.decode(frame);
```

`decode_0x100()` is private.

It is an internal helper for decoding analog input frames.

```cpp
void decode_0x100(const CanFrame& frame);
```

Simple explanation:

```text
decode() chooses what to decode.
decode_0x100() handles the specific details for CAN ID 0x100.
```

---

## Why make `decode_0x100()` private?

Outside code does not need to know the details of how ID `0x100` is decoded.

Outside code should only say:

```cpp
decoder.decode(frame);
```

The class decides internally what helper to use.

This is called encapsulation.

---

## What is encapsulation?

Encapsulation means hiding internal details and exposing a clean interface.

Simple explanation:

```text
The user of the class should know what the class does, not every detail of how it does it.
```

For this project:

```text
public decode() = clean interface
private decode_0x100() = hidden implementation detail
```

---

## Valid Parentheses

Pattern:

```text
stack
```

Main idea:

```text
Push opening brackets.
When a closing bracket appears, compare it with the most recent opening bracket.
If it does not match, return false.
At the end, the stack should be empty.
```

Opening brackets:

```text
(
[
{
```

Closing brackets:

```text
)
]
}
```

---

## Why does Valid Parentheses use a stack?

A stack works because parentheses must close in reverse order.

Example:

```text
[ { ( ) } ]
```

The last opening bracket is `(`, so the first closing bracket must be `)`.

This is Last-In, First-Out.

```text
LIFO = Last In, First Out
```

That is exactly how a stack works.

---

## Day 1 main interview idea

```text
A class groups related data and functions together. In this project, TelemetryDecoder owns the decoding logic, while main.cpp only controls the program flow. Public functions form the interface, and private functions hide internal implementation details.
```
---

# Day 2 — Constructors and Queue Using Stacks

## Main goals

```text
Learn constructors.
Add a constructor to TelemetryDecoder.
Initialize internal decoder state.
Understand FIFO and LIFO.
Solve Implement Queue using Stacks.
```

---

## What is a constructor?

A constructor is a special function that runs automatically when an object is created.

Example:

```cpp
class TelemetryDecoder {
public:
    TelemetryDecoder();
};
```

Implementation:

```cpp
TelemetryDecoder::TelemetryDecoder()
    : frames_seen_(0) {
}
```

Simple explanation:

```text
A constructor sets up an object when it is created.
```

---

## Why use a constructor?

A constructor is used to initialize internal state.

Example:

```cpp
std::size_t frames_seen_;
```

This variable should start at zero.

The constructor does that:

```cpp
TelemetryDecoder::TelemetryDecoder()
    : frames_seen_(0) {
}
```

Simple explanation:

```text
The constructor makes sure the object starts in a valid state.
```

---

## What is internal state?

Internal state means data stored inside an object.

For `TelemetryDecoder`, an example is:

```cpp
std::size_t frames_seen_;
```

This tracks how many frames the decoder has seen.

It is private because outside code should not directly modify it.

---

## Why make `frames_seen_` private?

`frames_seen_` belongs to the decoder.

Outside code should not be able to randomly change it.

Instead, outside code can read it through:

```cpp
std::size_t frames_seen() const;
```

Simple explanation:

```text
Private data protects the object from being changed incorrectly.
```

---

## What is FIFO?

FIFO means:

```text
First In, First Out
```

A queue uses FIFO behavior.

Example:

```text
1 enters first
2 enters second
3 enters third

Removal order:
1, then 2, then 3
```

Real examples:

```text
line at a store
UART receive queue
CAN receive queue
printer queue
```

---

## What is LIFO?

LIFO means:

```text
Last In, First Out
```

A stack uses LIFO behavior.

Example:

```text
1 pushed first
2 pushed second
3 pushed third

Removal order:
3, then 2, then 1
```

Real examples:

```text
stack of plates
function call stack
undo history
Valid Parentheses
```

---

## Queue using two stacks

A queue is FIFO.

A stack is LIFO.

To build a queue using stacks, use two stacks:

```text
input stack = receives pushed values
output stack = provides values in queue order
```

When `output` is empty, move everything from `input` to `output`.

This reverses the order.

Example:

```text
input stack has:
1, 2, 3

Move to output stack.

output top becomes:
1
```

Now the oldest value comes out first.

---

## Why does reversing help?

A stack naturally gives the newest item first.

But a queue needs the oldest item first.

Moving items from one stack to another reverses the order.

That makes the oldest item become the top of the output stack.

Simple explanation:

```text
Two LIFO reversals can produce FIFO behavior.
```

---

## Why does a decoder need routing logic?

The decoder needs routing logic because different CAN IDs mean different payload formats.

Example:

```text
0x100 = analog inputs
0x101 = battery and temperature
0x102 = status flags
0x200 = vehicle data
```

The decoder should inspect the CAN ID and send the frame to the correct helper function.

Example:

```cpp
void TelemetryDecoder::decode(const CanFrame& frame) {
    switch (frame.id) {
        case 0x100:
            decode_0x100(frame);
            break;

        default:
            std::cout << "Unsupported ID" << std::endl;
            break;
    }
}
```

Simple explanation:

```text
Routing logic lets one public decode() function decide which private decoder helper should handle each CAN ID.
```

---

## Day 2 main interview idea

```text
A constructor initializes an object when it is created. In this project, TelemetryDecoder uses a constructor to initialize internal state like frames_seen_. The public decode() function uses routing logic to choose the correct private helper based on CAN ID.
```