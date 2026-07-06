# Week 1 Notes — C++ CAN Telemetry Decoder

## Project

Repo name:

```text
cpp-can-telemetry-decoder
```

Main goal:

```text
Build C++ fundamentals while slowly creating a CAN telemetry decoder project.
```

This project connects C++ practice to embedded systems by using:

```text
functions
vectors
strings
hash maps
references
const correctness
structs
fixed-width integers
hex/binary
bit shifting
bit masking
CAN frame decoding
```

---

# Day 1 — Setup + first C++ reps

## Main goals

```text
Install/configure C++ compiler.
Create GitHub repo.
Create main.cpp.
Write hello-world.
Write a function that adds two integers.
Write a function that takes const std::vector<int>&.
Solve LeetCode 1: Two Sum.
Start notes/week1.md.
```

---

## Basic C++ program structure

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, C++" << std::endl;
    return 0;
}
```

Meaning:

```text
#include <iostream> gives access to input/output tools.
int main() is where the program starts.
std::cout prints to the terminal.
return 0 means the program ended successfully.
```

---

## Function that adds two integers

```cpp
int add(int a, int b) {
    return a + b;
}
```

Example use:

```cpp
int result = add(3, 4);
std::cout << result << std::endl;
```

Output:

```text
7
```

---

## Function that takes a vector by const reference

```cpp
int sumVector(const std::vector<int>& nums) {
    int sum = 0;

    for (int value : nums) {
        sum += value;
    }

    return sum;
}
```

Important part:

```cpp
const std::vector<int>& nums
```

Meaning:

```text
const = function cannot modify the vector
std::vector<int> = vector of integers
& = pass by reference, so the vector is not copied
nums = parameter name
```

Use this when I want to read a vector without copying it.

---

## Compile from terminal

Command:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o main
```

Run on Windows PowerShell:

```powershell
.\main.exe
```

Important compiler flags:

```text
-std=c++17 = use C++17
-Wall = show common warnings
-Wextra = show extra warnings
-o main = name the output program main
```

---

## Git commands

```bash
git status
git add .
git commit -m "Day 1 setup and basic C++ functions"
git push
```

---

## LeetCode 1 — Two Sum

Pattern:

```text
Hash map
```

Main idea:

```text
For each number, calculate target - current number.
Check if the needed number already exists in the hash map.
If yes, return the two indices.
If no, store the current number and its index.
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

# Day 2 — Vectors, loops, and arrays

## Main topic

Today I practiced:

```text
std::vector
indexing
range-based loops
regular for-loops
basic sensor/ADC data processing
```

This connects to embedded work because sensor readings, CAN payloads, ADC values, and buffers are often stored and processed as arrays or array-like structures.

---

## `std::vector`

A vector is like a flexible array. Unlike a normal array, it can grow or shrink.

```cpp
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5};
```

Access values using indexing:

```cpp
nums[0]; // first element
nums[1]; // second element
```

Get the size:

```cpp
nums.size();
```

Add a value:

```cpp
nums.push_back(6);
```

---

## Range-based loop

This is a cleaner way to loop through every value in a vector.

```cpp
for (int value : nums) {
    std::cout << value << std::endl;
}
```

This means:

```text
For every int value inside nums, run the code inside the loop.
```

---

## Regular for-loop

A regular loop is useful when I need the index.

```cpp
for (int i = 0; i < nums.size(); i++) {
    std::cout << nums[i] << std::endl;
}
```

Use this when I care about positions like:

```text
i
i + 1
i - 1
neighboring values
```

---

## Passing vectors into functions

```cpp
int findMin(const std::vector<int>& readings)
```

Meaning:

```text
const = the function cannot change the vector
std::vector<int> = vector of integers
& = pass by reference, so the vector is not copied
readings = parameter name
```

This is better than copying the whole vector.

---

## ADC readings program

ADC readings are usually integer values from a sensor input. For example, a 10-bit ADC gives values from 0 to 1023.

Example:

```cpp
std::vector<int> adcReadings = {512, 600, 498, 700, 650, 620, 580, 560, 610, 590};
```

Things I practiced:

```text
Find minimum reading
Find maximum reading
Find average reading
Loop through all values
Use functions to organize code
```

---

## Finding minimum

```cpp
int minimum = readings[0];

for (int value : readings) {
    if (value < minimum) {
        minimum = value;
    }
}
```

Logic:

```text
Start by assuming the first value is the minimum.
Go through every value.
If I find something smaller, update minimum.
```

---

## Finding maximum

```cpp
int maximum = readings[0];

for (int value : readings) {
    if (value > maximum) {
        maximum = value;
    }
}
```

Logic:

```text
Start by assuming the first value is the maximum.
Go through every value.
If I find something bigger, update maximum.
```

---

## Finding average

```cpp
int sum = 0;

for (int value : readings) {
    sum += value;
}

double average = static_cast<double>(sum) / readings.size();
```

Important part:

```cpp
static_cast<double>(sum)
```

This converts `sum` from `int` to `double` so the division gives a decimal answer.

Without this, C++ might do integer division.

---

## Header files

I created:

```text
include/can_frame.hpp
```

A header file lets me separate code into different files instead of putting everything in `main.cpp`.

In `main.cpp`, I include it with:

```cpp
#include "can_frame.hpp"
```

Because the file is inside the `include` folder, I compile with:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp -o main
```

Important part:

```bash
-Iinclude
```

This tells the compiler to look inside the `include` folder for header files.

---

## CAN frame struct

```cpp
struct CanFrame {
    std::uint32_t id;
    std::uint8_t dlc;
    std::array<std::uint8_t, 8> data;
};
```

Meaning:

```text
id = CAN message ID
dlc = data length code, how many bytes are used
data = 8-byte CAN payload
```

A CAN frame is basically a small packet of data sent on a CAN bus.

---

## Why `std::uint8_t` and `std::uint32_t`?

These are fixed-width integer types.

```cpp
std::uint8_t
```

means unsigned 8-bit integer.

```cpp
std::uint32_t
```

means unsigned 32-bit integer.

This matters in embedded systems because hardware data sizes need to be exact.

---

## LeetCode 217 — Contains Duplicate

Pattern:

```text
Hash set
```

Main idea:

```text
Go through each number.
If I have already seen it, return true.
If not, store it.
If I finish the loop, return false.
```

Useful C++ tool:

```cpp
std::unordered_set<int> seen;
```

Check if value exists:

```cpp
if (seen.count(num)) {
    return true;
}
```

Add value:

```cpp
seen.insert(num);
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

## LeetCode 121 — Best Time to Buy and Sell Stock

Pattern:

```text
Track best value so far
```

Main idea:

```text
Keep track of the lowest price seen so far.
For each day, calculate the profit if I sold today.
Update max profit if this profit is better.
```

Variables:

```cpp
int minPrice = prices[0];
int maxProfit = 0;
```

Logic:

```cpp
if (prices[i] < minPrice) {
    minPrice = prices[i];
} else {
    int profit = prices[i] - minPrice;

    if (profit > maxProfit) {
        maxProfit = profit;
    }
}
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

# Day 3 — Strings and hash maps

## Main topic

Today I practiced:

```text
std::string
std::unordered_map
character frequency counting
Valid Anagram
fake CAN frame printing
```

---

## `std::string`

A string stores text.

```cpp
std::string word = "telemetry";
```

Access characters like an array:

```cpp
word[0]; // first character
word[1]; // second character
```

Get length:

```cpp
word.length();
```

or:

```cpp
word.size();
```

Loop through a string:

```cpp
for (char c : word) {
    std::cout << c << std::endl;
}
```

---

## `std::unordered_map`

An unordered map stores key-value pairs.

```cpp
std::unordered_map<char, int> frequency;
```

Meaning:

```text
key = char
value = int
```

Example:

```cpp
frequency['a'] = 3;
```

Meaning:

```text
The character 'a' appears 3 times.
```

---

## Frequency counter

```cpp
std::unordered_map<char, int> frequency;

for (char c : text) {
    frequency[c]++;
}
```

Important trick:

```cpp
frequency[c]++;
```

If `c` is not already in the map, C++ creates it with value `0`, then adds `1`.

---

## Looping through an unordered map

```cpp
for (const auto& pair : frequency) {
    std::cout << pair.first << ": " << pair.second << std::endl;
}
```

Meaning:

```text
pair.first = key
pair.second = value
```

For example:

```text
a: 3
```

means the key `'a'` has value `3`.

---

## LeetCode 242 — Valid Anagram

Pattern:

```text
Hash map / frequency counter
```

Main idea:

```text
If the strings have different lengths, they cannot be anagrams.
Count every character in the first string.
Subtract every character from the second string.
If any count becomes negative, it is not an anagram.
If everything balances, it is an anagram.
```

Example logic:

```cpp
bool isAnagram(const std::string& s, const std::string& t) {
    if (s.length() != t.length()) {
        return false;
    }

    std::unordered_map<char, int> frequency;

    for (char c : s) {
        frequency[c]++;
    }

    for (char c : t) {
        frequency[c]--;

        if (frequency[c] < 0) {
            return false;
        }
    }

    return true;
}
```

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(1) if only lowercase English letters
O(n) more generally
```

---

## Fake CAN frame

Example:

```cpp
CanFrame frame = {
    0x100,
    8,
    {0x34, 0x12, 0x78, 0x56, 0x00, 0x01, 0xAA, 0x55}
};
```

Meaning:

```text
CAN ID = 0x100
DLC = 8
Data bytes = 8 payload bytes
```

---

# Day 4 — References, const, and functions

## Main idea

Today I practiced passing vectors into functions in three ways:

```text
Pass by value
Pass by reference
Pass by const reference
```

This matters in C++ because large objects like vectors and structs can be expensive to copy.

---

## Pass by value

```cpp
void takeByValue(std::vector<int> nums)
```

This makes a copy of the vector.

If I change `nums` inside the function, the original vector does not change.

Use this when I actually want a copy.

---

## Pass by reference

```cpp
void takeByReference(std::vector<int>& nums)
```

This does not copy the vector.

The function works directly with the original vector.

If I change `nums` inside the function, the original vector changes too.

Use this when the function is supposed to modify the original data.

---

## Pass by const reference

```cpp
void takeByConstReference(const std::vector<int>& nums)
```

This does not copy the vector.

The function can read the original vector, but it cannot modify it.

This is usually the best choice when I only need to read a large object.

---

## Rule of thumb

```text
Small values like int or char: pass by value.
Large objects like vector, string, or structs: pass by const reference if only reading.
Use non-const reference only when the function needs to modify the original.
```

---

## CAN frame print function

```cpp
void print_frame(const CanFrame& frame)
```

Meaning:

```text
The function prints the CAN frame without copying it and without changing it.
```

This is useful because embedded code often passes structs around by reference.

---

## LeetCode 283 — Move Zeroes

Problem:

```text
Move all zeroes to the end while keeping the order of nonzero values.
```

Pattern:

```text
Two-pointer / overwrite position
```

Main idea:

```text
Keep an insert position.
Copy each nonzero value to the insert position.
After all nonzero values are placed, fill the rest with zeroes.
```

Solution idea:

```cpp
void moveZeroes(std::vector<int>& nums) {
    int insertPosition = 0;

    for (int num : nums) {
        if (num != 0) {
            nums[insertPosition] = num;
            insertPosition++;
        }
    }

    while (insertPosition < nums.size()) {
        nums[insertPosition] = 0;
        insertPosition++;
    }
}
```

Important part:

```cpp
std::vector<int>& nums
```

The `&` means the function modifies the original vector directly.

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(1)
```

---

# Day 5 — Hex, binary, and byte packing

## Main topic

Today I practiced:

```text
std::uint8_t
std::uint16_t
std::uint32_t
hex
binary
bit shifting
bitwise OR
bitwise AND
byte packing
bit checking
CAN analog input decoding
Reverse String
```

This is one of the most important days for embedded/CAN work.

---

## Fixed-width integer types

```cpp
std::uint8_t
```

means unsigned 8-bit integer.

```cpp
std::uint16_t
```

means unsigned 16-bit integer.

```cpp
std::uint32_t
```

means unsigned 32-bit integer.

These are useful when data size must be exact.

Example:

```text
CAN data byte = 8 bits
ADC raw value might be 16 bits
CAN ID might fit in 32 bits
```

---

## Decimal, binary, and hex

```text
Decimal = base 10
Binary = base 2
Hex = base 16
```

Example:

```text
Decimal: 10
Binary:  0b1010
Hex:     0x0A
```

Important relationship:

```text
1 hex digit = 4 bits
2 hex digits = 1 byte
```

Example:

```text
0xFF = 11111111 binary = 255 decimal
```

---

## Bitwise operators

Left shift:

```cpp
value << 8
```

Moves bits left by 8 positions.

Right shift:

```cpp
value >> 1
```

Moves bits right by 1 position.

Bitwise OR:

```cpp
a | b
```

Combines bits. If either bit is 1, result is 1.

Bitwise AND:

```cpp
a & b
```

Checks bits. Result is 1 only if both bits are 1.

---

## Byte packing

Function:

```cpp
std::uint16_t pack_u16(std::uint8_t low, std::uint8_t high) {
    return static_cast<std::uint16_t>(low) |
           (static_cast<std::uint16_t>(high) << 8);
}
```

This combines two 8-bit bytes into one 16-bit value.

Example:

```text
low  = 0x34
high = 0x12
```

Then:

```text
high << 8 = 0x1200
low       = 0x0034
combined  = 0x1234
```

So:

```cpp
pack_u16(0x34, 0x12)
```

gives:

```text
0x1234
```

This is little-endian byte order because the low byte comes first.

---

## Bit checking

Function:

```cpp
bool get_bit(std::uint8_t value, int bit_position) {
    return (value & (1 << bit_position)) != 0;
}
```

Example:

```cpp
std::uint8_t flags = 0b00000101;
```

Bits:

```text
Bit 0 = 1
Bit 1 = 0
Bit 2 = 1
```

So:

```cpp
get_bit(flags, 0); // true
get_bit(flags, 1); // false
get_bit(flags, 2); // true
```

This is useful for embedded status flags like:

```text
Bit 0: sensor fault
Bit 1: overvoltage
Bit 2: CAN error
Bit 3: system ready
```

---

## CAN analog input decoding

A CAN payload is just bytes.

Example 8-byte frame:

```text
Byte 0-1: AIN1_RAW
Byte 2-3: AIN2_RAW
Byte 4-5: AIN3_RAW
Byte 6: flags
Byte 7: status
```

Each analog input is 16 bits, so it takes 2 bytes.

Example:

```cpp
std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);
```

Example data:

```text
frame.data[0] = 0x34
frame.data[1] = 0x12
```

Decoded value:

```text
AIN1_RAW = 0x1234 = 4660 decimal
```

---

## Why check DLC?

Before decoding AIN1, AIN2, and AIN3, the frame needs at least 6 bytes.

```cpp
if (frame.dlc < 6) {
    std::cout << "Frame does not have enough bytes." << std::endl;
    return;
}
```

Reason:

```text
AIN1 uses bytes 0 and 1.
AIN2 uses bytes 2 and 3.
AIN3 uses bytes 4 and 5.
That means I need at least 6 bytes.
```

Without this check, the program might read data that does not exist.

---

## LeetCode 344 — Reverse String

Pattern:

```text
Two pointers
```

Main idea:

```text
Use one pointer at the left.
Use one pointer at the right.
Swap the characters.
Move left forward.
Move right backward.
Stop when left >= right.
```

Solution:

```cpp
void reverseString(std::vector<char>& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;

    while (left < right) {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;

        left++;
        right--;
    }
}
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

# Day 6 — Review and interview practice

## Main goals

```text
Redo Two Sum without looking.
Redo Best Time to Buy and Sell Stock without looking.
Redo Move Zeroes without looking if time allows.
Explain CAN payload packing out loud.
Push repo to GitHub.
Write Week 1 summary.
Practice Week 1 interview questions out loud.
```

---

## Review: Two Sum

Pattern:

```text
Hash map
```

Main idea:

```text
Use a hash map.
For each number, check if target - current number already exists.
If yes, return the two indices.
If not, store current number and index.
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

## Review: Best Time to Buy and Sell Stock

Pattern:

```text
Track best value so far
```

Main idea:

```text
Track the lowest price so far.
For each price, calculate the profit if I sold today.
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

## Review: Move Zeroes

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

## Review: CAN payload packing explanation

A CAN payload is made of bytes. If one sensor value is 16 bits, it needs two 8-bit bytes.

In little-endian order:

```text
low byte comes first
high byte comes second
```

To rebuild the 16-bit value:

```cpp
std::uint16_t value =
    static_cast<std::uint16_t>(data[0]) |
    (static_cast<std::uint16_t>(data[1]) << 8);
```

Example:

```text
data[0] = 0x34
data[1] = 0x12

data[1] << 8 = 0x1200
data[0]      = 0x0034

final value  = 0x1234
```

---

# Week 1 Summary

This week I set up my C++ development environment and started building the `cpp-can-telemetry-decoder` project.

I practiced basic C++ syntax, functions, vectors, strings, hash maps, references, const correctness, and byte-level operations.

The most important C++ concepts I learned were pass-by-value, pass-by-reference, and pass-by-const-reference. I learned that large objects like vectors and structs should usually be passed by const reference when the function only needs to read them.

I also practiced basic interview patterns including hash maps, two pointers, and tracking the best value so far. The LeetCode problems I worked on included Two Sum, Contains Duplicate, Best Time to Buy and Sell Stock, Valid Anagram, Move Zeroes, and Reverse String.

On the embedded side, I created a CAN frame struct, printed fake CAN frames, and decoded analog input values from an 8-byte CAN payload. I learned how two bytes can be packed into a 16-bit value using bit shifting and bitwise OR.

The biggest things I need to keep practicing are:

```text
C++ syntax without looking
vector references
unordered_map syntax
bitwise operations
explaining byte packing clearly
```

---

# Week 1 Interview Questions

## 1. What is the difference between C and C++?

My answer:









Reference answer:

C is mostly a procedural language. C++ supports procedural programming too, but it also adds object-oriented programming, classes, references, function overloading, templates, and the standard library.

Simple version:

```text
C++ is like C with more abstraction tools. It still gives low-level control, but it also gives features like classes, references, templates, and containers like vector and unordered_map.
```

---

## 2. What is the difference between pass-by-value and pass-by-reference?

My answer:









Reference answer:

Pass by value makes a copy.

```cpp
void function(std::vector<int> nums)
```

The original vector does not change.

Pass by reference uses the original object.

```cpp
void function(std::vector<int>& nums)
```

The function can modify the original vector.

Simple version:

```text
Pass by value copies the argument, so changes inside the function do not affect the original. Pass by reference gives the function access to the original object, so changes inside the function can affect the original.
```

---

## 3. Why use `const std::vector<int>&`?

My answer:









Reference answer:

Because it avoids copying the vector and prevents accidental changes.

```cpp
void printVector(const std::vector<int>& nums)
```

Simple version:

```text
I use const reference when I want to read a large object without copying it and without allowing the function to modify it.
```

---

## 4. What is the difference between an array and `std::vector`?

My answer:









Reference answer:

An array usually has a fixed size.

```cpp
int arr[5];
```

A vector can grow dynamically.

```cpp
std::vector<int> nums;
nums.push_back(10);
```

Simple version:

```text
An array has a fixed size and is lower-level. A std::vector is a standard library container that manages dynamic storage and can grow or shrink.
```

Embedded version:

```text
In embedded systems, arrays are common when memory needs to be predictable. Vectors are useful in normal C++ programs, but dynamic allocation may be avoided on small microcontrollers.
```

---

## 5. What is a struct?

My answer:









Reference answer:

A struct groups related data together.

```cpp
struct CanFrame {
    std::uint32_t id;
    std::uint8_t dlc;
    std::array<std::uint8_t, 8> data;
};
```

Simple version:

```text
A struct is a user-defined type that groups related variables together. For example, a CAN frame struct can hold an ID, DLC, and data bytes in one object.
```

---

## 6. What is a byte?

My answer:









Reference answer:

A byte is 8 bits.

```text
1 byte = 8 bits
```

Example:

```text
0b10101010
```

Simple version:

```text
A byte is 8 bits. In embedded systems, bytes are important because hardware registers, CAN payloads, UART data, and memory are often handled byte by byte.
```

---

## 7. What does `data[0] | (data[1] << 8)` do?

My answer:









Reference answer:

It combines two bytes into one 16-bit value.

```cpp
data[0] | (data[1] << 8)
```

Meaning:

```text
data[0] is the low byte.
data[1] is the high byte.
data[1] gets shifted left by 8 bits.
Then OR combines both bytes.
```

Example:

```text
data[0] = 0x34
data[1] = 0x12

result = 0x1234
```

Simple version:

```text
It packs two 8-bit bytes into one 16-bit value using little-endian order.
```

---

## 8. What is the difference between decimal, hex, and binary?

My answer:









Reference answer:

They are different number bases.

```text
Decimal = base 10
Binary  = base 2
Hex     = base 16
```

Example:

```text
Decimal:  10
Binary:   0b1010
Hex:      0x0A
```

Simple version:

```text
Decimal is the normal base-10 system. Binary represents values using 0s and 1s. Hex is base 16 and is commonly used in embedded systems because it represents binary data compactly.
```

Important relationship:

```text
1 hex digit = 4 bits
2 hex digits = 1 byte
```

---

## 9. What is the time complexity of Two Sum?

My answer:









Reference answer:

Using a hash map:

```text
Time complexity: O(n)
Space complexity: O(n)
```

Simple version:

```text
Using a hash map, Two Sum is O(n) time and O(n) space because each lookup is average O(1), and we scan the vector once.
```

---

## 10. Why are fixed-width types like `uint8_t` useful in embedded systems?

My answer:









Reference answer:

Fixed-width types make the size of the data explicit and predictable.

```cpp
std::uint8_t  // exactly 8 bits
std::uint16_t // exactly 16 bits
std::uint32_t // exactly 32 bits
```

Simple version:

```text
Fixed-width types are useful because embedded code often interacts with hardware registers, communication protocols, and byte-level data. Using uint8_t or uint16_t makes the size of the data explicit and predictable.
```

This is especially important for CAN because CAN payload bytes are exactly 8-bit values.

---

# End of Week 1 Git Commands

```bash
git status
git add .
git commit -m "Complete week one C++ notes and review"
git push
```