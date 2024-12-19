#ifndef BUZZER_DRIVER
#define BUZZER_DRIVER

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef PICO_STDLIB_H
#define PICO_STDLIB_H
#include "pico/stdlib.h"
#endif

void buzzer_init(uint8_t buzzer_id);
void buzzer_turn_on(uint8_t buzzer_id);
void buzzer_turn_off(uint8_t buzzer_id);

#endif //BUZZER_DRIVER
