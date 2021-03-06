#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#define TS30 0x1FFFF7B8
#define TS110 0x1FFFF7C2

#define CPUTEMP_MEASUREMENTS_COUNT 50

#include <stm32f30x.h>
#include <stdbool.h>
#include <cpu/adc.h>
#include <library/delay.h>

#ifdef __cplusplus
 extern "C" {
#endif

bool CPUTEMP_Enable();
bool CPUTEMP_Disable();
float CPUTEMP_Read();

#ifdef __cplusplus
}
#endif


#endif
