#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#define TS30 0x1FFFF7B8
#define TS110 0x1FFFF7C2

#include <stm32f30x.h>
#include <cpu/adc.h>

void CPUTEMP_Enable();
void CPUTEMP_Disable();
float CPUTEMP_Read();

#endif
