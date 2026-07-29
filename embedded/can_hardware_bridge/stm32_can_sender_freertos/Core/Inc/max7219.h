#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void MAX7219_Init(void);
void MAX7219_Clear(void);
void MAX7219_DisplayPattern(const uint8_t pattern[8]);
void MAX7219_DisplayCheck(void);

#ifdef __cplusplus
}
#endif

#endif