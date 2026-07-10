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