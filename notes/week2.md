# Week 2 Notes

## Week 2 Focus

This week focuses on pointers, linked lists, CAN frame validation, simulated CAN logs, and basic embedded peripheral notes.

Main C++ topics:

```text
pointers
references
nullptr
linked lists
vectors of structs
const references
validation functions
```

Main embedded/CAN topics:

```text
CAN ID
DLC
payload validation
unknown ID faults
invalid DLC faults
GPIO basics
ADC/UART setup planning
```

---

# Day 1 — Simulated CAN Log and Pointer Basics

## Main goals

```text
Study pointer basics.
Understand value, address, pointer, dereference, and reference.
Solve LeetCode 206: Reverse Linked List.
Create a simulated CAN log using std::vector<CanFrame>.
Loop through frames using const CanFrame&.
Create STM32 GPIO/ADC/UART README.
```

---

## Pointer basics

Example:

```cpp
int x = 10;
int* p = &x;
int& r = x;
```

Meaning:

```text
x = normal integer variable
&x = address of x
p = pointer that stores the address of x
*p = dereference p, meaning go to that address and access the value
r = reference, another name for x
```

---

## Value

A value is the actual data stored in a variable.

Example:

```cpp
int x = 10;
```

The value of `x` is:

```text
10
```

---

## Address

An address is the memory location where a variable is stored.

Example:

```cpp
std::cout << &x << std::endl;
```

This prints the address of `x`.

The exact address can change each time the program runs.

---

## Pointer

A pointer stores an address.

Example:

```cpp
int* p = &x;
```

This means:

```text
p stores the address of x
```

---

## Dereference

Dereferencing means using a pointer to access the value at the address it stores.

Example:

```cpp
std::cout << *p << std::endl;
```

If `p` points to `x`, then `*p` gives the value of `x`.

Example:

```cpp
*p = 20;
```

This changes the value of `x` to `20`.

---

## Reference

A reference is another name for an existing variable.

Example:

```cpp
int& r = x;
```

This means:

```text
r is another name for x
```

So:

```cpp
r = 30;
```

also changes `x` to `30`.

---

## Pointer vs reference

A pointer stores an address and can be changed to point somewhere else.

A reference is an alias for one variable and usually cannot be changed to refer to something else.

Simple version:

```text
Pointer = stores an address
Reference = another name for an existing variable
```

---

## Why linked lists use pointers

A linked list node points to the next node.

Example:

```cpp
struct ListNode {
    int val;
    ListNode* next;
};
```

The `next` pointer stores the address of the next node.

That is why linked list problems require pointer movement.

---

## LeetCode 206 — Reverse Linked List

Pattern:

```text
Linked list pointer reversal
```

Important pointers:

```text
prev
curr
next
```

Main idea:

```text
Save the next node.
Reverse curr->next to point backward.
Move prev forward.
Move curr forward.
```

Steps:

```text
1. next = curr->next
2. curr->next = prev
3. prev = curr
4. curr = next
```

Code idea:

```cpp
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr != nullptr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}
```

Why return `prev`?

```text
At the end, curr becomes nullptr and prev points to the new head of the reversed list.
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

## Simulated CAN log

A simulated CAN log is a vector of CAN frames.

Example:

```cpp
std::vector<CanFrame> log = {
    {0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05}},
    {0x101, 8, {0x88, 0x13, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}},
    {0x102, 8, {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x999, 8, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}},
    {0x100, 4, {0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}}
};
```

This lets me test CAN decoding logic before using real CAN hardware.

---

## Looping through CAN frames

```cpp
for (const CanFrame& frame : log) {
    print_frame(frame);
}
```

Meaning:

```text
Loop through every CAN frame in the log.
Use const reference so the frame is not copied.
Do not modify the frame.
Print the frame.
```

---

## Why use `const CanFrame&`?

```cpp
const CanFrame& frame
```

Meaning:

```text
const = do not modify the frame
CanFrame& = pass by reference, so the frame is not copied
frame = parameter name
```

Simple explanation:

```text
I use const CanFrame& because I only need to read the frame. Passing by const reference avoids copying and prevents accidental changes.
```

---

## What is DLC?

DLC means Data Length Code.

It tells how many data bytes are valid in the CAN frame.

Example:

```cpp
frame.dlc = 8;
```

This means:

```text
The frame has 8 valid data bytes.
```

---

## Why use `uint8_t` for CAN data?

CAN payload data is byte-based.

One byte is 8 bits.

```cpp
std::uint8_t
```

means exactly unsigned 8-bit integer.

This is useful because CAN data bytes should be exactly 8 bits.

Simple explanation:

```text
I use uint8_t for CAN data because each CAN payload byte is exactly 8 bits.
```

---

## Why simulate CAN logs before hardware?

Simulated CAN logs let me test the decoder logic before I have real hardware.

Benefits:

```text
I can test known good frames.
I can test bad frames.
I can test unknown IDs.
I can test invalid DLC.
I can test decoding logic early.
I can build the desktop-side analyzer before connecting STM32 hardware.
```

Simple explanation:

```text
Simulating CAN logs lets me develop and test the C++ decoder before physical CAN hardware is available.
```

---

# Day 2 — Validation Functions and Linked List Merge

## Main goals

```text
Write is_known_id().
Write has_valid_dlc().
Print FAULT: Unknown CAN ID.
Print FAULT: Invalid DLC.
Solve LeetCode 21: Merge Two Sorted Lists.
Add GPIO notes.
```

---

## CAN validation logic

Today I added helper functions to validate CAN frames before decoding them.

Known CAN IDs for now:

```text
0x100
0x101
0x102
0x200
```

Valid DLC for the current telemetry protocol:

```text
8
```

---

## `is_known_id`

Function:

```cpp
bool is_known_id(std::uint32_t id)
```

This function checks whether a CAN ID belongs to the expected telemetry protocol.

Example:

```cpp
bool is_known_id(std::uint32_t id) {
    return id == 0x100 ||
           id == 0x101 ||
           id == 0x102 ||
           id == 0x200;
}
```

If the ID is not known, the program prints:

```text
FAULT: Unknown CAN ID
```

---

## `has_valid_dlc`

Function:

```cpp
bool has_valid_dlc(const CanFrame& frame)
```

This function checks whether the frame has the expected number of data bytes.

For now, the simulated telemetry messages expect:

```text
dlc == 8
```

Example:

```cpp
bool has_valid_dlc(const CanFrame& frame) {
    return frame.dlc == 8;
}
```

If the DLC is wrong, the program prints:

```text
FAULT: Invalid DLC
```

---

## Why check frame ID before decoding?

The CAN ID tells the decoder what type of message it is reading.

Example protocol:

```text
0x100 = analog input data
0x101 = battery and temperature data
0x102 = status flags
0x200 = vehicle telemetry
```

If the ID is unknown, the decoder does not know how to interpret the payload bytes.

Simple explanation:

```text
I check the CAN ID before decoding because the ID tells me what the payload means.
```

---

## Why check DLC before reading data bytes?

DLC tells how many data bytes are valid in the frame.

If the decoder expects 8 bytes but the frame only has 4, then reading bytes 4 through 7 would be wrong.

Simple explanation:

```text
I check DLC before decoding so I do not read bytes that are not valid for that frame.
```

---

## What could happen if DLC is wrong?

Possible problems:

```text
wrong decoded values
reading invalid bytes
false fault detection
misinterpreting one message as another
unsafe assumptions in embedded code
```

Example:

```text
If AIN1, AIN2, and AIN3 need 6 bytes total, but DLC is only 4, then AIN3 cannot be decoded safely.
```

---

## Processing a frame

Example:

```cpp
void process_frame(const CanFrame& frame) {
    print_frame(frame);

    bool has_fault = false;

    if (!is_known_id(frame.id)) {
        std::cout << "FAULT: Unknown CAN ID" << std::endl;
        has_fault = true;
    }

    if (!has_valid_dlc(frame)) {
        std::cout << "FAULT: Invalid DLC" << std::endl;
        has_fault = true;
    }

    if (!has_fault) {
        std::cout << "Frame status: OK" << std::endl;
    }
}
```

This function:

```text
prints the frame
checks the ID
checks the DLC
prints faults if needed
prints OK if no fault exists
```

---

## LeetCode 21 — Merge Two Sorted Lists

Pattern:

```text
Linked list pointer movement
Dummy node
```

Main idea:

```text
Use a dummy node as a fake starting point.
Use a tail pointer to build the merged list.
Compare list1 and list2.
Attach the smaller node to tail.
Move the pointer that was used.
Move tail forward.
Attach the remaining list at the end.
Return dummy.next.
```

---

## Why use a dummy node?

A dummy node gives the merged list a fake starting point.

This avoids special-case logic for the first node.

Example:

```cpp
ListNode dummy;
ListNode* tail = &dummy;
```

Meaning:

```text
dummy is a fake node before the real merged list.
tail points to the last node in the merged list.
```

At the end:

```cpp
return dummy.next;
```

because the real merged list starts after the dummy node.

---

## Merge Two Sorted Lists solution idea

```cpp
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode dummy;
    ListNode* tail = &dummy;

    while (list1 != nullptr && list2 != nullptr) {
        if (list1->val < list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }

        tail = tail->next;
    }

    if (list1 != nullptr) {
        tail->next = list1;
    } else {
        tail->next = list2;
    }

    return dummy.next;
}
```

Time complexity:

```text
O(n + m)
```

Space complexity:

```text
O(1)
```

---

## Day 2 main interview idea

```text
Never decode a CAN payload blindly. Check the ID to know what the message means, and check the DLC to make sure enough bytes are available.
```
---

# Day 3 — Stack vs Heap and ADC Concept

## Main goals

```text
Study stack vs heap.
Understand why new/delete can be risky in embedded systems.
Solve Linked List Cycle.
Solve Middle of the Linked List.
Create pack_u16().
Decode CAN ID 0x100.
Add ADC notes.
```

---

## Stack memory

Stack memory is used for local variables.

Example:

```cpp
int stack_value = 5;
```

The variable is automatically created when the function starts and automatically destroyed when the function ends.

Simple explanation:

```text
Stack memory is automatic, fast, and managed for me, but it is limited in size.
```

---

## Heap memory

Heap memory is used for dynamic allocation.

Example:

```cpp
int* heap_value = new int(10);
delete heap_value;
```

The programmer is responsible for freeing heap memory.

Simple explanation:

```text
Heap memory is manually managed when using new/delete.
```

---

## Stack vs heap

```text
Stack:
- automatic memory
- used for local variables
- fast
- cleaned up when the function ends
- limited size

Heap:
- dynamic memory
- used with new/delete
- must be manually managed
- can live beyond one function call
- easier to misuse
```

---

## Why `new/delete` can be risky in embedded systems

`new/delete` can be risky in embedded firmware because RAM is limited and the timing of allocation can be unpredictable.

Possible problems:

```text
memory leaks
heap fragmentation
unpredictable allocation time
allocation failure
harder debugging
crashes from dangling pointers
```

For embedded systems, it is often safer to use:

```text
fixed-size arrays
static allocation
stack allocation
fixed-size buffers
object pools
```

Simple explanation:

```text
Embedded systems often avoid dynamic allocation because memory is limited and predictable behavior matters.
```

---

## `pack_u16`

Function:

```cpp
std::uint16_t pack_u16(std::uint8_t low, std::uint8_t high) {
    return static_cast<std::uint16_t>(low) |
           (static_cast<std::uint16_t>(high) << 8);
}
```

This combines two 8-bit bytes into one 16-bit value.

---

## Little-endian

Little-endian means the low byte comes first.

Example:

```text
data[0] = low byte
data[1] = high byte
```

So this:

```cpp
pack_u16(data[0], data[1])
```

means:

```text
combine data[0] as the low byte and data[1] as the high byte
```

---

## What does low byte mean?

The low byte contains the lower 8 bits of a larger value.

Example:

```text
16-bit value: 0x1234
```

Low byte:

```text
0x34
```

High byte:

```text
0x12
```

---

## What does high byte mean?

The high byte contains the upper 8 bits of a larger value.

Example:

```text
16-bit value: 0x1234
```

High byte:

```text
0x12
```

Low byte:

```text
0x34
```

---

## Why is `data[0]` combined with `data[1] << 8`?

Because `data[0]` is the low byte and `data[1]` is the high byte.

The high byte must be shifted left by 8 bits so it occupies the upper half of the 16-bit value.

Example:

```text
data[0] = 0x34
data[1] = 0x12

data[1] << 8 = 0x1200
data[0]      = 0x0034

combined     = 0x1234
```

The `|` operator combines the two parts.

Simple explanation:

```text
Shift the high byte left by 8 bits, then OR it with the low byte to rebuild the original 16-bit value.
```

---

## Decode CAN ID `0x100`

Protocol for `0x100`:

```text
Byte 0-1: AIN1
Byte 2-3: AIN2
Byte 4-5: AIN3
Byte 6: status
Byte 7: counter
```

Example:

```cpp
std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);

std::uint8_t status = frame.data[6];
std::uint8_t counter = frame.data[7];
```

This is the first real decoder step for the CAN telemetry project.

---

## Example decoding

Example bytes:

```text
frame.data[0] = 0x00
frame.data[1] = 0x08
```

Since the frame is little-endian:

```text
low byte  = 0x00
high byte = 0x08
```

So:

```text
0x08 << 8 = 0x0800
0x00      = 0x0000

AIN1      = 0x0800 = 2048 decimal
```

---

## ADC concept

ADC stands for Analog-to-Digital Converter.

An ADC converts an analog voltage into a digital number.

For a 12-bit ADC:

```text
minimum raw value = 0
maximum raw value = 4095
```

If the reference voltage is 3.3 V:

```text
0    = 0 V
4095 = about 3.3 V
2048 = about 1.65 V
```

---

## Raw ADC to voltage formula

```text
voltage = (adc_raw * 3.3) / 4095.0
```

Example:

```text
adc_raw = 2048

voltage = (2048 * 3.3) / 4095.0
voltage ≈ 1.65 V
```

---

## Why ADC matters for vehicle telemetry

Many vehicle sensors produce analog voltages.

Examples:

```text
throttle position sensor
brake pressure sensor
temperature sensor
battery voltage divider
suspension position sensor
```

The STM32 reads those voltages as raw ADC values.

Then the firmware can:

```text
scale the value
filter the value
pack the value into CAN bytes
transmit the CAN frame
```

The C++ decoder can then decode those bytes back into useful sensor values.

---

## LeetCode 141 — Linked List Cycle

Pattern:

```text
slow and fast pointers
```

Main idea:

```text
Move slow by 1 step.
Move fast by 2 steps.
If slow and fast meet, there is a cycle.
If fast reaches nullptr, there is no cycle.
```

Code idea:

```cpp
bool hasCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return true;
        }
    }

    return false;
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

## LeetCode 876 — Middle of the Linked List

Pattern:

```text
slow and fast pointers
```

Main idea:

```text
Move slow by 1 step.
Move fast by 2 steps.
When fast reaches the end, slow is at the middle.
```

Code idea:

```cpp
ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
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

## Day 3 main interview idea

```text
CAN data is byte-based. To decode a 16-bit sensor value from an 8-byte CAN payload, I combine two bytes using little-endian parsing: low byte first, high byte shifted left by 8, then bitwise OR.
```

---

## Things to keep practicing

```text
slow and fast pointer movement
why fast catches slow in a cycle
stack vs heap explanation
why new/delete is risky in embedded systems
little-endian byte packing
raw ADC to voltage scaling
```
---

# Day 4 — References and UART Concept

## Main goals

```text
Practice pass by value.
Practice pass by reference.
Practice pass by const reference.
Solve Remove Linked List Elements.
Create decode_analog_inputs().
Add UART notes.
```

---

## Pass by value

Function:

```cpp
void print_by_value(std::vector<int> readings)
```

This copies the vector into the function.

If the function modifies `readings`, the original vector outside the function does not change.

Use pass by value when:

```text
I need a copy
the object is small
I intentionally want to modify a local copy
```

For large vectors, pass by value can be inefficient because it copies the whole vector.

---

## Pass by reference

Function:

```cpp
void print_by_reference(std::vector<int>& readings)
```

This does not copy the vector.

The function works with the original vector.

If the function modifies `readings`, the original vector changes.

Use pass by reference when:

```text
the function needs to modify the original object
```

---

## Pass by const reference

Function:

```cpp
void print_by_const_reference(const std::vector<int>& readings)
```

This does not copy the vector.

The function can read the original vector, but it cannot modify it.

Use pass by const reference when:

```text
the function only needs to read a large object
I want efficiency
I want to prevent accidental modification
```

This is usually the best choice for print/read-only functions.

---

## Comparing the three

```text
Pass by value:
- makes a copy
- original does not change
- can be slower for large objects

Pass by reference:
- no copy
- original can change
- use when modification is intended

Pass by const reference:
- no copy
- original cannot change
- best for reading large objects safely
```

---

## Why use `const CanFrame&`?

Function:

```cpp
void decode_analog_inputs(const CanFrame& frame)
```

This passes the CAN frame without copying it.

The `const` prevents the decoder from accidentally changing the frame.

Simple explanation:

```text
I use const CanFrame& because the decoder only needs to read the frame, not modify it.
```

This is useful because CAN frames are structured data, and the decoder should only interpret the frame, not change the original data.

---

## `decode_analog_inputs`

Function purpose:

```text
Check DLC.
Decode AIN1.
Decode AIN2.
Decode AIN3.
Print status byte.
Print counter.
```

Protocol for CAN ID `0x100`:

```text
Byte 0-1: AIN1
Byte 2-3: AIN2
Byte 4-5: AIN3
Byte 6: status
Byte 7: counter
```

Code idea:

```cpp
void decode_analog_inputs(const CanFrame& frame) {
    if (frame.dlc < 8) {
        std::cout << "Cannot decode analog inputs: invalid DLC" << std::endl;
        return;
    }

    std::uint16_t ain1 = pack_u16(frame.data[0], frame.data[1]);
    std::uint16_t ain2 = pack_u16(frame.data[2], frame.data[3]);
    std::uint16_t ain3 = pack_u16(frame.data[4], frame.data[5]);

    std::uint8_t status = frame.data[6];
    std::uint8_t counter = frame.data[7];

    std::cout << "AIN1: " << ain1 << std::endl;
    std::cout << "AIN2: " << ain2 << std::endl;
    std::cout << "AIN3: " << ain3 << std::endl;
    std::cout << "Status: " << static_cast<int>(status) << std::endl;
    std::cout << "Counter: " << static_cast<int>(counter) << std::endl;
}
```

---

## Why check DLC inside the decoder?

The decoder should not assume the frame has enough bytes.

For `0x100`, the decoder needs 8 bytes:

```text
AIN1 needs bytes 0 and 1.
AIN2 needs bytes 2 and 3.
AIN3 needs bytes 4 and 5.
Status needs byte 6.
Counter needs byte 7.
```

If the DLC is too small, the function should stop before decoding.

Simple explanation:

```text
I check DLC before decoding so I do not read bytes that are not valid or not present.
```

---

## LeetCode 203 — Remove Linked List Elements

Pattern:

```text
linked list pointer relinking
dummy node
```

Main idea:

```text
Create a dummy node before the head.
Use curr to look at curr->next.
If curr->next has the value that should be removed, skip it.
Otherwise move curr forward.
Return dummy.next.
```

Important line:

```cpp
curr->next = curr->next->next;
```

This removes a node by skipping over it.

Example:

```text
Before:
1 -> 2 -> 6 -> 3

Remove 6:

After:
1 -> 2 -> 3
```

The node is removed because the previous node no longer points to it.

Time complexity:

```text
O(n)
```

Space complexity:

```text
O(1)
```

---

## UART

UART stands for Universal Asynchronous Receiver/Transmitter.

UART sends and receives serial data using TX and RX pins.

```text
TX = transmit
RX = receive
```

Common baud rate:

```text
115200
```

UART is commonly used for debug printing in embedded systems.

---

## TX

TX means transmit.

The TX pin sends data out of the microcontroller.

Example:

```text
STM32 TX ---> USB-UART RX
```

If the STM32 is printing debug messages, those messages leave through TX.

---

## RX

RX means receive.

The RX pin receives data into the microcontroller.

Example:

```text
USB-UART TX ---> STM32 RX
```

If the computer sends a command to the STM32, the STM32 receives it through RX.

---

## Baud rate

Baud rate is the communication speed.

Common baud rates:

```text
9600
38400
57600
115200
```

Both devices must use the same baud rate.

Example:

```text
STM32 UART = 115200 baud
Serial terminal = 115200 baud
```

---

## Serial terminal

A serial terminal is a program on the computer that displays UART messages.

Examples:

```text
PuTTY
Tera Term
RealTerm
Arduino Serial Monitor
VS Code serial monitor extensions
```

The terminal must use the correct COM port and correct baud rate.

---

## Blocking transmit

Blocking transmit means the program waits until the UART transmission finishes.

Simple explanation:

```text
Send message.
Wait until message is done.
Continue program.
```

This is simple and useful for beginner debug printing.

Downside:

```text
It can slow down the program if too much data is printed.
```

---

## Non-blocking transmit

Non-blocking transmit means the program starts the transmission and continues running while the UART hardware, interrupt, or DMA handles the transfer.

Examples:

```text
interrupt-based UART transmit
DMA-based UART transmit
```

This is better for real-time systems, but it is more complex.

---

## Why UART is useful for debugging

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

This helps debug firmware when there is no normal desktop console.

---

## What happens if baud rate is wrong?

If the baud rate is wrong, the serial terminal may show garbage characters or no readable output.

Both the STM32 UART configuration and the computer serial terminal must use the same baud rate.

Example:

```text
STM32 configured for 115200
Terminal configured for 9600
Result: unreadable output
```

Simple explanation:

```text
Wrong baud rate means the computer and microcontroller are not speaking at the same speed.
```

---

## What would I print from my telemetry system?

I would print:

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

This would help confirm that the telemetry system is reading, packing, transmitting, and decoding data correctly.

---

## Day 4 main interview idea

```text
For large objects like vectors and CAN frame structs, use const reference when the function only needs to read the data. In firmware, UART is one of the simplest ways to print debug information from a microcontroller.
```