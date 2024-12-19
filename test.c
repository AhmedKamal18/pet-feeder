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

// Declare task handles
TaskHandle_t wifi_task_handle = NULL;
TaskHandle_t led_task_handle = NULL;
TaskHandle_t motor_task_handle = NULL;

void led_task(void *params) {
    printf("LED Task: Starting and waiting for notification...\n");

    // Wait for a notification
    uint32_t ulNotificationValue;
    BaseType_t xResult = xTaskNotifyWait(
            0x00,           // Clear no bits on entry
            0xFFFFFFFF,     // Clear all bits on exit
            &ulNotificationValue,
            portMAX_DELAY   // Wait indefinitely
    );

    // Check if notification was received
    if (xResult == pdTRUE) {
        printf("LED Task: Received notification! Value: %lu\n", ulNotificationValue);
    } else {
        printf("LED Task: Failed to receive notification\n");
    }

    // LED blinking logic
    const TickType_t xDelay = pdMS_TO_TICKS(500); // 500ms delay between toggles

    while (1) {
        // Turn LED on
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(xDelay);

        // Turn LED off
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(xDelay);
    }
}

//void motor_task(void *params) {
//    printf("Motor Task: Starting and waiting for notification...\n");
//
//    // Wait for a notification
//    uint32_t ulNotificationValue;
//    BaseType_t xResult = xTaskNotifyWait(
//            0x00,           // Clear no bits on entry
//            0xFFFFFFFF,     // Clear all bits on exit
//            &ulNotificationValue,
//            portMAX_DELAY   // Wait indefinitely
//    );
//
//    // Check if notification was received
//    if (xResult == pdTRUE) {
//        printf("Motor Task: Received notification! Value: %lu\n", ulNotificationValue);
//    } else {
//        printf("Motor Task: Failed to receive notification\n");
//    }
//
//    servo_attach(&servo, SERVO_PIN);
////    while (1){
////        servo_write(&servo, 0);
////        vTaskDelay(pdMS_TO_TICKS(2000));
////        servo_write(&servo, 90);
////    }
//}

// Task to handle Wi-Fi connection
void wifi_task(void *params) {
    printf("WiFi Task: Starting connection process...\n");

    cyw43_arch_enable_sta_mode();

    // Connect to the WiFI network - loop until connected
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0) {
        printf("Attempting to connect...\n");
        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
    printf("Connected! \n");

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Print the IP address
    struct netif *netif = netif_list;
    if (netif != NULL && netif_is_up(netif) && !ip4_addr_isany(netif_ip4_addr(netif))) {
        printf("IP Address: %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
    } else {
        printf("Failed to get IP address\n");
    }

    // Configure SSI and CGI handler
    ssi_init();
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");

    // Notify the motor task
    printf("WiFi Task: Attempting to notify motor task...\n");
    BaseType_t xResult = xTaskNotify(
            motor_task_handle,      // Target task handle (motor_task_handle)
            1,                      // Notification value
            eSetBits                // Action to take with notification
    );

    if (xResult == pdTRUE) {
        printf("WiFi Task: Successfully notified motor task\n");
    } else {
        printf("WiFi Task: Failed to notify motor task\n");
    }

    // Suspend this task after initialization
    vTaskSuspend(NULL);
}

// FreeRTOS main entry point
int main() {
    stdio_init_all();

    if (cyw43_arch_init() != 0) {
        printf("Failed to initialize CYW43\n");
        return -1;
    }

    ir_sensor_init(IR_PIN);

    servo_t servo;
    servo_attach(&servo, SERVO_PIN);

    ultrasonic_t ultrasonic;
    ultrasonic_init(&ultrasonic, TRIGGER_PIN, ECHO_PIN);

    while (1){
        printf( "ir: %i\n", ir_sensor_read(IR_PIN));
        sleep_ms(1000);
    }
}
