#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "drivers/buzzer.h"

#define BUZZER_PIN 3
#define IR_PIN 26

// Function declarations
extern bool can_activate_motor(void);
extern bool ir_object_detected(uint8_t pin);

static bool motor_requested = false;

// CGI handler for LED
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Keep your existing LED handler code
    if (strcmp(pcParam[0] , "led") == 0){
        if(strcmp(pcValue[0], "0") == 0) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
            buzzer_turn_off(BUZZER_PIN);
        } else if(strcmp(pcValue[0], "1") == 0) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            buzzer_turn_on(BUZZER_PIN);
        }
    }
    return "/index.shtml";
}

// CGI handler for motor
const char * cgi_motor_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    if(can_activate_motor() && ir_object_detected(26)) {
        motor_requested = true;  // Just set the flag
    }
    return "/index.shtml";
}

// tCGI Struct
static const tCGI cgi_handlers[] = {
        {"/led.cgi", cgi_led_handler},
        {"/motor.cgi", cgi_motor_handler},
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 2);
    buzzer_init(BUZZER_PIN);
}