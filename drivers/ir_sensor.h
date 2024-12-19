#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef PICO_STDLIB_H
#define PICO_STDLIB_H
#include "pico/stdlib.h"
#endif

#define THRESHOLD 2400

#include "stdio.h"

void ir_sensor_init(uint8_t ir_pin);
uint32_t ir_sensor_read(uint8_t ir_pin);
float ir_sensor_read_voltage(uint8_t ir_pin);
bool ir_object_detected(uint8_t ir_pin);

#endif //IR_SENSOR_H
