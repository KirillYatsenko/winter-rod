#include "servo_driver.h"
#include <math.h>
#include <stdio.h>
#include "driver/mcpwm.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/mcpwm_periph.h"

#define TAG                  "SERVO_DRIVER"
#define SERVO_GPIO           13
#define SERVO_FAST_DELAY_MS  1.6   // per angle
#define SERVO_MIN_DELAY      2   // per angle
#define SERVO_MAX_DELAY      10    // per angle
#define SERVO_MIN_PULSEWIDTH 500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH 2400  // Maximum pulse width in microsecond
#define SERVO_MAX_DEGREE     90

static uint32_t servo_duty_calculate(uint32_t degree_of_rotation);
static uint32_t servo_delay_calculate_per_angle(uint8_t speed_percent);
static esp_err_t servo_fast_drive(uint16_t amplitude);

static esp_err_t servo_normal_drive(uint16_t initial_value,
                                    uint16_t destination_value,
                                    int16_t increment_value,
                                    uint8_t speed_percent);

esp_err_t servo_driver_init(void)
{
    esp_err_t result = ESP_OK;
    mcpwm_config_t pwm_config;

    ESP_LOGI(TAG, "pwm_driver_init called");

    if ((result = mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_GPIO)) !=
        ESP_OK) {
        return result;
    }

    pwm_config.frequency = 50;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    if ((result = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config)) !=
        ESP_OK) {
        return result;
    }

    return result;
}

esp_err_t servo_driver_drive(uint8_t speed_percent, uint8_t amplitude_percent)
{
    esp_err_t result = ESP_OK;
    uint16_t amplitude = SERVO_MAX_DEGREE * amplitude_percent / 100;

    // if (speed_percent > 90) {
    //     if ((result = servo_fast_drive(amplitude)) != ESP_OK) {
    //         return result;
    //     }
    // }
    // else {
        if ((result = servo_normal_drive(amplitude, 1, -10, speed_percent)) !=
            ESP_OK) {
            return result;
        }

        if ((result = servo_normal_drive(0, amplitude, +10, speed_percent)) !=
            ESP_OK) {
            return result;
        }
    // }

    return result;
}

static esp_err_t servo_fast_drive(uint16_t amplitude)
{
    esp_err_t result = ESP_OK;
    uint32_t angle;

    angle = servo_duty_calculate(0);
    if ((result = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,
                                       angle)) != ESP_OK) {
        return result;
    }
    ESP_LOGI(TAG, "amplitude = %d", amplitude);
    ESP_LOGI(TAG, "delay ms = %d", (int)(SERVO_FAST_DELAY_MS * amplitude));

    vTaskDelay(SERVO_FAST_DELAY_MS * amplitude / portTICK_PERIOD_MS);

    angle = servo_duty_calculate(amplitude);
    if ((result = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,
                                       angle)) != ESP_OK) {
        return result;
    }

    vTaskDelay(SERVO_FAST_DELAY_MS * amplitude / portTICK_PERIOD_MS);
    // vTaskDelay(SERVO_FAST_DELAY_MS / portTICK_PERIOD_MS);
    return result;
}

static esp_err_t servo_normal_drive(uint16_t initial_value,
                                    uint16_t destination_value,
                                    int16_t increment_value,
                                    uint8_t speed_percent)
{
    esp_err_t result = ESP_OK;
    uint32_t angle, delay;

    for (size_t i = initial_value; i < destination_value;
         i += increment_value) {
        angle = servo_duty_calculate(i);

        if ((result = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0,
                                           MCPWM_OPR_A, angle)) != ESP_OK) {
            return result;
        }

        delay = servo_delay_calculate_per_angle(speed_percent) *
                abs(increment_value);

        ESP_LOGI(TAG, "amplitude = %d", destination_value);
        ESP_LOGI(TAG, "delay ms = %d", delay);

        vTaskDelay(delay / portTICK_PERIOD_MS);
    }

    return result;
}

static uint32_t servo_duty_calculate(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH +
                      (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) *
                        (degree_of_rotation)) /
                       (SERVO_MAX_DEGREE)));

    return cal_pulsewidth;
}

static uint32_t servo_delay_calculate_per_angle(uint8_t speed_percent)
{
    uint32_t delay = (SERVO_MIN_DELAY + ((SERVO_MAX_DELAY - SERVO_MIN_DELAY) *
                                         ((100 - speed_percent) / 100.0)));

    return delay;
}
