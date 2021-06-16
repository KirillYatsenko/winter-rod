
// Description:
//  Short moves then one big one

#include "esp_err.h"
#include "esp_log.h"
#include "servo_driver.h"

esp_err_t strategy_2(uint8_t speed_percent, uint8_t amplitude_percent)
{
    servo_driver_drive(speed_percent, 20);
    servo_driver_drive(speed_percent, 20);
    servo_driver_drive(speed_percent, 20);

    servo_driver_drive(speed_percent, 100);

    return ESP_OK;
}