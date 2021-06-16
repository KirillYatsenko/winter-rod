#ifndef STRATEGY_H
#define STRATEGY_H

#include <inttypes.h>

#include "strategies/strategy_1.h"
#include "strategies/strategy_2.h"
#include "strategies/strategy_3.h"
#include "strategies/strategy_4.h"

extern uint8_t speed_percent;     // in percents from (0 - 100)%
extern uint8_t amplitude_percent; // in percents from (0 - 100)%

typedef esp_err_t (*strategy_t)(uint8_t speed_percent, uint8_t amplitude_percent);

#endif