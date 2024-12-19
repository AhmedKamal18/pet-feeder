#include "positive_logic_led.h"

void POSITIVE_LOGIC_LED_Init(uint8_t led_id){
    gpio_init(led_id);
    gpio_set_dir(led_id, GPIO_OUT);
}

void POSITIVE_LOGIC_LED_On(uint8_t led_id){
    gpio_put(led_id, 1);
}

void POSITIVE_LOGIC_LED_Off(uint8_t led_id){
    gpio_put(led_id, 0);
}