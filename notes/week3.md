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
---

# Day 3 — std::array vs Raw Array

## Main goals

```text
Compare raw arrays with std::array.
Understand why embedded structs often use raw arrays.
Understand what std::array adds in C++.
Keep CanFrame payload as uint8_t data[8].
Solve Implement Stack using Queues.
```

---

## Raw array

A raw array is the C-style fixed-size array.

Example:

```cpp
std::uint8_t data[8];
```

This creates 8 bytes of storage.

For a CAN frame, this makes sense because a classic CAN payload can hold up to 8 data bytes.

Simple explanation:

```text
A raw array is a fixed-size block of elements.
```

---

## std::array

`std::array` is a C++ fixed-size array wrapper.

Example:

```cpp
std::array<std::uint8_t, 8> data;
```

It still has fixed size, but it gives extra C++ features.

Useful features:

```text
.size()
.at()
.begin()
.end()
works better with standard algorithms
```

Simple explanation:

```text
std::array is a safer C++ wrapper around a fixed-size array.
```

---

## Raw array vs std::array

Raw array:

```text
C-style
common in embedded code
simple memory layout
no .size() member function
can decay to pointer
```

`std::array`:

```text
C++ style
fixed-size
has .size()
supports iterators
works well with STL
does not decay to pointer as easily
```

---

## Why are fixed-size arrays useful in embedded systems?

Fixed-size arrays are useful because embedded systems usually have limited memory and need predictable behavior.

Benefits:

```text
fixed memory usage
no heap allocation
predictable size
simple layout
good for hardware data buffers
good for protocols like CAN, UART, SPI, and I2C
```

Simple explanation:

```text
Fixed-size arrays are useful in embedded systems because they avoid dynamic memory and make memory usage predictable.
```

---

## Why is `uint8_t data[8]` useful for CAN?

CAN data is byte-based.

Classic CAN payloads are up to 8 bytes.

So this struct:

```cpp
struct CanFrame {
    std::uint32_t id;
    std::uint8_t dlc;
    std::uint8_t data[8];
};
```

matches the idea of a real CAN frame.

Simple explanation:

```text
uint8_t data[8] clearly represents the 8-byte payload of a classic CAN frame.
```

---

## What does std::array add?

`std::array` adds useful C++ behavior while still keeping fixed size.

Example:

```cpp
std::array<std::uint8_t, 8> data = {
    0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05
};

std::cout << data.size() << std::endl;
```

A raw array does not have:

```cpp
data.size()
```

For a raw array, size is usually calculated with:

```cpp
sizeof(data) / sizeof(data[0])
```

---

## Why keep `uint8_t data[8]` in `CanFrame`?

We keep `uint8_t data[8]` because it looks closer to embedded firmware and CAN driver code.

Many embedded examples use raw byte arrays for protocol payloads.

Simple explanation:

```text
I keep uint8_t data[8] in CanFrame because it directly represents an 8-byte CAN payload and matches embedded firmware style.
```

---

## Stack using Queues

A stack uses LIFO behavior.

```text
LIFO = Last In, First Out
```

A queue uses FIFO behavior.

```text
FIFO = First In, First Out
```

The problem asks us to build stack behavior using queue operations.

---

## Main idea for Stack using Queues

Use one queue.

When pushing a new value:

```text
push the new value
rotate all older values behind it
```

Example:

```text
push 1:
queue = 1

push 2:
queue before rotation = 1, 2
queue after rotation = 2, 1

push 3:
queue before rotation = 2, 1, 3
queue after rotation = 3, 2, 1
```

Now the newest item is always at the front.

So:

```text
pop removes the newest item
top returns the newest item
```

That creates stack behavior.

---

## What did Stack using Queues teach me?

It taught that data structures are mostly about access order.

A stack and a queue can store similar data, but they remove data in different orders.

```text
stack = newest item first
queue = oldest item first
```

To make a queue act like a stack, I had to rotate the queue so the newest item becomes the first item removed.

Simple explanation:

```text
Stack using Queues taught me that I can change access behavior by rearranging the order of stored elements.
```

---

## Day 3 main interview idea

```text
Fixed-size arrays are useful in embedded systems because they provide predictable memory usage. std::array adds safer C++ features like .size(), but uint8_t data[8] is still common for embedded protocol payloads like CAN frames because it directly represents a fixed 8-byte message buffer.
```
---

# Day 4 — Min Stack and Output Formatting

## Main goals

```text
Study common stack use cases.
Solve Min Stack.
Improve CAN decoder output formatting.
Make project output easier to read like a debugging tool.
```

---

## Stack use cases

A stack uses LIFO behavior.

```text
LIFO = Last In, First Out
```

Common stack use cases:

```text
parsing
undo history
function calls
monotonic stack
nested structures
backtracking
expression evaluation
```

---

## Stack use case: parsing

Stacks are useful for parsing nested structures.

Example:

```text
({[]})
```

The most recent opening bracket must be closed first.

This is why Valid Parentheses uses a stack.

---

## Stack use case: undo history

Undo history works like a stack.

The most recent action is the first action undone.

Example:

```text
type word
delete word
paste text

undo removes paste text first
```

---

## Stack use case: function calls

The function call stack also uses stack behavior.

When a function calls another function, the new function goes on top of the call stack.

When that function finishes, it is removed from the stack and control returns to the previous function.

This is why recursion uses stack memory.

---

## Stack use case: monotonic stack

A monotonic stack keeps values in increasing or decreasing order.

It is useful for problems where I need to find the next greater or smaller value.

Example problem:

```text
Daily Temperatures
```

---

## Stack use case: nested structures

Stacks are useful when the most recent unfinished item must be handled first.

Examples:

```text
parentheses
HTML tags
function calls
nested expressions
compiler parsing
```

---

## Why does formatting matter for debugging tools?

Formatting matters because debugging output needs to be readable quickly.

A good debugging log should make it easy to see:

```text
which frame was received
what type of frame it is
what raw values were decoded
whether the frame passed validation
what fault occurred if something failed
```

Bad formatting makes debugging slower because the developer has to mentally organize messy output.

Simple explanation:

```text
Formatting matters because clear logs help me find problems faster.
```

---

## Improved CAN decoder output

Example output:

```text
Frame ID: 0x100
DLC: 8
Payload: 0x00 0x08 0x10 0x00 0xff 0x0a 0x01 0x05
Type: Analog Inputs
AIN1_RAW: 2048
AIN2_RAW: 16
AIN3_RAW: 2815
Status: 0x01
Counter: 5
Result: OK
```

This is better than random print statements because it looks like a structured diagnostic report.

---

## What is Min Stack's trick?

Min Stack needs to support:

```text
push
pop
top
getMin
```

efficiently.

The trick is to use two stacks:

```text
values stack = stores all values
mins stack = stores the minimum values
```

The minimum stack tracks the minimum value so far.

This allows `getMin()` to run in:

```text
O(1)
```

instead of scanning the whole stack.

---

## Why use <= when pushing to the min stack?

Use `<=` to handle duplicate minimum values.

Example:

```text
push 2
push 2
pop
```

If both `2`s are minimum values, the min stack needs to track both of them.

Otherwise, popping one duplicate could incorrectly remove the minimum too early.

---

## Min Stack time complexity

```text
push: O(1)
pop: O(1)
top: O(1)
getMin: O(1)
```

The whole point is that `getMin()` should be constant time.

---

## How would I explain this project output in an interview?

I would say:

```text
My decoder takes simulated CAN frames, validates the CAN ID and DLC, routes known messages to decoder functions, and formats the result like a diagnostic log. For ID 0x100, it decodes three analog raw values, a status byte, and a counter. The formatted output makes it easier to debug frame contents and fault conditions.
```

---

## Day 4 main interview idea

```text
Clear formatting makes debugging tools easier to use. Min Stack uses an extra stack to track the current minimum, which allows getMin() to run in O(1) time instead of scanning the whole stack.
```
---

# Day 5 — Decode 0x101 and Monotonic Stack

## Main goals

```text
Study monotonic stack.
Solve Daily Temperatures.
Add decoder support for CAN ID 0x101.
Decode battery_mV and temperature_deciC.
Scale raw values into volts and Celsius.
```

---

## CAN ID 0x101 format

Frame `0x101` represents battery and temperature data.

Payload format:

```text
Byte 0-1: battery_mV
Byte 2-3: temperature_deciC
Byte 4-7: reserved
```

Example frame:

```cpp
{0x101, 8, {0x88, 0x13, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}}
```

---

## Decoding battery_mV

Battery voltage is stored in millivolts.

Code:

```cpp
std::uint16_t battery_mV = pack_u16(frame.data[0], frame.data[1]);
```

For the sample frame:

```text
data[0] = 0x88
data[1] = 0x13

battery_mV = 0x1388 = 5000
```

---

## How do I scale raw battery_mV to volts?

To convert millivolts to volts, divide by `1000.0`.

Formula:

```text
battery_V = battery_mV / 1000.0
```

Example:

```text
5000 mV / 1000.0 = 5.00 V
12000 mV / 1000.0 = 12.00 V
```

Simple explanation:

```text
Millivolts are one-thousandth of a volt, so divide by 1000 to convert mV to V.
```

---

## Decoding temperature_deciC

Temperature is stored in deci-degrees Celsius.

Code:

```cpp
std::uint16_t temperature_deciC = pack_u16(frame.data[2], frame.data[3]);
```

For the sample frame:

```text
data[2] = 0x2C
data[3] = 0x01

temperature_deciC = 0x012C = 300
```

---

## How do I scale temperature_deciC to Celsius?

To convert deci-degrees Celsius to Celsius, divide by `10.0`.

Formula:

```text
temperature_C = temperature_deciC / 10.0
```

Example:

```text
300 deciC / 10.0 = 30.0 C
805 deciC / 10.0 = 80.5 C
```

Simple explanation:

```text
DeciC means tenths of a degree Celsius, so divide by 10 to get Celsius.
```

---

## Why bytes 4-7 are reserved

Bytes 4-7 are reserved for future use.

That means they are not decoded yet.

They could later store:

```text
battery status
temperature sensor status
charging state
fault flags
extra sensor data
```

Reserved bytes make the protocol easier to expand later.

---

## What is a monotonic stack?

A monotonic stack is a stack that keeps values in a controlled order.

It is usually either:

```text
monotonic increasing
monotonic decreasing
```

It is useful for problems involving:

```text
next greater value
next smaller value
previous greater value
previous smaller value
```

Simple explanation:

```text
A monotonic stack stores unresolved values in an order that helps answer next greater or next smaller questions efficiently.
```

---

## Daily Temperatures pattern

Daily Temperatures asks:

```text
For each day, how many days until a warmer temperature?
```

Pattern:

```text
stack stores indexes
current temperature checks previous unresolved days
when current temperature is warmer, resolve previous days
```

---

## Why store indexes instead of temperatures?

The answer needs the number of days waited.

Code:

```cpp
answer[previous_index] = i - previous_index;
```

To calculate that difference, I need the index.

If I only store the temperature value, I lose the day position.

Simple explanation:

```text
Daily Temperatures stores indexes because the answer depends on distance between days, not just temperature values.
```

---

## Why is Daily Temperatures not just a normal stack problem?

A normal stack only gives last-in, first-out behavior.

Daily Temperatures uses a monotonic stack because the stack holds unresolved days in a useful order.

When a warmer day appears, it can resolve multiple previous colder days.

Simple explanation:

```text
Daily Temperatures is not just a normal stack problem because the stack is used to maintain unresolved temperatures in an order that helps find the next warmer day efficiently.
```

---

## Daily Temperatures time complexity

Each index is pushed once and popped once.

So the time complexity is:

```text
O(n)
```

The space complexity is:

```text
O(n)
```

---

## Day 5 main interview idea

```text
CAN ID 0x101 stores battery voltage in millivolts and temperature in deci-degrees Celsius. The decoder combines little-endian byte pairs into raw values, then scales them to volts and Celsius. Daily Temperatures uses a monotonic stack of indexes to efficiently resolve previous days when a warmer temperature appears.
```
---

# Day 6 — unordered_map and Scaled Output

## Main goals

```text
Learn std::unordered_map.
Understand key-value storage.
Understand average O(1) lookup.
Use unordered_map to map CAN IDs to readable names.
Improve scaled 0x101 output.
Finish Week 3 integration.
```

---

## What is std::unordered_map?

`std::unordered_map` stores key-value pairs.

Example:

```cpp
std::unordered_map<std::uint32_t, std::string> frame_names = {
    {0x100, "Analog Inputs"},
    {0x101, "Battery and Temperature"},
    {0x102, "Status Flags"},
    {0x200, "Vehicle Telemetry"}
};
```

In this example:

```text
key = CAN ID
value = frame name
```

Simple explanation:

```text
std::unordered_map lets me quickly look up a value using a key.
```

---

## Why is unordered_map useful?

`std::unordered_map` is useful for:

```text
frequency counting
lookup tables
mapping IDs to names
caching results
checking if a key exists
```

In this project, it maps CAN IDs to readable names.

Example:

```text
0x101 -> Battery and Temperature
```

---

## Average O(1) lookup

`std::unordered_map` has average O(1) lookup.

That means looking up a key is usually constant time.

Example:

```cpp
auto it = frame_names.find(id);
```

Simple explanation:

```text
unordered_map is usually fast because it uses hashing instead of searching one item at a time.
```

---

## unordered_map vs vector search

With a vector, I might have to check each item one by one.

That is O(n).

With an unordered_map, I can usually look up the key directly.

That is average O(1).

Simple comparison:

```text
vector search = check many items
unordered_map lookup = jump close to the answer using a hash
```

---

## Top K Frequent Elements

Top K Frequent Elements uses a frequency map.

Example:

```text
nums = [1, 1, 1, 2, 2, 3]
k = 2
```

Frequency map:

```text
1 -> 3
2 -> 2
3 -> 1
```

Top 2 frequent:

```text
1, 2
```

The main pattern is:

```text
count frequencies with unordered_map
rank by frequency
return the top k keys
```

---

## Scaled 0x101 output

CAN ID `0x101` stores:

```text
Byte 0-1: battery_mV
Byte 2-3: temperature_deciC
Byte 4-7: reserved
```

The decoder first combines bytes into raw values:

```cpp
std::uint16_t battery_mV = pack_u16(frame.data[0], frame.data[1]);
std::uint16_t temperature_deciC = pack_u16(frame.data[2], frame.data[3]);
```

Then it scales them:

```cpp
double battery_V = battery_mV / 1000.0;
double temperature_C = temperature_deciC / 10.0;
```

---

## How does 0x101 scaling work?

`battery_mV` is stored in millivolts.

To convert millivolts to volts:

```text
battery_V = battery_mV / 1000.0
```

Example:

```text
12600 mV = 12.60 V
```

`temperature_deciC` is stored in tenths of a degree Celsius.

To convert deciC to Celsius:

```text
temperature_C = temperature_deciC / 10.0
```

Example:

```text
345 deciC = 34.5 C
```

Simple explanation:

```text
0x101 scaling converts raw integer protocol values into real-world units.
```

---

## Why print scaled values?

Raw values are useful for debugging the protocol.

Scaled values are easier for humans to understand.

Example:

```text
battery_mV = 12600
Battery = 12.60 V
```

The scaled value is what an engineer would usually want to read quickly.

---

## Why might embedded teams limit STL usage?

Embedded teams may limit STL usage because some STL features can involve:

```text
dynamic memory allocation
larger code size
less predictable timing
exceptions
hidden runtime behavior
```

This does not mean STL is bad.

It means embedded teams choose carefully depending on the system constraints.

Simple explanation:

```text
Embedded teams may limit STL usage when memory, timing, and code size need to be tightly controlled.
```

---

## Week 3 deliverable summary

By the end of Week 3, the project has:

```text
TelemetryDecoder class
constructor
private decode helpers
decode_0x100
decode_0x101
formatted output
raw-to-scaled battery output
raw-to-scaled temperature output
unordered_map CAN ID names
```

---

## Day 6 main interview idea

```text
std::unordered_map stores key-value pairs and provides average O(1) lookup. In this project, I used it to map CAN IDs to readable frame names. I also improved 0x101 decoding by scaling battery millivolts to volts and temperature deciC to Celsius, making the output more useful as a diagnostic log.
```