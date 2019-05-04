#ifndef GP2_H
#define GP2_H

#include <stm32f30x.h>
#include <stdbool.h>
#include <cpu/adc.h>

#define GP2_DATA_PIN GPIO_Pin_11
#define GP2_ILED_PIN GPIO_Pin_2
#define GP2_PORT GPIOB

bool GP2_Enable();
bool GP2_Disable();

float GP2_Read();

#endif
