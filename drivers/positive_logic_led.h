#ifndef LED_DRIVER
#define LED_DRIVER

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef PICO_STDLIB_H
#define PICO_STDLIB_H
#include "pico/stdlib.h"
#endif

void POSITIVE_LOGIC_LED_Init(uint8_t led_id);
void POSITIVE_LOGIC_LED_On(uint8_t led_id);
void POSITIVE_LOGIC_LED_Off(uint8_t led_id);

#endif