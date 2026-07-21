# CAN Debug Checklist

This document lists common CAN communication problems and the checks used to debug them.

---

## Expected Test Frame

The first STM32 CAN transmit test sends:

```text
ID: 0x100
DLC: 8
Data: 00 08 10 00 FF 0A 07 01