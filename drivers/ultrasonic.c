#include "ultrasonic.h"
#include <stdio.h>

void ultrasonic_init(ultrasonic_t *ultrasonic, uint8_t trigger_pin, uint8_t echo_pin){
    ultrasonic->trigger_pin = trigger_pin;
    ultrasonic->echo_pin = echo_pin;

    gpio_init(trigger_pin);
    gpio_set_dir(trigger_pin, GPIO_OUT);
    gpio_put(trigger_pin, 0);

    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);
}

float ultrasonic_get_distance(ultrasonic_t *ultrasonic){
    gpio_put(ultrasonic->trigger_pin, 0);
    sleep_us(2);
    gpio_put(ultrasonic->trigger_pin, 1);
    sleep_us(5);
    gpio_put(ultrasonic->trigger_pin, 0);

    uint32_t start_time = time_us_32();
    while (gpio_get(ultrasonic->echo_pin) == 0);
    start_time = time_us_32();
    while (gpio_get(ultrasonic->echo_pin) == 1);
    uint32_t duration = time_us_32() - start_time;

    float distance = (duration / 2.0f) * 0.0343f;

    return distance;
}
