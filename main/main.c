#include <stdio.h>
#include "adc_driver.h"
#include "button_driver.h"
#include "servo_driver.h"
#include "strategy_runner.h"

void app_main(void)
{
    esp_err_t result = ESP_OK;

    result = button_driver_init();
    ESP_ERROR_CHECK_WITHOUT_ABORT(result);  // not critical

    result = adc_driver_init();
    ESP_ERROR_CHECK_WITHOUT_ABORT(result);  // not critical

    result = servo_driver_init();
    ESP_ERROR_CHECK(result);  // critical

    result = strategy_runner_init();
    ESP_ERROR_CHECK(result);  // critical
}
