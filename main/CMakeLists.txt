set(COMPONENT_SRCS "main.c"
                    "sources/strategy_runner.c"
                    "sources/button_driver.c"
                    "sources/servo_driver.c"
                    "sources/adc_driver.c"
                    "sources/strategies/strategy_1.c"
                    "sources/strategies/strategy_2.c"
                    "sources/strategies/strategy_3.c"
                    "sources/strategies/strategy_4.c")

set(COMPONENT_ADD_INCLUDEDIRS "." "includes/" "includes/strategies/")

register_component()