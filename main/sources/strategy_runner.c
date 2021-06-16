#include <inttypes.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "strategies.h"

#define TAG "STRATEGY_RUNNER"

uint8_t speed_percent = 0;
uint8_t amplitude_percent = 0;

static TaskHandle_t strategy_task_handler;

static uint8_t current_strategy_idx = 0;
static const strategy_t strategies[] = {strategy_1, strategy_2, strategy_3, strategy_4};

static void execute_strategy_task(void *params);

esp_err_t strategy_runner_init(void)
{
    ESP_LOGI(TAG, "strategy_runner_init called");

    xTaskCreate(execute_strategy_task, "execute_strategy_task", 2048, NULL, 1,
                &strategy_task_handler);

    return ESP_OK;
}

esp_err_t strategy_runner_next_strategy(void)
{
    ESP_LOGI(TAG, "strategy_runner_next_strategy called");

    vTaskSuspend(strategy_task_handler);

    uint8_t size = sizeof(strategies) / sizeof(strategies[0]);

    current_strategy_idx++;
    if (current_strategy_idx == size) current_strategy_idx = 0;

    ESP_LOGI(TAG, "strategy idx changed to = (%d)", current_strategy_idx);

    vTaskResume(strategy_task_handler);

    return ESP_OK;
}

static void execute_strategy_task(void *params)
{
    ESP_LOGI(TAG, "execute_strategy called");

    uint32_t temp = 0;

    while (1) {
        strategies[current_strategy_idx](speed_percent, amplitude_percent);

        ESP_LOGI(TAG, "executing strategy: (%d)", temp++);
    }
}