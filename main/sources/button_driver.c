#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "strategy_runner.h"

#define TAG         "BUTTON_DRIVER"
#define BUTTON_GPIO 12

static xQueueHandle interput_queue;

static void button_pressed_task(void *params);
static void IRAM_ATTR gpio_isr_handler(void *args);

esp_err_t button_driver_init()
{
    esp_err_t result = ESP_OK;

    ESP_LOGI(TAG, "button_driver_init called");

    gpio_pad_select_gpio(BUTTON_GPIO);

    if ((result = gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT)) != ESP_OK) {
        return result;
    }

    if ((result = gpio_pulldown_en(BUTTON_GPIO)) != ESP_OK) {
        return result;
    }

    if ((result = gpio_pullup_dis(BUTTON_GPIO)) != ESP_OK) {
        return result;
    }

    if ((result = gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_POSEDGE)) !=
        ESP_OK) {
        return result;
    }

    interput_queue = xQueueCreate(10, sizeof(int));
    xTaskCreate(button_pressed_task, "button_pressed_task", 2048, NULL, 1,
                NULL);

    if ((result = gpio_install_isr_service(0)) != ESP_OK) {
        return result;
    }

    result = gpio_isr_handler_add(BUTTON_GPIO, gpio_isr_handler,
                                  (void *)BUTTON_GPIO);

    return result;
}

static void IRAM_ATTR gpio_isr_handler(void *args)
{
    int pinNumber = (int)args;
    xQueueSendFromISR(interput_queue, &pinNumber, NULL);
}

static void button_pressed_task(void *params)
{
    int pin_number = 0;
    while (true) {
        if (xQueueReceive(interput_queue, &pin_number, portMAX_DELAY)) {
            gpio_isr_handler_remove(pin_number);

            do {
                vTaskDelay(20 / portTICK_PERIOD_MS);  // debounce
            } while (gpio_get_level(pin_number) == 1);
        }

        ESP_LOGI(TAG, "Button Pressed PIN = %d", pin_number);

        ESP_ERROR_CHECK_WITHOUT_ABORT(strategy_runner_next_strategy());

        // re-enable the interrupt
        gpio_isr_handler_add(pin_number, gpio_isr_handler, (void *)pin_number);
    }
}
