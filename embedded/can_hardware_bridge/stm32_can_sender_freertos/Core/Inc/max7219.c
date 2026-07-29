#include "max7219.h"

#include "main.h"

#define MAX7219_REG_NOOP          0x00
#define MAX7219_REG_DIGIT0        0x01
#define MAX7219_REG_DECODE_MODE   0x09
#define MAX7219_REG_INTENSITY     0x0A
#define MAX7219_REG_SCAN_LIMIT    0x0B
#define MAX7219_REG_SHUTDOWN      0x0C
#define MAX7219_REG_DISPLAY_TEST  0x0F
#define MAX7219_DEVICE_COUNT 4

static void MAX7219_DelaySmall(void)
{
    for (volatile int i = 0; i < 40; i++) {
        __NOP();
    }
}

static void MAX7219_SetDin(uint8_t value)
{
    HAL_GPIO_WritePin(MAX7219_DIN_GPIO_Port,
                      MAX7219_DIN_Pin,
                      value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void MAX7219_SetClk(uint8_t value)
{
    HAL_GPIO_WritePin(MAX7219_CLK_GPIO_Port,
                      MAX7219_CLK_Pin,
                      value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void MAX7219_SetCs(uint8_t value)
{
    HAL_GPIO_WritePin(MAX7219_CS_GPIO_Port,
                      MAX7219_CS_Pin,
                      value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void MAX7219_WriteByte(uint8_t data)
{
    for (int bit = 7; bit >= 0; bit--) {
        MAX7219_SetClk(0);

        if ((data & (1U << bit)) != 0U) {
            MAX7219_SetDin(1);
        } else {
            MAX7219_SetDin(0);
        }

        MAX7219_DelaySmall();

        MAX7219_SetClk(1);
        MAX7219_DelaySmall();
    }

    MAX7219_SetClk(0);
}

static void MAX7219_WriteRegister(uint8_t reg, uint8_t data)
{
    MAX7219_SetCs(0);
    MAX7219_DelaySmall();

    for (uint8_t device = 0; device < MAX7219_DEVICE_COUNT; device++) {
        MAX7219_WriteByte(reg);
        MAX7219_WriteByte(data);
    }

    MAX7219_DelaySmall();
    MAX7219_SetCs(1);
    MAX7219_DelaySmall();
}
void MAX7219_Clear(void)
{
    for (uint8_t row = 0; row < 8; row++) {
        MAX7219_WriteRegister((uint8_t)(MAX7219_REG_DIGIT0 + row), 0x00);
    }
}

void MAX7219_DisplayPattern(const uint8_t pattern[8])
{
    for (uint8_t row = 0; row < 8; row++) {
        MAX7219_WriteRegister((uint8_t)(MAX7219_REG_DIGIT0 + row), pattern[row]);
    }
}

void MAX7219_DisplayCheck(void)
{
    const uint8_t check_pattern[8] = {
        0x00,
        0x01,
        0x02,
        0x04,
        0x88,
        0x50,
        0x20,
        0x00
    };

    MAX7219_DisplayPattern(check_pattern);
}

void MAX7219_Init(void)
{
    MAX7219_SetCs(1);
    MAX7219_SetClk(0);
    MAX7219_SetDin(0);

    MAX7219_WriteRegister(MAX7219_REG_DISPLAY_TEST, 0x00);
    MAX7219_WriteRegister(MAX7219_REG_SHUTDOWN, 0x01);
    MAX7219_WriteRegister(MAX7219_REG_DECODE_MODE, 0x00);
    MAX7219_WriteRegister(MAX7219_REG_SCAN_LIMIT, 0x07);
    MAX7219_WriteRegister(MAX7219_REG_INTENSITY, 0x01);

    MAX7219_WriteRegister(MAX7219_REG_DISPLAY_TEST, 0x00);

    MAX7219_Clear();
}
