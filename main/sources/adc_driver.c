
#include <stdio.h>
#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "strategy_runner.h"

#define TAG                "ADC_DRIVER"
#define SPEED_ADC_GPIO     14
#define AMPLITUDE_ADC_GPIO 27
#define ADC_MAX_VALUE      4096.0

static void read_adc_task(void *params);

esp_err_t adc_driver_init(void)
{
    esp_err_t result = ESP_OK;

    ESP_LOGI(TAG, "adc_driver_init called");

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);

    xTaskCreate(read_adc_task, "read_adc_task", 2048, NULL, 1, NULL);

    return result;
}

static void read_adc_task(void *params)
{
    int speed_raw;
    int amplitude_raw;

    while (true) {
        speed_raw = adc1_get_raw(ADC1_CHANNEL_6);
        amplitude_raw = adc1_get_raw(ADC1_CHANNEL_7);

        speed_percent = (speed_raw / ADC_MAX_VALUE) * 100;
        amplitude_percent = (amplitude_raw / ADC_MAX_VALUE) * 100;

        ESP_LOGI(TAG,
                 "speed_raw: (%d), speed_percent: (%d), amplitude_raw: (%d), "
                 "amplitude_percent: (%d)",
                 speed_raw, speed_percent, amplitude_raw, amplitude_percent);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
