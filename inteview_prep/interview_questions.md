# C++ / Embedded Interview Questions

## How to use this file

Read the question first. Try to answer out loud before looking at the reference answer.

---

# Week 1 — C++ Basics and CAN Foundations

## 1. What is a C++ function?

My answer:









Reference answer:

A function is a reusable block of code that performs a specific task.

Example:

```cpp
int add(int a, int b) {
    return a + b;
}
```

This function takes two integers and returns their sum.

---

## 2. What does `int main()` mean?

My answer:









Reference answer:

`main()` is the starting point of a C++ program.

```cpp
int main() {
    return 0;
}
```

The `int` means the function returns an integer. Returning `0` usually means the program ended successfully.

---

## 3. What is `std::cout` used for?

My answer:









Reference answer:

`std::cout` prints output to the terminal.

Example:

```cpp
std::cout << "Hello, C++" << std::endl;
```

It is commonly used for console output and debugging.

---

## 4. What is `std::vector`?

My answer:









Reference answer:

`std::vector` is a dynamic array from the C++ standard library.

Example:

```cpp
std::vector<int> nums = {1, 2, 3};
```

Unlike a normal array, a vector can grow or shrink.

---

## 5. What does `const std::vector<int>& nums` mean?

My answer:









Reference answer:

```cpp
const std::vector<int>& nums
```

means the function receives the original vector without copying it, but cannot modify it.

Breakdown:

```text
const = do not modify
std::vector<int> = vector of integers
& = pass by reference
nums = parameter name
```

Simple version:

```text
Use const reference when reading a large object without copying it.
```

---

## 6. What is the difference between a regular for-loop and a range-based for-loop?

My answer:









Reference answer:

A regular for-loop gives access to the index.

```cpp
for (int i = 0; i < nums.size(); i++) {
    std::cout << nums[i] << std::endl;
}
```

A range-based loop directly gives each value.

```cpp
for (int value : nums) {
    std::cout << value << std::endl;
}
```

Use a regular loop when you need the index. Use a range-based loop when you only need the values.

---

## 7. What is a struct?

My answer:









Reference answer:

A struct groups related data together.

Example:

```cpp
struct CanFrame {
    std::uint32_t id;
    std::uint8_t dlc;
    std::array<std::uint8_t, 8> data;
};
```

Simple version:

```text
A struct is a custom data type that holds related variables in one object.
```

---

## 8. What is a CAN frame?

My answer:









Reference answer:

A CAN frame is a message sent on a CAN bus.

For this project, a CAN frame has:

```text
CAN ID
DLC
data bytes
```

Example:

```cpp
struct CanFrame {
    std::uint32_t id;
    std::uint8_t dlc;
    std::array<std::uint8_t, 8> data;
};
```

---

## 9. What is a CAN ID?

My answer:









Reference answer:

The CAN ID identifies what type of message the frame is.

Example:

```text
0x100 = analog input data
0x101 = battery/temperature data
0x102 = status flags
0x200 = vehicle telemetry
```

The decoder uses the CAN ID to know how to interpret the payload.

---

## 10. What is DLC?

My answer:









Reference answer:

DLC means Data Length Code.

It tells how many data bytes are valid in the CAN frame.

Example:

```text
DLC = 8 means the frame has 8 valid data bytes.
```

---

## 11. Why use fixed-width integer types like `uint8_t` and `uint32_t`?

My answer:









Reference answer:

Fixed-width integer types make the data size explicit.

Examples:

```cpp
std::uint8_t   // exactly 8 bits
std::uint16_t  // exactly 16 bits
std::uint32_t  // exactly 32 bits
```

This is useful in embedded systems because hardware registers, CAN payloads, and protocol fields often require exact sizes.

---

## 12. What is a byte?

My answer:









Reference answer:

A byte is 8 bits.

Example:

```text
0b10101010
```

In CAN, each data element in the payload is one byte.

---

## 13. What is hex?

My answer:









Reference answer:

Hexadecimal is base 16.

It uses:

```text
0-9 and A-F
```

Example:

```text
0x0A = 10 decimal
0xFF = 255 decimal
```

Hex is common in embedded systems because it represents binary data compactly.

---

## 14. What is binary?

My answer:









Reference answer:

Binary is base 2.

It uses only:

```text
0 and 1
```

Example:

```text
0b1010 = 10 decimal
```

Binary is useful because computers and microcontrollers store data as bits.

---

## 15. What is the relationship between hex and binary?

My answer:









Reference answer:

One hex digit represents 4 bits.

```text
1 hex digit = 4 bits
2 hex digits = 1 byte
```

Example:

```text
0xFF = 11111111 binary = 255 decimal
```

---

## 16. What does `data[0] | (data[1] << 8)` do?

My answer:









Reference answer:

It combines two 8-bit bytes into one 16-bit value.

```cpp
data[0] | (data[1] << 8)
```

Meaning:

```text
data[0] is the low byte
data[1] is the high byte
data[1] is shifted left by 8 bits
bitwise OR combines both bytes
```

Example:

```text
data[0] = 0x34
data[1] = 0x12

result = 0x1234
```

---

## 17. What is little-endian parsing?

My answer:









Reference answer:

Little-endian means the low byte comes first.

Example:

```text
data[0] = low byte
data[1] = high byte
```

So if the bytes are:

```text
0x34, 0x12
```

the 16-bit value is:

```text
0x1234
```

---

## 18. What is pass by value?

My answer:









Reference answer:

Pass by value copies the argument into the function.

Example:

```cpp
void function(std::vector<int> nums)
```

Changes inside the function do not affect the original vector.

---

## 19. What is pass by reference?

My answer:









Reference answer:

Pass by reference gives the function access to the original object.

Example:

```cpp
void function(std::vector<int>& nums)
```

Changes inside the function affect the original vector.

---

## 20. What is pass by const reference?

My answer:









Reference answer:

Pass by const reference gives the function read-only access to the original object without copying it.

Example:

```cpp
void function(const std::vector<int>& nums)
```

Use this when the function only needs to read a large object.

---

## 21. Why use `const CanFrame& frame`?

My answer:









Reference answer:

Use `const CanFrame&` to avoid copying the frame and prevent accidental modification.

Simple version:

```text
The function only needs to read the CAN frame, so passing by const reference is efficient and safe.
```

---

## 22. What is `std::unordered_map` useful for?

My answer:









Reference answer:

`std::unordered_map` stores key-value pairs and gives fast average lookups.

Example:

```cpp
std::unordered_map<char, int> frequency;
frequency['a']++;
```

It is useful for frequency counting, lookup tables, and problems like Two Sum or Valid Anagram.

---

## 23. What is `std::unordered_set` useful for?

My answer:









Reference answer:

`std::unordered_set` stores unique values and allows fast average lookup.

Example:

```cpp
std::unordered_set<int> seen;
seen.insert(5);
```

It is useful when you need to know whether something has already appeared.

---

## 24. What is the pattern for Two Sum?

My answer:









Reference answer:

Pattern:

```text
Hash map lookup
```

Main idea:

```text
For each number, calculate target - current number.
Check if that needed number already exists.
If yes, return the two indices.
If no, store the current number and index.
```

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(n)
```

---

## 25. What is the pattern for Contains Duplicate?

My answer:









Reference answer:

Pattern:

```text
Hash set
```

Main idea:

```text
Go through each number.
If it has already been seen, return true.
Otherwise, add it to the set.
If the loop finishes, return false.
```

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(n)
```

---

## 26. What is the pattern for Best Time to Buy and Sell Stock?

My answer:









Reference answer:

Pattern:

```text
Track best value so far
```

Main idea:

```text
Track the lowest price seen so far.
For each price, calculate the profit if sold today.
Update max profit.
```

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(1)
```

---

## 27. What is the pattern for Valid Anagram?

My answer:









Reference answer:

Pattern:

```text
Frequency counter
```

Main idea:

```text
Count characters in the first string.
Subtract characters from the second string.
If counts balance, it is an anagram.
```

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(1) for lowercase English letters
```

---

## 28. What is the pattern for Move Zeroes?

My answer:









Reference answer:

Pattern:

```text
Two pointers / overwrite position
```

Main idea:

```text
Move all nonzero values forward.
Then fill the rest with zeroes.
Modify the vector in-place.
```

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(1)
```

---

## 29. What is the pattern for Reverse String?

My answer:









Reference answer:

Pattern:

```text
Two pointers
```

Main idea:

```text
Use one pointer at the left.
Use one pointer at the right.
Swap characters.
Move left forward and right backward.
Stop when left >= right.
```

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(1)
```

---

# Week 2 — Pointers, Linked Lists, CAN Validation, ADC, UART

## 30. What is a pointer?

My answer:









Reference answer:

A pointer is a variable that stores a memory address.

Example:

```cpp
int x = 10;
int* p = &x;
```

Here, `p` stores the address of `x`.

Simple version:

```text
A pointer stores the memory address of another variable.
```

---

## 31. What does dereference mean?

My answer:









Reference answer:

Dereferencing means using a pointer to access the value at the address it stores.

Example:

```cpp
int x = 10;
int* p = &x;

std::cout << *p << std::endl;
```

`*p` gives the value of `x`.

---

## 32. What is a reference?

My answer:









Reference answer:

A reference is another name for an existing variable.

Example:

```cpp
int x = 10;
int& r = x;
```

Here, `r` is another name for `x`.

---

## 33. Pointer vs reference?

My answer:









Reference answer:

A pointer stores an address and can be changed to point somewhere else.

A reference is an alias for an existing variable.

Simple version:

```text
Pointer = stores an address
Reference = another name for an existing variable
```

---

## 34. What is `nullptr`?

My answer:









Reference answer:

`nullptr` means the pointer points to nothing.

It is often used to mark the end of a linked list.

Example:

```cpp
ListNode* curr = nullptr;
```

In a linked list, the last node usually has:

```cpp
lastNode->next = nullptr;
```

---

## 35. Why do linked lists use pointers?

My answer:









Reference answer:

Each linked list node points to the next node.

Example:

```cpp
struct ListNode {
    int val;
    ListNode* next;
};
```

The `next` pointer stores the address of the next node.

---

## 36. What is the Reverse Linked List pattern?

My answer:









Reference answer:

Pattern:

```text
prev / curr / next pointer reversal
```

Steps:

```text
1. Save curr->next.
2. Point curr->next backward to prev.
3. Move prev to curr.
4. Move curr to next.
```

At the end, `prev` is the new head.

---

## 37. What is a dummy node?

My answer:









Reference answer:

A dummy node is a fake node placed before the real head of a linked list.

It helps avoid special cases when modifying the head.

Example:

```cpp
ListNode dummy;
dummy.next = head;
```

The real list starts at:

```cpp
dummy.next
```

---

## 38. Why use a dummy node?

My answer:









Reference answer:

A dummy node makes linked list operations cleaner because it removes special handling for the first node.

Simple version:

```text
The dummy node gives me a safe node before the head so I can modify curr->next consistently.
```

---

## 39. How does slow/fast pointer logic detect a cycle?

My answer:









Reference answer:

Use two pointers:

```text
slow moves 1 step
fast moves 2 steps
```

If there is a cycle, fast eventually catches slow.

If there is no cycle, fast reaches `nullptr`.

---

## 40. How does slow/fast pointer logic find the middle of a linked list?

My answer:









Reference answer:

Use two pointers:

```text
slow moves 1 step
fast moves 2 steps
```

When fast reaches the end, slow is at the middle.

---

## 41. What is stack memory?

My answer:









Reference answer:

Stack memory is used for local variables.

Example:

```cpp
int stack_value = 5;
```

The variable is automatically created when the function starts and destroyed when the function ends.

Simple version:

```text
Stack memory is automatic, fast, and managed for me, but limited in size.
```

---

## 42. What is heap memory?

My answer:









Reference answer:

Heap memory is used for dynamic allocation.

Example:

```cpp
int* heap_value = new int(10);
delete heap_value;
```

The programmer is responsible for freeing heap memory.

Simple version:

```text
Heap memory is manually managed when using new/delete.
```

---

## 43. Why can `new/delete` be risky in embedded systems?

My answer:









Reference answer:

`new/delete` can be risky because embedded systems have limited memory and need predictable behavior.

Possible problems:

```text
memory leaks
heap fragmentation
unpredictable allocation time
allocation failure
dangling pointers
harder debugging
```

Simple version:

```text
Embedded systems often avoid dynamic allocation because memory is limited and predictable behavior matters.
```

---

## 44. Why check CAN ID before decoding?

My answer:









Reference answer:

The CAN ID tells the decoder what type of message the frame contains.

Example:

```text
0x100 = analog input data
0x101 = battery and temperature data
0x102 = status flags
0x200 = vehicle telemetry
```

If the ID is unknown, the decoder does not know how to interpret the payload.

---

## 45. Why check DLC before decoding?

My answer:









Reference answer:

DLC tells how many bytes are valid in the frame.

If the decoder expects 8 bytes but the frame only has 4, reading bytes 4 through 7 would be wrong.

Simple version:

```text
Check DLC before decoding so the decoder does not read invalid or missing bytes.
```

---

## 46. What is `pack_u16(low, high)`?

My answer:









Reference answer:

`pack_u16(low, high)` combines two 8-bit bytes into one 16-bit value.

Example:

```cpp
std::uint16_t pack_u16(std::uint8_t low, std::uint8_t high) {
    return static_cast<std::uint16_t>(low) |
           (static_cast<std::uint16_t>(high) << 8);
}
```

It is used to decode 16-bit sensor values from CAN data bytes.

---

## 47. What is ADC?

My answer:









Reference answer:

ADC stands for Analog-to-Digital Converter.

It converts an analog voltage into a digital number.

Example:

```text
0 V to 3.3 V input becomes a raw integer value.
```

---

## 48. What does a 12-bit ADC range mean?

My answer:









Reference answer:

A 12-bit ADC has:

```text
2^12 = 4096 possible values
```

So the raw ADC range is:

```text
0 to 4095
```

If the reference voltage is 3.3 V:

```text
0    = 0 V
4095 = about 3.3 V
2048 = about 1.65 V
```

---

## 49. How do you convert raw ADC to voltage?

My answer:









Reference answer:

Use:

```text
voltage = (adc_raw * 3.3) / 4095.0
```

Example:

```text
adc_raw = 2048
voltage ≈ 1.65 V
```

---

## 50. Why is ADC useful for vehicle telemetry?

My answer:









Reference answer:

Many vehicle sensors output analog voltages.

Examples:

```text
throttle position sensor
brake pressure sensor
temperature sensor
battery voltage divider
suspension position sensor
```

The STM32 can read these voltages using ADC, then firmware can pack the raw values into CAN frames.

---

## 51. What is UART?

My answer:









Reference answer:

UART stands for Universal Asynchronous Receiver/Transmitter.

It is a serial communication peripheral used to send and receive data.

Simple version:

```text
UART lets a microcontroller send and receive serial data using TX and RX pins.
```

---

## 52. What are TX and RX?

My answer:









Reference answer:

```text
TX = transmit
RX = receive
```

TX sends data out of the microcontroller.

RX receives data into the microcontroller.

---

## 53. What is baud rate?

My answer:









Reference answer:

Baud rate is the communication speed for UART.

Common baud rates:

```text
9600
38400
57600
115200
```

Both devices must use the same baud rate.

---

## 54. What happens if baud rate is wrong?

My answer:









Reference answer:

If baud rate is wrong, the serial terminal may show garbage characters or no readable output.

Example:

```text
STM32 configured for 115200
Terminal configured for 9600
Result: unreadable output
```

Both sides must match.

---

## 55. What is blocking UART transmit?

My answer:









Reference answer:

Blocking transmit means the program waits until UART transmission is finished before continuing.

Simple version:

```text
Send message, wait until it is done, then continue.
```

It is simple but can slow down the program if too much data is printed.

---

## 56. What is non-blocking UART transmit?

My answer:









Reference answer:

Non-blocking transmit means the program starts the transmission and continues running while UART hardware, interrupt, or DMA handles the transfer.

It is better for real-time systems but more complex.

---

## 57. Why is UART useful for debugging?

My answer:









Reference answer:

UART is useful because it lets firmware print internal values while the microcontroller is running.

Useful debug prints:

```text
System started
ADC raw value
ADC voltage
CAN frame ID
CAN data bytes
Fault detected
Invalid DLC
Unknown CAN ID
Counter value
```

Simple version:

```text
UART gives embedded firmware a way to print debug information when there is no normal desktop console.
```

---

## 58. What would you print from a telemetry system?

My answer:









Reference answer:

Useful telemetry debug prints:

```text
ADC raw readings
scaled voltages
CAN frame ID
DLC
CAN data bytes
decoded sensor values
fault messages
counter values
system state
```

These help confirm the telemetry system is reading, packing, transmitting, and decoding data correctly.

---

# Core Summary

## Main C++ idea

```text
Pointers and references are the foundation of linked lists and efficient C++ data passing.
```

## Main CAN idea

```text
Never decode a CAN payload blindly. Check the CAN ID and DLC first.
```

## Main embedded idea

```text
Microcontrollers read real-world signals through peripherals like GPIO, ADC, UART, and CAN.
```

## Main debugging idea

```text
UART is one of the simplest ways to see what firmware is doing while it runs.
```
---

# Week 2 Day 5 — Circular Buffers and Debugging

## 59. What is a circular buffer?

My answer:









Reference answer:

A circular buffer is a fixed-size buffer that wraps around when it reaches the end.

It uses indexes instead of shifting data around.

Important parts:

```text
head = next write location
tail = next read location
count = number of stored items
```

Simple version:

```text
A circular buffer stores streaming data in fixed memory and wraps around when it reaches the end.
```

---

## 60. What does `head` mean in a circular buffer?

My answer:









Reference answer:

`head` is the next write location.

When new data is pushed into the buffer, it is stored at `head`.

Then `head` moves forward.

Example:

```cpp
buffer_[head_] = frame;
head_ = (head_ + 1) % Capacity;
```

Simple version:

```text
head tells the buffer where the next new item will be written.
```

---

## 61. What does `tail` mean in a circular buffer?

My answer:









Reference answer:

`tail` is the next read location.

When data is popped from the buffer, it is read from `tail`.

Then `tail` moves forward.

Example:

```cpp
frame = buffer_[tail_];
tail_ = (tail_ + 1) % Capacity;
```

Simple version:

```text
tail tells the buffer where the next item will be read from.
```

---

## 62. What does `count` mean in a circular buffer?

My answer:









Reference answer:

`count` stores how many items are currently in the buffer.

It helps detect whether the buffer is empty or full.

```cpp
bool is_empty() const {
    return count_ == 0;
}

bool is_full() const {
    return count_ == Capacity;
}
```

Simple version:

```text
count tracks how many valid items are currently stored.
```

---

## 63. Why does a circular buffer use modulo?

My answer:









Reference answer:

Modulo wraps the index back to zero after it reaches the end.

Example:

```text
Capacity = 5
head = 4

(head + 1) % 5 = 0
```

So after index `4`, the next index becomes `0`.

Simple version:

```text
Modulo creates the wraparound behavior that makes the buffer circular.
```

---

## 64. Why are circular buffers common in embedded systems?

My answer:









Reference answer:

Circular buffers are common in embedded systems because they use fixed-size memory and avoid dynamic allocation.

Benefits:

```text
fixed memory usage
predictable behavior
no new/delete
efficient push and pop
good for streaming data
useful with interrupts
```

Simple version:

```text
Circular buffers are useful in embedded systems because they handle streaming data with predictable memory usage.
```

---

## 65. How does a UART RX buffer relate to a circular buffer?

My answer:









Reference answer:

UART receives bytes over time.

A UART interrupt can push each received byte into a circular buffer.

Then the main program can pop bytes from the buffer later.

Example flow:

```text
UART byte arrives
interrupt runs
byte is pushed into RX buffer
main loop pops bytes and processes them
```

Simple version:

```text
A UART RX circular buffer prevents received bytes from being lost while the main code is busy.
```

---

## 66. How does a CAN receive queue relate to a circular buffer?

My answer:









Reference answer:

CAN frames can arrive at unpredictable times.

A CAN receive interrupt or driver can store incoming frames in a queue or circular buffer.

Then the main program can process frames one by one.

Example flow:

```text
CAN frame arrives
driver stores frame in buffer
main loop pops frame
decoder checks ID and DLC
decoder decodes payload
fault analyzer checks for problems
```

Simple version:

```text
A CAN receive queue separates fast frame reception from slower decoding and fault analysis.
```

---

## 67. What should `push(const CanFrame& frame)` do?

My answer:









Reference answer:

`push` should add a frame to the buffer if there is space.

It returns:

```text
true if the frame was stored
false if the buffer was full
```

Example:

```cpp
bool CircularBuffer::push(const CanFrame& frame) {
    if (is_full()) {
        return false;
    }

    buffer_[head_] = frame;
    head_ = (head_ + 1) % Capacity;
    count_++;

    return true;
}
```

Simple version:

```text
push stores a new frame at head, moves head forward, and increases count.
```

---

## 68. What should `pop(CanFrame& frame)` do?

My answer:









Reference answer:

`pop` should remove a frame from the buffer if one is available.

It returns:

```text
true if a frame was read
false if the buffer was empty
```

Example:

```cpp
bool CircularBuffer::pop(CanFrame& frame) {
    if (is_empty()) {
        return false;
    }

    frame = buffer_[tail_];
    tail_ = (tail_ + 1) % Capacity;
    count_--;

    return true;
}
```

Simple version:

```text
pop reads the frame at tail, moves tail forward, and decreases count.
```

---

## 69. Why does `push` use `const CanFrame& frame`?

My answer:









Reference answer:

`push` only needs to read the frame being passed in.

It should not modify the caller's frame.

Using `const CanFrame&` avoids unnecessary copying and prevents accidental modification.

Simple version:

```text
push uses const reference because it only reads the input frame.
```

---

## 70. Why does `pop` use `CanFrame& frame`?

My answer:









Reference answer:

`pop` needs to give a frame back to the caller.

Using a non-const reference lets the function write into the caller's variable.

Example:

```cpp
CanFrame frame{};

if (rx_buffer.pop(frame)) {
    process_frame(frame);
}
```

Simple version:

```text
pop uses non-const reference because it needs to modify the caller's output variable.
```

---

## 71. What should happen if a circular buffer is full?

My answer:









Reference answer:

If the buffer is full, the program must handle overflow.

Possible strategies:

```text
drop the new item
overwrite the oldest item
set an overflow flag
count dropped items
print/debug a warning
```

For this project:

```text
If the buffer is full, push returns false and the frame is dropped.
```

Simple version:

```text
A full buffer should be handled intentionally instead of silently corrupting data.
```

---

## 72. What should happen if a circular buffer is empty?

My answer:









Reference answer:

If the buffer is empty, `pop` should fail and return false.

This prevents reading invalid data.

Example:

```cpp
if (is_empty()) {
    return false;
}
```

Simple version:

```text
An empty buffer should not return fake or invalid data.
```

---

## 73. What is SWD?

My answer:









Reference answer:

SWD stands for Serial Wire Debug.

It is a debugging interface commonly used with STM32 microcontrollers.

SWD can be used to:

```text
flash code
set breakpoints
step through firmware
watch variables
inspect registers
debug embedded code
```

Simple version:

```text
SWD is the common STM32 debug interface used to program and debug firmware.
```

---

## 74. What is JTAG?

My answer:









Reference answer:

JTAG is a debugging and programming interface.

It usually uses more pins than SWD.

It can be used for:

```text
debugging
programming
boundary scan testing
```

Simple version:

```text
JTAG is another hardware debugging interface, but STM32 projects often use SWD because it needs fewer pins.
```

---

## 75. What is ST-LINK?

My answer:









Reference answer:

ST-LINK is STMicroelectronics' programmer/debugger for STM32 boards.

It lets the computer communicate with the STM32 for flashing and debugging.

It can be used to:

```text
flash firmware
start a debug session
set breakpoints
step through code
inspect variables
view registers
reset the microcontroller
```

Simple version:

```text
ST-LINK is the tool that lets me program and debug STM32 microcontrollers.
```

---

## 76. What is a breakpoint?

My answer:









Reference answer:

A breakpoint pauses the program at a specific line of code.

It lets me inspect the program state at that moment.

Use breakpoints to check:

```text
Did the code reach this line?
What are the variable values?
Did this branch run?
What happened before the bug?
```

Simple version:

```text
A breakpoint stops the program so I can inspect what is happening.
```

---

## 77. What is a watch variable?

My answer:









Reference answer:

A watch variable is a variable I monitor during debugging.

Examples:

```text
adc_raw
voltage
frame.id
frame.dlc
counter
head
tail
count
fault flag
```

Simple version:

```text
A watch variable lets me see how a variable changes while debugging.
```

---

## 78. What is step over?

My answer:









Reference answer:

Step over runs the current line without entering any function call on that line.

Use it when:

```text
I trust the function
I only want to go to the next line
I do not need to debug inside the function
```

Simple version:

```text
Step over executes a function call without going inside it.
```

---

## 79. What is step into?

My answer:









Reference answer:

Step into enters the function being called.

Use it when:

```text
I want to debug inside that function
I do not trust the function yet
I want to see exactly what happens
```

Simple version:

```text
Step into lets me follow execution inside a function.
```

---

## 80. What is register view?

My answer:









Reference answer:

Register view shows CPU and peripheral registers during debugging.

This is useful because embedded peripherals are controlled by registers.

Examples:

```text
GPIO registers
ADC registers
UART registers
timer registers
CAN registers
```

Simple version:

```text
Register view lets me inspect low-level hardware configuration and status.
```

---

## Day 5 Core Interview Summary

My answer:









Reference answer:

```text
Circular buffers are common in embedded systems because they provide fixed-size, predictable storage for streaming data like UART bytes or CAN frames. They use head and tail indexes to avoid moving memory and wrap around with modulo arithmetic.
```
---

# Week 3 Day 2 — Constructors and Queue Using Stacks

## What is a constructor?

My answer:









Reference answer:

A constructor is a special function that runs automatically when an object is created.

It is usually used to initialize the object.

Example:

```cpp
TelemetryDecoder::TelemetryDecoder()
    : frames_seen_(0) {
}
```

Simple version:

```text
A constructor sets up an object when it is created.
```

---

## Why use a constructor?

My answer:









Reference answer:

A constructor is used to initialize internal state.

Example:

```cpp
std::size_t frames_seen_;
```

This should start at zero.

The constructor makes sure that happens.

Simple version:

```text
A constructor makes sure an object starts in a valid state.
```

---

## What is FIFO?

My answer:









Reference answer:

FIFO means First In, First Out.

A queue uses FIFO behavior.

Example:

```text
1 enters first
2 enters second
3 enters third

Removal order:
1, 2, 3
```

Simple version:

```text
FIFO means the first item added is the first item removed.
```

---

## What is LIFO?

My answer:









Reference answer:

LIFO means Last In, First Out.

A stack uses LIFO behavior.

Example:

```text
1 pushed first
2 pushed second
3 pushed third

Removal order:
3, 2, 1
```

Simple version:

```text
LIFO means the last item added is the first item removed.
```

---

## How can two stacks implement a queue?

My answer:









Reference answer:

Use one stack for input and one stack for output.

```text
input stack receives new values
output stack provides values in queue order
```

When output is empty, move all values from input to output.

This reverses the order, making the oldest value appear on top of output.

Simple version:

```text
Moving items from one stack to another reverses them, which lets two stacks create FIFO queue behavior.
```

---

## Why does a decoder need routing logic?

My answer:









Reference answer:

A decoder needs routing logic because different CAN IDs have different payload formats.

Example:

```text
0x100 = analog inputs
0x101 = battery and temperature
0x102 = status flags
0x200 = vehicle data
```

The public `decode()` function checks the CAN ID and calls the correct private helper.

Simple version:

```text
Routing logic lets the decoder choose the correct decoding function based on the CAN ID.
```
---

# Week 3 Day 3 — std::array vs Raw Array

## What is a raw array?

My answer:









Reference answer:

A raw array is a C-style fixed-size array.

Example:

```cpp
std::uint8_t data[8];
```

This creates 8 elements of type `std::uint8_t`.

Simple version:

```text
A raw array is a fixed-size block of elements using C-style syntax.
```

---

## What is std::array?

My answer:









Reference answer:

`std::array` is a C++ fixed-size array wrapper.

Example:

```cpp
std::array<std::uint8_t, 8> data;
```

It keeps fixed-size storage but adds C++ features like `.size()`, `.at()`, and iterators.

Simple version:

```text
std::array is a safer C++ wrapper around a fixed-size array.
```

---

## Why are fixed-size arrays useful in embedded systems?

My answer:









Reference answer:

Fixed-size arrays are useful because embedded systems need predictable memory usage.

Benefits:

```text
fixed memory size
no heap allocation
simple layout
predictable behavior
good for hardware buffers
good for protocol payloads
```

Simple version:

```text
Fixed-size arrays are useful in embedded systems because they avoid dynamic memory and make memory usage predictable.
```

---

## What does std::array add?

My answer:









Reference answer:

`std::array` adds C++ convenience and safety while keeping fixed size.

It provides:

```text
.size()
.at()
.begin()
.end()
better STL compatibility
```

Simple version:

```text
std::array gives fixed-size array storage with useful C++ member functions.
```

---

## Why keep uint8_t data[8] in CanFrame?

My answer:









Reference answer:

We keep `uint8_t data[8]` because it directly represents the 8-byte payload of a classic CAN frame.

It also looks closer to embedded firmware and CAN driver code.

Example:

```cpp
struct CanFrame {
    std::uint32_t id;
    std::uint8_t dlc;
    std::uint8_t data[8];
};
```

Simple version:

```text
uint8_t data[8] clearly represents a fixed 8-byte CAN payload and matches embedded firmware style.
```

---

## Raw array vs std::array?

My answer:









Reference answer:

A raw array is C-style and common in embedded code.

`std::array` is C++ style and adds useful features.

Raw array:

```text
simple
common in firmware structs
no .size()
can decay to pointer
```

`std::array`:

```text
safer C++ wrapper
has .size()
supports iterators
works with STL
still fixed-size
```

Simple version:

```text
Raw arrays are closer to C/firmware style, while std::array is safer and more convenient modern C++.
```

---

## What is the difference between stack and queue access order?

My answer:









Reference answer:

A stack uses LIFO.

```text
Last In, First Out
```

A queue uses FIFO.

```text
First In, First Out
```

Simple version:

```text
A stack removes the newest item first, while a queue removes the oldest item first.
```

---

## How can queues implement a stack?

My answer:









Reference answer:

Use a queue and rotate it after each push.

When a new value is pushed, move all older values behind it.

Example:

```text
push 1:
queue = 1

push 2:
queue = 2, 1

push 3:
queue = 3, 2, 1
```

Now the newest value is always at the front.

Simple version:

```text
Queues can implement a stack by rotating elements so the newest item is always removed first.
```

---

## Week 3 Day 3 Core Interview Summary

My answer:









Reference answer:

Fixed-size arrays are useful in embedded systems because they provide predictable memory usage. `std::array` adds safer C++ features like `.size()`, but `uint8_t data[8]` is still common for embedded protocol payloads like CAN frames because it directly represents a fixed 8-byte message buffer. Stack using Queues reinforced that data structures are defined by access order: stack is LIFO and queue is FIFO.
---

# Week 3 Day 4 — Min Stack and Output Formatting

## What are common stack use cases?

My answer:









Reference answer:

Stacks are useful when the most recent item needs to be handled first.

Common use cases:

```text
parsing
undo history
function calls
recursion
monotonic stack problems
nested structures
expression evaluation
backtracking
```

Simple version:

```text
Stacks are useful for problems where the last thing added should be handled first.
```

---

## Why do parentheses/parsing problems use stacks?

My answer:









Reference answer:

Parsing problems use stacks because nested structures must close in reverse order.

Example:

```text
({[]})
```

The `[` opens last, so it must close first.

Simple version:

```text
Stacks work for parsing because the most recent unfinished item must be completed first.
```

---

## How does the function call stack work?

My answer:









Reference answer:

When a function is called, it is placed on the call stack.

When it finishes, it is removed from the stack and control returns to the previous function.

Simple version:

```text
The most recently called function finishes first, so function calls follow stack behavior.
```

---

## What is a monotonic stack?

My answer:









Reference answer:

A monotonic stack keeps values in increasing or decreasing order.

It is useful for finding next greater or next smaller values.

Example problem:

```text
Daily Temperatures
```

Simple version:

```text
A monotonic stack stores values in a controlled order to answer next greater or next smaller questions efficiently.
```

---

## What is Min Stack's trick?

My answer:









Reference answer:

Min Stack uses two stacks.

```text
values stack = stores all values
mins stack = stores minimum values
```

The minimum stack tracks the current minimum so `getMin()` can return the minimum in O(1).

Simple version:

```text
Min Stack avoids scanning by keeping a second stack of minimum values.
```

---

## Why does Min Stack use <= when pushing to the min stack?

My answer:









Reference answer:

It uses `<=` to handle duplicate minimum values correctly.

Example:

```text
push 2
push 2
pop
```

Both `2`s are minimum values, so both should be tracked.

Simple version:

```text
Using <= makes sure duplicate minimum values are stored correctly.
```

---

## What is the time complexity of Min Stack?

My answer:









Reference answer:

```text
push: O(1)
pop: O(1)
top: O(1)
getMin: O(1)
```

The goal is for every operation to be constant time.

---

## Why does formatting matter for debugging tools?

My answer:









Reference answer:

Formatting matters because debugging output should be easy to read quickly.

Good output shows:

```text
frame ID
DLC
payload
decoded values
faults
result
```

Simple version:

```text
Clear formatting helps developers find problems faster.
```

---

## How would you explain your CAN decoder output in an interview?

My answer:









Reference answer:

My decoder takes simulated CAN frames, validates the CAN ID and DLC, routes known messages to decoder functions, and formats the result like a diagnostic log.

For ID `0x100`, it decodes three analog raw values, a status byte, and a counter.

Simple version:

```text
The formatted output makes it easy to inspect frame contents, decoded sensor values, and fault conditions.
```

---

## Week 3 Day 4 Core Interview Summary

My answer:









Reference answer:

Stacks are useful when the most recent item must be handled first, such as parsing, undo history, function calls, recursion, and monotonic stack problems. Min Stack uses an extra stack to track current minimum values so `getMin()` runs in O(1). For the CAN decoder project, clean output formatting makes the tool easier to use as a debugging or diagnostic utility.
---

# Week 3 Day 5 — Decode 0x101 and Monotonic Stack

## What does CAN ID 0x101 represent in this project?

My answer:









Reference answer:

CAN ID `0x101` represents battery and temperature data.

Payload format:

```text
Byte 0-1: battery_mV
Byte 2-3: temperature_deciC
Byte 4-7: reserved
```

Simple version:

```text
0x101 stores battery voltage and temperature information.
```

---

## How do you decode battery_mV from 0x101?

My answer:









Reference answer:

Use `pack_u16()` to combine bytes 0 and 1.

```cpp
std::uint16_t battery_mV = pack_u16(frame.data[0], frame.data[1]);
```

The data is little-endian, so byte 0 is the low byte and byte 1 is the high byte.

Simple version:

```text
battery_mV is decoded by combining bytes 0 and 1 into a 16-bit value.
```

---

## How do you scale battery_mV to volts?

My answer:









Reference answer:

Divide by `1000.0`.

```cpp
double battery_V = battery_mV / 1000.0;
```

Example:

```text
5000 mV = 5.00 V
12000 mV = 12.00 V
```

Simple version:

```text
Millivolts convert to volts by dividing by 1000.
```

---

## How do you decode temperature_deciC from 0x101?

My answer:









Reference answer:

Use `pack_u16()` to combine bytes 2 and 3.

```cpp
std::uint16_t temperature_deciC = pack_u16(frame.data[2], frame.data[3]);
```

The data is little-endian, so byte 2 is the low byte and byte 3 is the high byte.

Simple version:

```text
temperature_deciC is decoded by combining bytes 2 and 3 into a 16-bit value.
```

---

## How do you scale temperature_deciC to Celsius?

My answer:









Reference answer:

Divide by `10.0`.

```cpp
double temperature_C = temperature_deciC / 10.0;
```

Example:

```text
300 deciC = 30.0 C
805 deciC = 80.5 C
```

Simple version:

```text
Deci-degrees Celsius convert to Celsius by dividing by 10.
```

---

## What does reserved mean in a CAN payload?

My answer:









Reference answer:

Reserved bytes are not used yet, but they are kept for future expansion.

For `0x101`:

```text
Byte 4-7: reserved
```

They could later be used for status flags, fault bits, or extra sensor data.

Simple version:

```text
Reserved bytes are unused now but available for future protocol changes.
```

---

## What is a monotonic stack?

My answer:









Reference answer:

A monotonic stack keeps values in a controlled increasing or decreasing order.

It is useful for problems involving next greater or next smaller values.

Simple version:

```text
A monotonic stack stores unresolved values in an order that helps answer next greater or next smaller questions efficiently.
```

---

## Why does Daily Temperatures store indexes?

My answer:









Reference answer:

Daily Temperatures stores indexes because the answer is the number of days waited.

Code:

```cpp
answer[previous_index] = i - previous_index;
```

If I only stored temperatures, I would not know the distance between days.

Simple version:

```text
Indexes are needed because the answer depends on how far apart two days are.
```

---

## Why is Daily Temperatures not just a normal stack problem?

My answer:









Reference answer:

It is not just a normal stack problem because the stack is used to maintain unresolved days in a useful order.

When the current temperature is warmer than previous unresolved days, the stack helps resolve them efficiently.

Simple version:

```text
Daily Temperatures uses a monotonic stack because it needs to find the next warmer day efficiently, not just use basic LIFO behavior.
```

---

## What is the time complexity of Daily Temperatures?

My answer:









Reference answer:

Each index is pushed once and popped once.

So the time complexity is:

```text
O(n)
```

The space complexity is:

```text
O(n)
```

Simple version:

```text
Daily Temperatures is O(n) because each day enters and leaves the stack at most once.
```

---

## Week 3 Day 5 Core Interview Summary

My answer:









Reference answer:

CAN ID `0x101` stores battery voltage in millivolts and temperature in deci-degrees Celsius. The decoder combines little-endian byte pairs into raw values, then scales battery to volts by dividing by `1000.0` and temperature to Celsius by dividing by `10.0`. Daily Temperatures uses a monotonic stack of indexes to efficiently resolve previous days when a warmer temperature appears.
---

# Week 3 Day 6 — unordered_map and Week 3 Review

## What is a class?

My answer:









Reference answer:

A class is a custom type that groups related data and functions together.

Example:

```cpp
class TelemetryDecoder {
public:
    void decode(const CanFrame& frame);

private:
    void decode_0x100(const CanFrame& frame);
};
```

Simple version:

```text
A class groups related data and behavior into one object.
```

---

## What is encapsulation?

My answer:









Reference answer:

Encapsulation means hiding internal implementation details and exposing a clean public interface.

In this project:

```text
public decode() = outside interface
private decode_0x100() = internal helper
```

Simple version:

```text
Encapsulation lets outside code use an object without needing to know all of its internal details.
```

---

## What is a constructor?

My answer:









Reference answer:

A constructor is a special function that runs automatically when an object is created.

It is used to initialize the object.

Example:

```cpp
TelemetryDecoder::TelemetryDecoder()
    : frames_seen_(0) {
}
```

Simple version:

```text
A constructor sets up an object when it is created.
```

---

## What is a destructor?

My answer:









Reference answer:

A destructor is a special function that runs automatically when an object is destroyed.

It is commonly used to clean up resources.

Example:

```cpp
class Example {
public:
    ~Example() {
        // cleanup code
    }
};
```

Simple version:

```text
A destructor cleans up an object when it goes away.
```

---

## What is RAII?

My answer:









Reference answer:

RAII means Resource Acquisition Is Initialization.

It means resources are acquired in a constructor and released in a destructor.

Examples of resources:

```text
memory
files
locks
network handles
hardware handles
```

Simple version:

```text
RAII ties resource lifetime to object lifetime so cleanup happens automatically.
```

---

## Why might embedded teams limit STL usage?

My answer:









Reference answer:

Embedded teams may limit STL usage because some STL features can involve dynamic memory allocation, larger code size, exceptions, or less predictable timing.

Concerns:

```text
RAM usage
flash/code size
heap allocation
deterministic timing
hidden runtime behavior
```

Simple version:

```text
Embedded teams may limit STL usage when memory, timing, and code size must be tightly controlled.
```

---

## Why use std::array instead of raw arrays sometimes?

My answer:









Reference answer:

`std::array` is safer and more convenient than a raw array while still having fixed size.

It provides:

```text
.size()
.at()
.begin()
.end()
STL compatibility
```

Simple version:

```text
std::array gives fixed-size storage with safer C++ features.
```

---

## How does 0x101 scaling work?

My answer:









Reference answer:

CAN ID `0x101` stores battery voltage and temperature as raw integer values.

```text
Byte 0-1: battery_mV
Byte 2-3: temperature_deciC
```

The decoder combines the bytes using `pack_u16()`.

Then it scales:

```cpp
double battery_V = battery_mV / 1000.0;
double temperature_C = temperature_deciC / 10.0;
```

Simple version:

```text
battery_mV is divided by 1000 to get volts, and temperature_deciC is divided by 10 to get Celsius.
```

---

## What is a stack?

My answer:









Reference answer:

A stack is a data structure that uses LIFO behavior.

```text
LIFO = Last In, First Out
```

The last item added is the first item removed.

Common operations:

```text
push
pop
top
empty
```

Simple version:

```text
A stack removes the newest item first.
```

---

## What is an unordered_map?

My answer:









Reference answer:

`std::unordered_map` stores key-value pairs.

Example:

```cpp
std::unordered_map<std::uint32_t, std::string> frame_names = {
    {0x100, "Analog Inputs"},
    {0x101, "Battery and Temperature"}
};
```

It provides average O(1) lookup.

Simple version:

```text
unordered_map lets me quickly look up a value using a key.
```

---

## Why is unordered_map useful for Top K Frequent Elements?

My answer:









Reference answer:

Top K Frequent Elements needs to count how often each number appears.

`unordered_map` is useful because it maps each number to its frequency.

Example:

```text
1 -> 3
2 -> 2
3 -> 1
```

Simple version:

```text
unordered_map is useful because it quickly counts frequencies.
```

---

## Week 3 Core Interview Summary

My answer:









Reference answer:

This week I moved the CAN decoding logic into a `TelemetryDecoder` class. The class uses public methods as the outside interface and private helpers for specific CAN IDs. I added a constructor, formatted output, decoded `0x100` analog inputs, decoded `0x101` battery and temperature data, and scaled raw values into real units. I also practiced stack problems and learned that `std::unordered_map` provides key-value storage with average O(1) lookup.
---

# Week 4 Day 1 — Bit Basics

## What is a bit mask?

My answer:









Reference answer:

A bit mask is a value used to target one or more specific bits.

Example:

```cpp
std::uint8_t mask = 0x04;
```

Binary:

```text
0x04 = 0000 0100
```

This targets bit position `2`.

Simple version:

```text
A bit mask lets me isolate, check, set, clear, or toggle specific bits.
```

---

## How do you set a bit?

My answer:









Reference answer:

Use bitwise OR.

```cpp
value |= mask;
```

This forces the masked bit to become `1`.

Simple version:

```text
Set means force a bit to 1.
```

---

## How do you clear a bit?

My answer:









Reference answer:

Use bitwise AND with the inverted mask.

```cpp
value &= ~mask;
```

This forces the masked bit to become `0`.

Simple version:

```text
Clear means force a bit to 0.
```

---

## How do you toggle a bit?

My answer:









Reference answer:

Use bitwise XOR.

```cpp
value ^= mask;
```

This flips the masked bit.

Simple version:

```text
Toggle means change 0 to 1 or 1 to 0.
```

---

## How do you check if a bit is set?

My answer:









Reference answer:

Use bitwise AND.

```cpp
(value & mask) != 0
```

If the result is not zero, the bit is set.

Simple version:

```text
Checking a bit means testing whether a specific bit is 1.
```

---

## What does get_bit do?

My answer:









Reference answer:

`get_bit` checks whether one bit position is set.

Example:

```cpp
bool bit_is_set = get_bit(value, 2);
```

It creates a mask by shifting `1` left by the bit position.

Simple version:

```text
get_bit checks whether a selected bit position is 1.
```

---

## What does is_mask_set do?

My answer:









Reference answer:

`is_mask_set` checks whether all bits in a mask are set.

Example:

```cpp
return (value & mask) == mask;
```

Simple version:

```text
is_mask_set checks whether a selected mask is present in the value.
```

---

## Why are flags common in embedded systems?

My answer:









Reference answer:

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

Simple version:

```text
Flags save space and match how hardware registers and protocol status bytes are designed.
```

---

## What is a status byte?

My answer:









Reference answer:

A status byte is one byte where individual bits represent different conditions.

Example:

```text
status = 0x85 = 1000 0101
```

This means bits `7`, `2`, and `0` are set.

Simple version:

```text
A status byte packs multiple flags into one byte.
```

---

## How did Number of 1 Bits work?

My answer:









Reference answer:

The simple method checks the lowest bit and shifts right.

```cpp
while (n != 0) {
    count += n & 1;
    n >>= 1;
}
```

`n & 1` checks whether the lowest bit is `1`.

`n >>= 1` moves the next bit into the lowest position.

Simple version:

```text
Number of 1 Bits counts how many bits in a number are set to 1.
```

---

## What does n & (n - 1) do?

My answer:









Reference answer:

`n & (n - 1)` removes the lowest set bit.

Example:

```text
n     = 1011
n - 1 = 1010

n & (n - 1) = 1010
```

One `1` bit was removed.

Simple version:

```text
n & (n - 1) clears the lowest 1 bit.
```

---

## Week 4 Day 1 Core Interview Summary

My answer:









Reference answer:

Bit masks are used to target specific bits inside a value. In embedded systems, flags are often packed into status bytes or hardware registers, so engineers need to know how to set, clear, toggle, and check bits. Number of 1 Bits works by using bit operations to count how many bits are set to 1.
---

# Week 4 Day 2 — constexpr Masks and Status Flags

## What is constexpr?

My answer:









Reference answer:

`constexpr` means compile-time constant.

It is used for values that are known before the program runs.

Example:

```cpp
constexpr std::uint8_t SENSOR1_VALID_MASK = 0x01;
```

Simple version:

```text
constexpr creates a constant value known at compile time.
```

---

## Why use constexpr for bit masks?

My answer:









Reference answer:

Bit masks are fixed values that should not change while the program runs.

Using `constexpr` also gives masks meaningful names.

Example:

```cpp
SENSOR1_VALID_MASK
```

is clearer than:

```cpp
0x01
```

Simple version:

```text
constexpr makes fixed masks readable and prevents magic numbers.
```

---

## What does SENSOR1_VALID_MASK = 0x01 mean?

My answer:









Reference answer:

`0x01` is binary:

```text
0000 0001
```

It targets bit `0`.

If bit `0` represents sensor 1 validity, then `SENSOR1_VALID_MASK` checks whether sensor 1 is valid.

Simple version:

```text
SENSOR1_VALID_MASK targets bit 0 of the status byte.
```

---

## How is 0x07 interpreted as flags?

My answer:









Reference answer:

`0x07` is binary:

```text
0000 0111
```

This means bits `0`, `1`, and `2` are set.

If those bits mean sensor validity, then:

```text
sensor 1 valid = yes
sensor 2 valid = yes
sensor 3 valid = yes
```

Simple version:

```text
0x07 means the first three flag bits are set.
```

---

## What does bit 0 mean in the 0x100 status byte?

My answer:









Reference answer:

Bit `0` means sensor 1 valid.

Simple version:

```text
If bit 0 is set, sensor 1 is valid.
```

---

## What does bit 1 mean in the 0x100 status byte?

My answer:









Reference answer:

Bit `1` means sensor 2 valid.

Simple version:

```text
If bit 1 is set, sensor 2 is valid.
```

---

## What does bit 2 mean in the 0x100 status byte?

My answer:









Reference answer:

Bit `2` means sensor 3 valid.

Simple version:

```text
If bit 2 is set, sensor 3 is valid.
```

---

## What is the difference between raw value and interpreted status?

My answer:









Reference answer:

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
```

Simple version:

```text
Raw value is the data byte. Interpreted status is the meaning of the bits inside it.
```

---

## How does Counting Bits work?

My answer:









Reference answer:

Counting Bits builds an array where each index stores the number of `1` bits in that number.

Example:

```text
bits[5] = 2
```

because:

```text
5 = 101 binary
```

and there are two `1` bits.

Simple version:

```text
Counting Bits calculates the number of set bits for every number from 0 to n.
```

---

## What does bits[i] = bits[i >> 1] + (i & 1) mean?

My answer:









Reference answer:

`i >> 1` removes the last bit.

`i & 1` checks whether the last bit was `1`.

So:

```cpp
bits[i] = bits[i >> 1] + (i & 1);
```

means:

```text
bit count of i = bit count of i without its last bit + whether the last bit was 1
```

Simple version:

```text
Use the smaller number's bit count, then add the last bit.
```

---

## Week 4 Day 2 Core Interview Summary

My answer:









Reference answer:

`constexpr` is useful for fixed masks because it gives meaningful names to compile-time constants. In the decoder, the raw status byte `0x07` is interpreted as individual sensor-valid flags, turning raw CAN payload data into readable system status.
---

# Week 4 Day 3 — XOR and FaultAnalyzer

## What is XOR?

My answer:









Reference answer:

XOR is a bitwise operation written with:

```cpp
^
```

It compares bits and outputs `1` when the bits are different.

Simple version:

```text
XOR gives 1 when bits are different and 0 when bits are the same.
```

---

## What does x ^ x equal?

My answer:









Reference answer:

```text
x ^ x = 0
```

A value XORed with itself cancels out.

Simple version:

```text
Any number XOR itself becomes 0.
```

---

## What does x ^ 0 equal?

My answer:









Reference answer:

```text
x ^ 0 = x
```

A value XORed with zero stays unchanged.

Simple version:

```text
Any number XOR zero stays the same.
```

---

## Why does XOR find the single number?

My answer:









Reference answer:

XOR finds the single number because duplicate values cancel out.

Example:

```text
4 ^ 1 ^ 2 ^ 1 ^ 2
```

The duplicate `1`s cancel.

The duplicate `2`s cancel.

The leftover value is `4`.

Simple version:

```text
Pairs cancel out, and the unpaired number remains.
```

---

## How does XOR help solve Missing Number?

My answer:









Reference answer:

XOR all numbers from `0` to `n`.

Then XOR all numbers in the array.

Every number that appears in both places cancels out.

The missing number remains.

Simple version:

```text
XOR cancellation leaves the missing value behind.
```

---

## How is XOR useful for toggling bits?

My answer:









Reference answer:

XOR flips selected bits.

Example:

```cpp
value ^= mask;
```

If the masked bit is `0`, it becomes `1`.

If the masked bit is `1`, it becomes `0`.

Simple version:

```text
XOR toggles bits by flipping them.
```

---

## What is AnalogData?

My answer:









Reference answer:

`AnalogData` is a struct that stores decoded analog frame values.

It can hold:

```text
AIN1 raw
AIN2 raw
AIN3 raw
status byte
counter
sensor-valid flags
error flag
```

Simple version:

```text
AnalogData stores meaningful decoded values from the 0x100 CAN frame.
```

---

## What should a FaultAnalyzer class do?

My answer:









Reference answer:

A `FaultAnalyzer` should check decoded telemetry values against fault rules.

Examples:

```text
sensor invalid
analog value out of range
battery voltage too low
battery voltage too high
temperature too high
error flag set
```

Simple version:

```text
FaultAnalyzer checks decoded values and reports problems.
```

---

## What should FaultAnalyzer not do?

My answer:









Reference answer:

`FaultAnalyzer` should not decode raw CAN bytes.

That is the decoder's job.

Simple version:

```text
FaultAnalyzer should analyze values, not parse raw bytes.
```

---

## Why separate decoding from fault checking?

My answer:









Reference answer:

Decoding and fault checking are different responsibilities.

```text
TelemetryDecoder = turns raw bytes into meaningful values
FaultAnalyzer = checks those values for problems
```

This makes the project cleaner, easier to test, easier to expand, and easier to explain.

Simple version:

```text
Separation keeps each class focused on one job.
```

---

## Week 4 Day 3 Core Interview Summary

My answer:









Reference answer:

XOR is useful because duplicate values cancel out, which solves problems like Single Number and Missing Number. In the CAN decoder project, `FaultAnalyzer` separates fault rules from byte decoding, so `TelemetryDecoder` converts raw CAN payloads into values while `FaultAnalyzer` checks those values for abnormal conditions.
---

# Week 4 Day 5 — volatile Concept and Sensor Faults

## What is volatile?

My answer:









Reference answer:

`volatile` tells the compiler that a variable can change outside normal program flow.

This means the compiler should not optimize away reads or writes to that variable.

Simple version:

```text
volatile tells the compiler that something outside normal code may change this value.
```

---

## Why is volatile used for hardware registers?

My answer:









Reference answer:

Hardware registers can change because of hardware events.

Examples:

```text
GPIO pin changes
ADC conversion completes
UART receives data
timer counter increments
CAN message arrives
interrupt flag gets set
```

Simple version:

```text
volatile is used because hardware can change register values without normal C++ assignment.
```

---

## What are common examples of volatile variables?

My answer:









Reference answer:

Common examples include:

```text
hardware registers
memory-mapped peripheral registers
ISR-shared flags
timer counters
GPIO registers
status registers
```

Simple version:

```text
volatile is common for hardware registers and interrupt-shared flags.
```

---

## Why is volatile not enough for thread safety?

My answer:









Reference answer:

`volatile` does not make operations atomic.

It does not prevent race conditions.

Example:

```cpp
counter++;
```

This can involve:

```text
read counter
add one
write counter
```

An interrupt or another thread could interfere between those steps.

Simple version:

```text
volatile controls compiler optimization, but it does not protect shared data.
```

---

## What does GPIO_ODR |= (1U << 5) do?

My answer:









Reference answer:

It sets bit 5 of the GPIO output register.

```cpp
GPIO_ODR |= (1U << 5);
```

In a real embedded system, this could turn on a GPIO pin or LED.

Simple version:

```text
It sets bit 5 to 1.
```

---

## What does GPIO_ODR &= ~(1U << 5) do?

My answer:









Reference answer:

It clears bit 5 of the GPIO output register.

```cpp
GPIO_ODR &= ~(1U << 5);
```

In a real embedded system, this could turn off a GPIO pin or LED.

Simple version:

```text
It clears bit 5 to 0.
```

---

## How do sensor-valid flags produce faults?

My answer:









Reference answer:

Each sensor-valid flag is checked with a mask.

If the bit is not set, that sensor is considered invalid.

Example:

```cpp
if (!data.sensor1_valid) {
    std::cout << "FAULT: Sensor 1 invalid" << std::endl;
}
```

Simple version:

```text
Missing valid bits become sensor-invalid faults.
```

---

## What does status byte 0x00 mean for sensor-valid flags?

My answer:









Reference answer:

`0x00` is binary:

```text
0000 0000
```

So none of the sensor-valid bits are set.

That means:

```text
sensor 1 invalid
sensor 2 invalid
sensor 3 invalid
```

Simple version:

```text
0x00 means no sensors are marked valid.
```

---

## How does Sum of Two Integers work without + or -?

My answer:









Reference answer:

It uses bit operations.

```text
XOR = sum without carry
AND = carry bits
carry << 1 = move carry to next position
```

Repeat until there is no carry.

Simple version:

```text
Use XOR for the sum and AND-shift for the carry.
```

---

## Why does XOR represent addition without carry?

My answer:









Reference answer:

XOR gives `1` when bits are different and `0` when bits are the same.

Binary addition without carry behaves the same way:

```text
0 + 0 = 0
1 + 0 = 1
0 + 1 = 1
1 + 1 = 0 with carry
```

The `1 + 1` case needs a carry, which is handled separately with AND.

Simple version:

```text
XOR gives the bit result of addition before handling carry.
```

---

## Week 4 Day 5 Core Interview Summary

My answer:









Reference answer:

`volatile` tells the compiler that a variable can change outside normal program flow, which is important for hardware registers and ISR-shared flags. It does not provide thread safety. In the CAN decoder, missing sensor-valid bits are interpreted as sensor faults and reported by `FaultAnalyzer`.
---

# Week 4 Day 6 — Fault Rules Documentation and Week 4 Review

## Interview questions

```text
1. What is a bit mask?
2. How do you set a bit?
3. How do you clear a bit?
4. How do you check a bit?
5. What is XOR useful for?
6. What is volatile?
7. Is volatile thread-safe?
8. Why does fault detection come after decoding?
9. Why are status flags useful in CAN messages?
10. What does constexpr do?
```

---

## What is a bit mask?

My answer:









Reference answer:

A bit mask is a value used to target one or more specific bits.

Example:

```cpp
std::uint8_t mask = 0x04;
```

Binary:

```text
0x04 = 0000 0100
```

A mask lets code isolate, set, clear, toggle, or check selected bits.

Simple version:

```text
A bit mask lets me work with specific bits inside a value.
```

---

## How do you set a bit?

My answer:









Reference answer:

Use bitwise OR.

```cpp
value |= mask;
```

This forces the selected bit to become `1`.

Example:

```text
value = 0000 0000
mask  = 0000 0100

result = 0000 0100
```

Simple version:

```text
Set means force a bit to 1.
```

---

## How do you clear a bit?

My answer:









Reference answer:

Use bitwise AND with the inverted mask.

```cpp
value &= ~mask;
```

This forces the selected bit to become `0`.

Example:

```text
value = 0000 0100
mask  = 0000 0100
~mask = 1111 1011

result = 0000 0000
```

Simple version:

```text
Clear means force a bit to 0.
```

---

## How do you check a bit?

My answer:









Reference answer:

Use bitwise AND.

```cpp
(value & mask) != 0
```

If the result is not zero, the selected bit is set.

Example:

```text
value = 0000 0100
mask  = 0000 0100

value & mask = 0000 0100
```

Simple version:

```text
Checking a bit means testing whether a specific bit is 1.
```

---

## What is XOR useful for?

My answer:









Reference answer:

XOR is useful for toggling bits and solving cancellation problems.

Important rules:

```text
x ^ x = 0
x ^ 0 = x
```

Examples:

```text
toggle a bit
find the single number
find a missing number
combine bitwise addition without carry
```

Simple version:

```text
XOR is useful when matching pairs should cancel out or when bits need to flip.
```

---

## What is volatile?

My answer:









Reference answer:

`volatile` tells the compiler that a value can change outside normal program flow.

Common examples:

```text
hardware registers
ISR-shared flags
status registers
GPIO registers
timer counters
```

Simple version:

```text
volatile tells the compiler not to assume the value stays unchanged.
```

---

## Is volatile thread-safe?

My answer:









Reference answer:

No.

`volatile` is not thread-safe.

It does not make operations atomic and does not protect against race conditions.

Example:

```cpp
counter++;
```

This still has multiple steps:

```text
read
modify
write
```

An interrupt or another thread could interfere between those steps.

Simple version:

```text
volatile controls compiler optimization, not data safety.
```

---

## Why does fault detection come after decoding?

My answer:









Reference answer:

Fault detection comes after decoding because fault rules depend on meaningful values.

Examples:

```text
battery bytes must become volts
temperature bytes must become Celsius
status bytes must become flags
```

The program must decode raw CAN payload bytes before it can apply fault rules.

Simple version:

```text
The program must understand the bytes before it can judge whether they are faulty.
```

---

## Why are status flags useful in CAN messages?

My answer:









Reference answer:

Status flags are useful because they pack many true/false conditions into one byte.

Examples:

```text
sensor valid
low voltage
overtemperature
communication fault
error active
mode bit
```

This is efficient for embedded systems because CAN payload space is limited.

Simple version:

```text
Status flags are compact and efficient for embedded messages.
```

---

## What does constexpr do?

My answer:









Reference answer:

`constexpr` creates a compile-time constant.

It is useful for fixed values like:

```text
bit masks
CAN IDs
array sizes
limits
thresholds
```

Example:

```cpp
constexpr std::uint8_t SENSOR1_VALID_MASK = 0x01;
```

Simple version:

```text
constexpr gives a meaningful name to a value known at compile time.
```

---

## Week 4 Core Interview Summary

My answer:









Reference answer:

Week 4 focused on bit manipulation and fault detection. I learned how to use masks to set, clear, toggle, and check bits. I used `constexpr` masks to interpret CAN status bytes, decoded status frame `0x102`, and created a `FaultAnalyzer` to separate fault checking from byte decoding. I also learned that `volatile` is important for hardware registers but does not provide thread safety.
---

# Week 5 Day 1 — Recursion, DecoderStats, and CAN Wiring

## Interview questions

```text
1. What is recursion?
2. What is a base case?
3. What is a recursive case?
4. What is the call stack?
5. How does Maximum Depth of Binary Tree use recursion?
6. What is DecoderStats?
7. Why track decoder statistics?
8. What is CANH/CANL?
9. Why does CAN need a transceiver?
10. Why does the PC need a USB-CAN adapter?
11. Why does CAN wiring need common ground?
12. Why does CAN need 120 ohm termination?
```

---

## What is recursion?

My answer:









Reference answer:

Recursion is when a function calls itself to solve a smaller version of the same problem.

Example:

```cpp
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
```

Simple version:

```text
Recursion means a function solves a problem by calling itself on a smaller problem.
```

---

## What is a base case?

My answer:









Reference answer:

A base case is the condition that stops recursion.

Example:

```cpp
if (n <= 1) return 1;
```

Without a base case, recursion would continue forever.

Simple version:

```text
The base case is the stopping condition.
```

---

## What is a recursive case?

My answer:









Reference answer:

The recursive case is where the function calls itself.

Example:

```cpp
return n * factorial(n - 1);
```

This reduces the problem from `n` to `n - 1`.

Simple version:

```text
The recursive case keeps breaking the problem into smaller pieces.
```

---

## What is the call stack?

My answer:









Reference answer:

The call stack stores unfinished function calls.

In recursion, each recursive call is placed on the call stack until the base case is reached.

Then the calls return back up.

Simple version:

```text
The call stack keeps track of recursive function calls that are waiting to finish.
```

---

## How does Maximum Depth of Binary Tree use recursion?

My answer:









Reference answer:

Maximum Depth of Binary Tree uses recursion by finding the depth of the left subtree and the right subtree.

Then it returns:

```cpp
1 + max(left_depth, right_depth)
```

The base case is:

```cpp
if (root == nullptr) return 0;
```

Simple version:

```text
The function asks each subtree for its depth, then adds 1 for the current node.
```

---

## What is DecoderStats?

My answer:









Reference answer:

`DecoderStats` tracks high-level decoder results.

It tracks:

```text
total frames
valid frames
invalid DLC frames
unknown ID frames
fault count
```

Simple version:

```text
DecoderStats summarizes what happened while processing CAN frames.
```

---

## Why track decoder statistics?

My answer:









Reference answer:

Decoder statistics make the tool more useful because they summarize the whole run.

Example:

```text
Decoder Stats:
Total frames: 5
Valid frames: 3
Invalid DLC frames: 1
Unknown ID frames: 1
Fault count: 5
```

Simple version:

```text
Stats turn individual frame logs into a useful diagnostic summary.
```

---

## What is CANH/CANL?

My answer:









Reference answer:

CANH and CANL are the two physical CAN bus wires.

```text
CANH = CAN high
CANL = CAN low
```

CAN uses the voltage difference between these two wires to transmit data.

Simple version:

```text
CANH and CANL are the differential signal wires of the CAN bus.
```

---

## Why does CAN need a transceiver?

My answer:









Reference answer:

The STM32 FDCAN peripheral produces digital TX/RX logic signals.

The CAN bus uses differential CANH/CANL physical signals.

A transceiver converts between the two.

Simple version:

```text
The transceiver converts microcontroller logic into physical CAN bus signals.
```

---

## Why does the PC need a USB-CAN adapter?

My answer:









Reference answer:

A normal PC does not directly connect to CANH/CANL.

The USB-CAN adapter converts CAN bus traffic into USB communication that the PC can read.

Simple version:

```text
The USB-CAN adapter lets the PC send, receive, and log CAN frames.
```

---

## Why does CAN wiring need common ground?

My answer:









Reference answer:

The STM32 board, CAN transceiver, and USB-CAN adapter should share a common ground.

Common ground gives all devices the same electrical reference.

Simple version:

```text
Common ground helps devices agree on voltage levels.
```

---

## Why does CAN need 120 ohm termination?

My answer:









Reference answer:

A CAN bus normally needs 120 ohm termination at both ends of the bus.

Termination reduces signal reflections and helps keep communication reliable.

Simple version:

```text
Termination helps keep the CAN signal clean.
```

---

## Week 5 Day 1 Core Interview Summary

My answer:









Reference answer:

Recursion solves problems by breaking them into smaller versions of the same problem until a base case is reached. In the CAN project, `DecoderStats` summarizes frame processing results, while the CAN wiring plan explains how STM32 FDCAN logic connects through a transceiver to the physical CAN bus and then to the PC through a USB-CAN adapter.
---

# Week 5 Day 2 — Invert Tree and Wire CAN Bridge

## Interview questions

```text
1. What did Invert Tree teach you about recursion?
2. What is the base case in Invert Tree?
3. Why does recursion fit binary trees well?
4. What is DecoderStats responsible for?
5. Why increment stats during frame processing?
6. What is CAN termination?
7. What resistance should you measure with two 120 ohm terminations?
8. Why must CAN nodes share ground?
9. Why does the STM32 need the SN65HVD230?
10. What wiring mistake would break CAN?
```

---

## What did Invert Tree teach you about recursion?

My answer:









Reference answer:

Invert Tree teaches that recursion works well when each part of a structure is a smaller version of the whole structure.

Each subtree can be inverted the same way as the full tree.

Simple version:

```text
Invert Tree shows that recursive logic can be applied to each smaller subtree.
```

---

## What is the base case in Invert Tree?

My answer:









Reference answer:

The base case is:

```cpp
if (root == nullptr) {
    return nullptr;
}
```

If there is no node, there is nothing to invert.

Simple version:

```text
The base case is an empty node.
```

---

## Why does recursion fit binary trees well?

My answer:









Reference answer:

Recursion fits binary trees well because each child is the root of a smaller tree.

That means the same function can solve the left subtree and right subtree.

Simple version:

```text
A tree is made of smaller trees, so recursion matches its structure.
```

---

## What is DecoderStats responsible for?

My answer:









Reference answer:

`DecoderStats` records high-level frame processing results.

It tracks:

```text
total frames
valid frames
invalid DLC frames
unknown ID frames
fault count
```

Simple version:

```text
DecoderStats summarizes what happened during frame processing.
```

---

## Why increment stats during frame processing?

My answer:









Reference answer:

Stats should update when each frame is checked so the final summary accurately reflects the decoder run.

Example:

```cpp
stats.record_frame_received();

if (!is_known_id(frame.id)) {
    stats.record_unknown_id();
}

if (!has_valid_dlc(frame)) {
    stats.record_invalid_dlc();
}
```

Simple version:

```text
Stats are updated during processing so the final summary matches the actual frames.
```

---

## What is CAN termination?

My answer:









Reference answer:

CAN termination is a resistor placed across CANH and CANL at the ends of the CAN bus.

The typical value is:

```text
120 ohms at each end
```

Termination reduces signal reflections.

Simple version:

```text
Termination helps keep the CAN signal clean.
```

---

## What resistance should you measure with two 120 ohm terminations?

My answer:









Reference answer:

With power off, two 120 ohm terminations in parallel should measure about:

```text
60 ohms
```

Simple version:

```text
Two 120 ohm resistors in parallel measure about 60 ohms.
```

---

## Why must CAN nodes share ground?

My answer:









Reference answer:

CAN uses differential signaling, but the nodes still need a shared electrical reference.

The NUCLEO, transceiver, and USB-CAN adapter should share ground.

Simple version:

```text
Common ground helps devices agree on voltage levels.
```

---

## Why does the STM32 need the SN65HVD230?

My answer:









Reference answer:

The STM32 FDCAN peripheral uses digital TX/RX logic signals.

The CAN bus uses physical differential CANH/CANL signals.

The SN65HVD230 converts between the two.

Simple version:

```text
The transceiver converts STM32 logic signals into CAN bus signals.
```

---

## What wiring mistake would break CAN?

My answer:









Reference answer:

Common wiring mistakes include:

```text
CANH and CANL swapped
missing common ground
missing termination
too much termination
wrong bitrate
transceiver not powered
wrong FDCAN pins selected
STM32 TX/RX connected directly to CANH/CANL
```

Simple version:

```text
CAN breaks if the physical wiring, ground, termination, bitrate, or transceiver connection is wrong.
```

---

## Week 5 Day 2 Core Interview Summary

My answer:









Reference answer:

Invert Tree shows that recursion works well when a structure is made of smaller versions of itself. For CAN wiring, the STM32 does not connect directly to CANH/CANL. It uses a transceiver, shared ground, matching bitrate, and proper termination to communicate with a USB-CAN adapter and PC.
---

# Week 5 Day 3 — Same Tree, Symmetric Tree, and STM32 FDCAN Configuration

## Interview questions

```text
1. How does recursive comparison work?
2. How does Same Tree compare two trees?
3. How does Symmetric Tree compare a tree?
4. What is the difference between Same Tree and Symmetric Tree?
5. What are the battery voltage fault thresholds?
6. Why does voltage fault checking happen after decoding?
7. What is FDCAN/CAN configuration?
8. What bitrate are you using?
9. What frame ID are you sending first?
10. What payload are you sending for 0x100?
```

---

## How does recursive comparison work?

My answer:









Reference answer:

Recursive comparison checks two nodes at the same time.

The base cases are:

```text
both null = match
one null = not match
different values = not match
```

If the current nodes match, the function compares their child nodes recursively.

Simple version:

```text
Recursive comparison checks the current nodes, then checks smaller subtree pairs.
```

---

## How does Same Tree compare two trees?

My answer:









Reference answer:

Same Tree compares two trees in matching positions.

It compares:

```text
p node vs q node
p left vs q left
p right vs q right
```

Simple version:

```text
Same Tree checks whether two trees have the same structure and values.
```

---

## How does Symmetric Tree compare a tree?

My answer:









Reference answer:

Symmetric Tree compares the left side and right side of one tree as mirrors.

It compares:

```text
left node vs right node
left outer child vs right outer child
left inner child vs right inner child
```

In code, that means:

```cpp
isMirror(left->left, right->right);
isMirror(left->right, right->left);
```

Simple version:

```text
Symmetric Tree checks whether the tree mirrors itself.
```

---

## What is the difference between Same Tree and Symmetric Tree?

My answer:









Reference answer:

Same Tree checks whether two trees are identical.

Symmetric Tree checks whether one tree is mirrored around its center.

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

Simple version:

```text
Same Tree checks identical structure, while Symmetric Tree checks mirror structure.
```

---

## What are the battery voltage fault thresholds?

My answer:









Reference answer:

The current voltage fault thresholds are:

```text
battery < 10.5 V = low voltage fault
battery > 14.8 V = high voltage fault
```

Output examples:

```text
FAULT: Battery voltage too low
FAULT: Battery voltage too high
```

Simple version:

```text
Below 10.5 V is low, and above 14.8 V is high.
```

---

## Why does voltage fault checking happen after decoding?

My answer:









Reference answer:

Voltage fault checking happens after decoding because the raw CAN bytes must first be converted into a real voltage.

The decoder does:

```text
battery_mV = bytes 0-1
battery_V = battery_mV / 1000.0
```

Then the fault analyzer checks the voltage limits.

Simple version:

```text
The program must convert raw bytes into volts before checking voltage faults.
```

---

## What is FDCAN/CAN configuration?

My answer:









Reference answer:

FDCAN/CAN configuration means setting up the STM32 CAN peripheral so it can transmit and receive CAN frames correctly.

Configuration includes:

```text
FDCAN pins
bitrate
frame format
standard or extended ID
DLC
transmit mode
filters
```

Simple version:

```text
CAN configuration makes sure the STM32 and USB-CAN adapter use compatible CAN settings.
```

---

## What bitrate are you using?

My answer:









Reference answer:

The planned bitrate is:

```text
500 kbps
```

Both the STM32 FDCAN peripheral and the Waveshare USB-CAN adapter must use the same bitrate.

Simple version:

```text
The CAN bus is planned for 500 kbps.
```

---

## What frame ID are you sending first?

My answer:









Reference answer:

The first transmitted frame ID is:

```text
0x100
```

This is the analog input frame.

Simple version:

```text
The first CAN frame sent from the STM32 is 0x100.
```

---

## What payload are you sending for 0x100?

My answer:









Reference answer:

The first `0x100` payload is:

```text
00 08 10 00 FF 0A 07 01
```

Decoded meaning:

```text
AIN1_RAW = 2048
AIN2_RAW = 16
AIN3_RAW = 2815
Status = 0x07
Counter = 1
```

Simple version:

```text
The payload sends three simulated analog values, valid sensor flags, and a counter.
```

---

## Week 5 Day 3 Core Interview Summary

My answer:









Reference answer:

Recursive comparison works by checking two current nodes and then applying the same logic to smaller subtree pairs. Same Tree compares matching branches, while Symmetric Tree compares mirrored branches. On the hardware side, STM32 FDCAN must be configured with matching bitrate, correct TX/RX pins, standard ID format, DLC 8, and a first transmit frame of `0x100` with payload `00 08 10 00 FF 0A 07 01`.
---

# Week 5 Day 4 — BFS and Send First CAN Frame

## Interview questions

```text
1. What is BFS?
2. Why does BFS use a queue?
3. What problem does Level Order Traversal solve?
4. Why do we store level_size before the for loop?
5. What is the temperature high fault rule?
6. Why does temperature fault checking happen after decoding?
7. What CAN frame did you transmit from STM32?
8. How often did the STM32 transmit the frame?
9. What should the PC receive?
10. If the PC does not receive the frame, what should you check?
```

---

## What is BFS?

My answer:









Reference answer:

BFS means Breadth-First Search.

It visits nodes level by level before going deeper.

Simple version:

```text
BFS explores the current level before the next level.
```

---

## Why does BFS use a queue?

My answer:









Reference answer:

BFS uses a queue because a queue is first in, first out.

The first node discovered is the first node processed.

Simple version:

```text
A queue keeps nodes in discovery order.
```

---

## What problem does Level Order Traversal solve?

My answer:









Reference answer:

Level Order Traversal returns the values of a binary tree grouped by level.

Example output:

```text
[
  [3],
  [9, 20],
  [15, 7]
]
```

Simple version:

```text
It reads a tree from top to bottom, level by level.
```

---

## Why do we store level_size before the for loop?

My answer:









Reference answer:

We store `level_size` because the queue changes while processing the current level.

As nodes are processed, their children are added to the queue.

`level_size` tells us how many nodes belong to the current level before adding the next level.

Simple version:

```text
level_size separates the current level from the next level.
```

---

## What is the temperature high fault rule?

My answer:









Reference answer:

The temperature high fault rule is:

```text
temperature > 80 C = high temperature fault
```

Output:

```text
FAULT: Temperature too high
```

Simple version:

```text
Temperature above 80 C is reported as a fault.
```

---

## Why does temperature fault checking happen after decoding?

My answer:









Reference answer:

Temperature fault checking happens after decoding because the raw CAN bytes must be converted into Celsius first.

The decoder converts:

```text
temperature_deciC -> temperature_C
```

Then the fault analyzer checks whether the value is above 80 C.

Simple version:

```text
The program must convert the raw bytes into Celsius before checking the limit.
```

---

## What CAN frame did you transmit from STM32?

My answer:









Reference answer:

The STM32 transmitted standard CAN frame:

```text
ID: 0x100
DLC: 8
Data: 00 08 10 00 FF 0A 07 01
```

Simple version:

```text
The STM32 transmitted the 0x100 analog input frame.
```

---

## How often did the STM32 transmit the frame?

My answer:









Reference answer:

The STM32 transmitted the frame every:

```text
100 ms
```

That is about 10 frames per second.

Simple version:

```text
The STM32 sends the test frame every 100 ms.
```

---

## What should the PC receive?

My answer:









Reference answer:

The PC should receive:

```text
ID: 0x100
DLC: 8
Data: 00 08 10 00 FF 0A 07 01
```

The frame should repeat about every 100 ms in the Waveshare USB-CAN receive window.

Simple version:

```text
The PC should see repeated 0x100 frames with the expected 8-byte payload.
```

---

## If the PC does not receive the frame, what should you check?

My answer:









Reference answer:

Check:

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

Simple version:

```text
Check bitrate, wiring, ground, termination, power, transceiver mode, and firmware execution.
```

---

## Week 5 Day 4 Core Interview Summary

My answer:









Reference answer:

BFS uses a queue to process nodes in the order they are discovered, which allows level-by-level traversal. On the hardware side, the STM32 sends standard CAN frame `0x100` every 100 ms through the transceiver, and the PC should receive it through the Waveshare USB-CAN adapter if bitrate, wiring, ground, and termination are correct.