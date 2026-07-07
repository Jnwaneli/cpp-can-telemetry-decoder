# STM32 GPIO, ADC, and UART Practice

## Purpose

This folder documents my STM32 peripheral practice for GPIO, ADC, and UART.

The goal is to build basic firmware fluency alongside my C++ CAN telemetry decoder project. The C++ project lets me simulate and decode CAN frames on my computer, while the STM32 practice helps me understand how real sensor data would eventually be read from hardware and transmitted through firmware.

---

## Planned Hardware

Current board:

```text
STM32 Nucleo board
```

Supporting parts:

```text
USB cable
Breadboard
Jumper wires
LED
220-330 ohm resistor
Pushbutton
Potentiometer
```

Optional later parts:

```text
I2C sensor
SPI module
CAN transceiver module
USB-UART adapter if needed
```

---

# Peripheral Goals

## 1. GPIO

GPIO stands for General Purpose Input/Output.

GPIO pins allow a microcontroller to control digital outputs or read digital inputs.

### GPIO Output Goal

Blink an LED using a GPIO output pin.

Example use cases:

```text
status LED
fault indicator
power indicator
debug signal
```

### GPIO Input Goal

Read a pushbutton using a GPIO input pin.

Example use cases:

```text
mode switch
reset button
driver input
fault clear button
```

### GPIO Concepts to Understand

```text
input mode
output mode
pull-up resistor
pull-down resistor
floating input
button bounce
current-limiting resistor
```

---

## 2. ADC

ADC stands for Analog-to-Digital Converter.

An ADC converts an analog voltage into a digital number.

For example, a 12-bit ADC usually gives values from:

```text
0 to 4095
```

If the reference voltage is 3.3 V:

```text
0     = 0 V
4095  = about 3.3 V
2048  = about 1.65 V
```

### ADC Goal

Use a potentiometer to create a changing analog voltage and read it with an STM32 ADC pin.

### ADC Concepts to Understand

```text
ADC resolution
12-bit range
reference voltage
raw ADC value
voltage scaling
sampling time
noise
averaging/filtering
```

### Raw-to-Voltage Formula

```text
voltage = (raw_adc / 4095.0) * 3.3
```

Example:

```text
raw_adc = 2048
voltage = about 1.65 V
```

### How This Connects to Vehicle Telemetry

In a vehicle telemetry system, ADC inputs can be used to read sensors such as:

```text
throttle position sensor
brake pressure sensor
temperature sensor
suspension position sensor
battery voltage divider
```

The raw ADC values can then be packed into CAN frames and decoded by the C++ telemetry analyzer.

---

## 3. UART

UART stands for Universal Asynchronous Receiver/Transmitter.

UART is used for serial communication between a microcontroller and another device.

### UART Goal

Print ADC readings or debug messages from the STM32 to a serial terminal.

Example output:

```text
ADC Raw: 2048
Voltage: 1.65 V
Button: Pressed
```

### UART Concepts to Understand

```text
TX pin
RX pin
baud rate
serial terminal
blocking transmit
non-blocking transmit
debug printing
```

Common baud rate:

```text
115200
```

### How This Connects to Debugging

UART is useful because it lets firmware print internal values while the program is running.

Example debug messages:

```text
System started
ADC reading updated
CAN frame prepared
Fault detected
Button pressed
```

---

# STM32CubeIDE Setup Notes

Planned setup steps:

```text
1. Open STM32CubeIDE.
2. Create a new STM32 project.
3. Select the STM32 Nucleo board.
4. Configure one GPIO output pin for LED blinking.
5. Configure one GPIO input pin for pushbutton reading.
6. Configure one ADC channel for potentiometer input.
7. Configure UART for serial output.
8. Generate code.
9. Build the project.
10. Flash the board.
11. Debug using ST-LINK.
```

For now, this is a setup plan. The goal is not to finish all firmware immediately. The goal is to understand what each peripheral does and how it connects to telemetry.

---

# GPIO Notes

## What is GPIO?

GPIO stands for General Purpose Input/Output.

A GPIO pin is a microcontroller pin that can be configured by firmware to either read a digital signal or drive a digital output.

GPIO is one of the most basic peripherals in embedded systems.

---

## GPIO Output

A GPIO output pin is used when the microcontroller controls something.

Examples:

```text
LED
relay control signal
enable pin
chip select pin
fault indicator
```

For an LED, the microcontroller sets the pin either high or low.

```text
HIGH = usually 3.3 V
LOW = 0 V
```

The LED also needs a current-limiting resistor so too much current does not flow through the LED or microcontroller pin.

---

## GPIO Input

A GPIO input pin is used when the microcontroller reads a digital signal.

Examples:

```text
pushbutton
limit switch
digital sensor output
fault signal
mode select switch
```

The input reads either:

```text
HIGH
LOW
```

---

## Pull-up vs Pull-down

A pull-up resistor makes the input default to HIGH when nothing is pressing or driving the pin.

A pull-down resistor makes the input default to LOW when nothing is pressing or driving the pin.

This matters because an input pin should not be left floating.

A floating input can randomly change between HIGH and LOW because it is not connected to a defined voltage.

---

## Button Bounce

A mechanical button does not always make a clean transition from not pressed to pressed.

When pressed, the contact can bounce quickly between HIGH and LOW before settling.

This is called button bounce.

Firmware can handle this with debounce logic, such as:

```text
wait a few milliseconds after a change
confirm the input is still in the same state
then accept the button press
```

---

## LED Current-Limiting Resistor

An LED needs a resistor in series to limit current.

Without a resistor, too much current can flow and damage the LED or the microcontroller pin.

Common resistor values:

```text
220 ohm
330 ohm
1 kilo-ohm
```

For basic STM32 LED practice, 220-330 ohms is usually a reasonable starting range.

---

# Debugging Notes

Tools to use:

```text
ST-LINK debugger
breakpoints
watch variables
step over
step into
register view
UART serial output
```

Important debugging questions:

```text
Is the code running?
Is the correct pin configured?
Is the GPIO pin in input or output mode?
Is the ADC channel enabled?
Is the UART baud rate correct?
Is the board powered correctly?
Is the ground connected?
```

---

# Common GPIO Bugs

```text
Wrong pin selected
Pin not configured as output
Pin not configured as input
Missing pull-up or pull-down resistor
Button input floating
LED connected backwards
Missing current-limiting resistor
```

---

# Common ADC Bugs

```text
Wrong ADC channel selected
Pin not configured in analog mode
No common ground
Input voltage outside valid range
Noisy signal
Forgetting to start ADC conversion
Using integer math incorrectly during voltage conversion
```

---

# Common UART Bugs

```text
Wrong baud rate
Wrong COM port
TX/RX pins swapped
UART not enabled
Terminal not opened
Using blocking transmit too often
Printing too much data too fast
```

---

# Connection to C++ CAN Telemetry Decoder

The desktop C++ project simulates CAN frames like this:

```text
CAN ID
DLC
8 data bytes
```

The STM32 side would eventually produce real sensor data.

Example flow:

```text
Sensor voltage
ADC raw value
Pack ADC value into bytes
Create CAN frame
Transmit CAN frame
C++ decoder receives/logs frame
C++ decoder checks ID and DLC
C++ decoder decodes sensor values
C++ fault analyzer checks for problems
```

This means the STM32 practice and the C++ project support each other.

The STM32 practice teaches how sensor data is created.

The C++ project teaches how sensor data is decoded and analyzed.

---

# Interview Explanation

A short explanation I should be able to say out loud:

```text
I created this STM32 peripheral practice folder to document basic GPIO, ADC, and UART concepts. GPIO helps with digital inputs and outputs, ADC is used to read analog sensor values, and UART is useful for debug output. These peripherals connect directly to vehicle telemetry because a microcontroller reads sensor data, converts it into digital values, and can then transmit that data over CAN or print it over UART for debugging.
```

---

# Next Improvements

```text
Add GPIO LED blink notes
Add pushbutton input notes
Add ADC raw-to-voltage test notes
Add UART serial print notes
Add screenshots from STM32CubeIDE
Add wiring diagram
Add common debugging checklist
```