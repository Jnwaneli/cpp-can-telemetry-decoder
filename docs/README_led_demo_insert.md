## Suggested README insert for the updated LED wiring schematic

Add this near the hardware wiring section:

```markdown
### Updated CAN + LED Matrix Hardware Wiring

The STM32 FreeRTOS demo now includes a MAX7219 4-module LED matrix. The matrix is driven from GPIO pins PB6, PB7, and PB8 while the STM32 continues transmitting live CAN telemetry through the SN65HVD230 transceiver and Waveshare USB-CAN adapter.

<img src="./media/freertos_can_led_hardware_wiring.svg" alt="STM32 FreeRTOS CAN and MAX7219 LED fault demo wiring schematic" width="900">

**Figure X. Updated STM32 FreeRTOS CAN and MAX7219 LED fault demo wiring.** The NUCLEO-G431RB sends CAN frames through the SN65HVD230 transceiver to the Waveshare USB-CAN adapter while also driving a 4-module MAX7219 LED matrix over bit-banged GPIO. The LED matrix displays the same firmware demo mode that controls the injected CAN fault payloads.

For the full wiring table, see [`docs/week10_led_fault_demo_wiring.md`](./docs/week10_led_fault_demo_wiring.md).
```
