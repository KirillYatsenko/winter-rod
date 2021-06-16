// Description:
//  The most basic strategy moves from 0 degrees to 180

#include "esp_err.h"
#include "esp_log.h"
#include "servo_driver.h"

esp_err_t strategy_1(uint8_t speed_percent, uint8_t amplitude_percent)
{
    servo_driver_drive(speed_percent, amplitude_percent);

    return ESP_OK;
}