
// Description:
//  Do nothing

#include <inttypes.h>
#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t strategy_4(uint8_t speed_percent, uint8_t amplitude_percent)
{
    vTaskDelay(500 / portTICK_PERIOD_MS);

    return ESP_OK;
}