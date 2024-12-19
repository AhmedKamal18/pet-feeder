#include "buzzer.h"

void buzzer_init(uint8_t buzzer_id){
    gpio_init(buzzer_id);
    gpio_set_dir(buzzer_id, GPIO_OUT);
}

void buzzer_turn_on(uint8_t buzzer_id){
    gpio_put(buzzer_id, 1);
}

void buzzer_turn_off(uint8_t buzzer_id){
    gpio_put(buzzer_id, 0);
}