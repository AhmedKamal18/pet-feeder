#include "hardware/adc.h"
#include "ir_sensor.h"
#include "pico/stdlib.h"

void ir_sensor_init(uint8_t ir_pin) {
    // Initialize the ADC
    adc_init();

    // Ensure the ADC is powered on
    adc_set_temp_sensor_enabled(false);

    // Select the correct ADC input (0-3 for GPIO 26-29)
    if (ir_pin >= 26 && ir_pin <= 29) {
        adc_gpio_init(ir_pin);
    } else {
        printf("Invalid IR sensor pin! Use GPIO 26-29 for ADC.\n");
        return;
    }
}

uint32_t ir_sensor_read(uint8_t ir_pin) {
    // Validate pin range
    if (ir_pin < 26 || ir_pin > 29) {
        printf("Invalid IR sensor pin!\n");
        return 0;
    }

    // Select the corresponding ADC channel for the pin
    uint8_t adc_channel = ir_pin - 26; // ADC channels correspond to GPIO 26-29
    adc_select_input(adc_channel);

    // Small delay to ensure ADC is ready (optional but can improve stability)
    sleep_us(10);

    // Read the raw ADC value (12-bit resolution: 0-4095)
    uint32_t adc_value = adc_read();

    return adc_value;
}

// Optional: Convert raw ADC value to voltage
float ir_sensor_read_voltage(uint8_t ir_pin) {
    uint32_t adc_value = ir_sensor_read(ir_pin);

    // Convert to voltage (assuming 3.3V reference)
    // 12-bit AyDC, so max value is 4095
    float voltage = adc_value * 3.3f / 4095.0f;

    return voltage;
}

// Optional: Interpret IR sensor reading
bool ir_object_detected(uint8_t ir_pin) {
    uint32_t adc_value = ir_sensor_read(ir_pin);
    return adc_value > THRESHOLD;
}