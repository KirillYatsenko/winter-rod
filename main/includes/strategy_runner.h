#ifndef STRATEGY_RUNNER_H
#define STRATEGY_RUNNER_H

#include <inttypes.h>
#include "esp_err.h"

extern uint8_t speed_percent;       // in percents from (0 - 100)%
extern uint8_t amplitude_percent;   // in percents from (0 - 100)%

esp_err_t strategy_runner_init(void);
esp_err_t strategy_runner_next_strategy(void);

#endif
