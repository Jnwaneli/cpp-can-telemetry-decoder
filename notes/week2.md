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