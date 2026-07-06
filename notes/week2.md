# STM32 GPIO, ADC, UART Practice

## Goal

Use this folder to document basic STM32 peripheral practice for GPIO, ADC, and UART.

## Planned board

Board not selected yet. Possible options:
- STM32 Nucleo board
- Low-cost STM32 development board

## GPIO goal

Blink an LED using a GPIO output pin.

Read a pushbutton using a GPIO input pin.

## Why this matters

GPIO is the basic way firmware reads digital inputs and controls digital outputs. In vehicle telemetry systems, GPIO can be used for status LEDs, switches, fault indicators, and digital sensor signals.

Why use const CanFrame&?
What is DLC?
Why use uint8_t for CAN data?
Why simulate CAN logs before hardware?

const CanFrame& avoids copying the frame and prevents modification.

DLC means Data Length Code. It tells how many data bytes are valid in the CAN frame.

uint8_t is useful because CAN payload data is byte-based, and one byte is exactly 8 bits.

Simulated CAN logs let me test decoding and fault logic before having physical CAN hardware.