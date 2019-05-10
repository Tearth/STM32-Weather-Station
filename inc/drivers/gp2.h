#ifndef GP2_H
#define GP2_H

#include <stm32f30x.h>
#include <stdbool.h>
#include <cpu/adc.h>
#include <library/delay.h>

#define GP2_DATA_PIN GPIO_Pin_11
#define GP2_ILED_PIN GPIO_Pin_2
#define GP2_PORT GPIOB

#define GP2_MEASUREMENTS_COUNT 50
#define GP2_NO_DUST_MILIVOLTS 600
#define GP2_DUST_VOLT_RATIO 0.2

bool GP2_Enable();
bool GP2_Disable();

int GP2_Read();

#endif
