#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <inttypes.h>
#include "esp_err.h"

esp_err_t servo_driver_init(void);
esp_err_t servo_driver_drive(uint8_t speed_percent, uint8_t amplitude_percent);

#endif