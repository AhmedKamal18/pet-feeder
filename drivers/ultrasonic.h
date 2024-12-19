#ifndef BLINK_ULTRASONIC_H
#define BLINK_ULTRASONIC_H

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif


#ifndef PICO_STDLIB_H
#define PICO_STDLIB_H
#include "pico/stdlib.h"
#endif

typedef struct
{
    uint8_t echo_pin;
    uint8_t trigger_pin;

}ultrasonic_t;

void ultrasonic_init(ultrasonic_t *ultrasonic, uint8_t trigger_pin, uint8_t echo_pin);
float ultrasonic_get_distance(ultrasonic_t *ultrasonic);

#endif //BLINK_ULTRASONIC_H
