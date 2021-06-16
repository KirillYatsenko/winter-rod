
// Description:
//  move with delay

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "servo_driver.h"

#define DELAY_MS 2000

esp_err_t strategy_3(uint8_t speed_percent, uint8_t amplitude_percent)
{
    servo_driver_drive(speed_percent, amplitude_percent);

    vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);

    return ESP_OK;
}