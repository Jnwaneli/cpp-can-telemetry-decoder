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