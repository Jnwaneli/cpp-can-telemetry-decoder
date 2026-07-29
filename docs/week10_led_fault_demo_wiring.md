# STM32 FreeRTOS LED Fault Demo Wiring

This note documents the added MAX7219 LED matrix wiring used for the button-gated FreeRTOS fault demo.

## Hardware used

- NUCLEO-G431RB
- SN65HVD230 CAN transceiver
- Waveshare USB-CAN adapter
- 4-module MAX7219 LED matrix
- Common ground between STM32 board, CAN transceiver, USB-CAN adapter, and LED matrix

## CAN wiring

| NUCLEO-G431RB | SN65HVD230 | Notes |
|---|---|---|
| 3.3V | VCC | SN65HVD230 logic supply |
| GND | GND | Common ground |
| PA12 / FDCAN1_TX | TXD | STM32 CAN transmit into transceiver |
| PA11 / FDCAN1_RX | RXD | STM32 CAN receive from transceiver |
| - | CANH | Connects to Waveshare CANH |
| - | CANL | Connects to Waveshare CANL |

## MAX7219 LED matrix wiring

| NUCLEO-G431RB | MAX7219 module | Notes |
|---|---|---|
| 5V | VCC | LED matrix module supply |
| GND | GND | Common ground with STM32/CAN setup |
| PB6 / D10 | DIN | Bit-banged serial data |
| PB7 | CLK | Bit-banged clock |
| PB8 / D15 | CS / LOAD | Chip-select/load signal |

## Demo behavior

The LED matrix is controlled by a FreeRTOS `DisplayTask`. At startup, the display runs a scanner sweep animation while the firmware waits for the NUCLEO user button. After the button is pressed, `FaultInjectTask` begins cycling through demo fault modes. `CanTxTask` uses the same mode to inject matching CAN payload values before sending frames.

| LED state | Firmware mode | CAN payload effect | Decoder result |
|---|---|---|---|
| Check mark | `DEMO_MODE_NORMAL` | Normal battery, temperature, and status values | Clean telemetry |
| Exclamation mark | `DEMO_MODE_HIGH_TEMP` | `temperature_deciC = 950` | High-temperature fault |
| X symbol | `DEMO_MODE_LOW_VOLTAGE` | `battery_mV = 9500` | Low-voltage fault |
| X symbol | `DEMO_MODE_SENSOR_INVALID` | `status = 0x06` | Sensor-invalid fault |

## Updated wiring diagram

```mermaid
flowchart LR
    NUCLEO[NUCLEO-G431RB\nSTM32G431RB] -->|PA12 FDCAN1_TX| CAN_TXD[SN65HVD230 TXD]
    CAN_RXD[SN65HVD230 RXD] -->|PA11 FDCAN1_RX| NUCLEO
    NUCLEO -->|3.3V| CAN_VCC[SN65HVD230 VCC]
    NUCLEO -->|GND| CAN_GND[SN65HVD230 GND]

    CAN_TRANS[SN65HVD230 CAN Transceiver] -->|CANH| WAVESHARE[Waveshare USB-CAN]
    CAN_TRANS -->|CANL| WAVESHARE
    CAN_GND -->|Common GND| WAVESHARE

    NUCLEO -->|5V| MAXVCC[MAX7219 VCC]
    NUCLEO -->|GND| MAXGND[MAX7219 GND]
    NUCLEO -->|PB6 / D10| MAXDIN[MAX7219 DIN]
    NUCLEO -->|PB7| MAXCLK[MAX7219 CLK]
    NUCLEO -->|PB8 / D15| MAXCS[MAX7219 CS/LOAD]

    MAX7219[MAX7219 4-Module LED Matrix]
    MAXVCC --> MAX7219
    MAXGND --> MAX7219
    MAXDIN --> MAX7219
    MAXCLK --> MAX7219
    MAXCS --> MAX7219
```
