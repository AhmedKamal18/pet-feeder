#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/buzzer.h"
#include "drivers/servo.h"
#include "drivers/ultrasonic.h"
#include "drivers/ir_sensor.h"

// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "iPhone 13";
const char WIFI_PASSWORD[] = "ahmed131203";

#define IR_PIN 26
#define SERVO_PIN 2
#define BUZZER_PIN 3
#define TRIGGER_PIN 14
#define ECHO_PIN 15

// Configuration constants
#define DISTANCE_THRESHOLD 30.0f  // Distance in cm to detect pet presence
#define MOTOR_COOLDOWN_MS 10000   // 1 minute cooldown between motor activations

TaskHandle_t dispense_task_handle = NULL;

// Declare task handles
TaskHandle_t wifi_task_handle = NULL;
TaskHandle_t led_task_handle = NULL;
TaskHandle_t control_task_handle = NULL;

// Shared variables
servo_t servo;
ultrasonic_t ultrasonic;
static TickType_t last_motor_activation = 0;

void init() {
    ir_sensor_init(IR_PIN);
    ultrasonic_init(&ultrasonic, TRIGGER_PIN, ECHO_PIN);
    servo_attach(&servo, SERVO_PIN);
    buzzer_init(BUZZER_PIN);
}

void led_task(void *params) {
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(xDelay);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(xDelay);
    }
}

void activate_motor() {
    servo_write(&servo, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
    servo_write(&servo, 60);
    vTaskDelay(pdMS_TO_TICKS(500));
    servo_write(&servo, 0);
    last_motor_activation = xTaskGetTickCount();
}

void dispense_task(void *params) {
    while (1) {
        if (motor_requested) {
            servo_write(&servo, 0);
            vTaskDelay(pdMS_TO_TICKS(500));
            servo_write(&servo, 60);
            vTaskDelay(pdMS_TO_TICKS(500));
            servo_write(&servo, 0);
            last_motor_activation = xTaskGetTickCount();
            motor_requested = false;
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Check every 100ms
    }
}


bool can_activate_motor() {
    TickType_t current_time = xTaskGetTickCount();
    TickType_t time_since_last = current_time - last_motor_activation;
    return time_since_last >= pdMS_TO_TICKS(MOTOR_COOLDOWN_MS);
}

void control_task(void *params) {
    while (1) {
        float distance = ultrasonic_get_distance(&ultrasonic);
        bool ir_detected = ir_object_detected(IR_PIN);

        // Debug output
        printf("Distance: %.2f cm, IR: %d\n", distance, ir_detected);

        if (distance <= DISTANCE_THRESHOLD) {  // Pet is close
            if (ir_detected) {  // Food is present
                if (can_activate_motor()) {
                    printf("Activating motor - food present and pet detected\n");
                    activate_motor();
                } else {
                    printf("Motor in cooldown period\n");
                }
            } else {  // No food present
                printf("Activating buzzer - no food and pet detected\n");
                buzzer_turn_on(BUZZER_PIN);
                vTaskDelay(pdMS_TO_TICKS(500));
                buzzer_turn_off(BUZZER_PIN);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));  // Check every 500ms
    }
}

void wifi_init() {
    cyw43_arch_enable_sta_mode();

    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0) {
        printf("Attempting to connect...\n");
        sleep_ms(2000);
    }
    printf("Connected! \n");

    httpd_init();
    printf("Http server initialised\n");

    struct netif *netif = netif_list;
    if (netif != NULL && netif_is_up(netif) && !ip4_addr_isany(netif_ip4_addr(netif))) {
        printf("IP Address: %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
    } else {
        printf("Failed to get IP address\n");
    }

    ssi_init();
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
}

int main() {
    stdio_init_all();
    init();

    if (cyw43_arch_init() != 0) {
        printf("Failed to initialize CYW43\n");
        return -1;
    }

    wifi_init();

    // Create control task instead of separate sensor tasks
    xTaskCreate(control_task, "Control Task", 512, NULL, 2, &control_task_handle);
    xTaskCreate(dispense_task, "Dispense Task", 512, NULL, 1, &dispense_task_handle);
//    xTaskCreate(led_task, "LED Task", 512, NULL, 1, &led_task_handle);

    vTaskStartScheduler();

    while (1) {
        // Handle errors
    }
}