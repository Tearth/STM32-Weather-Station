#ifndef SYSCLOCK_H
#define SYSCLOCK_H

#include <stdbool.h>
#include <stm32f30x.h>

#ifdef __cplusplus
 extern "C" {
#endif

bool SYSCLOCK_Enable();
uint64_t SYSCLOCK_GetSystemClock();
void SysTick_Handler();

#ifdef __cplusplus
 extern "C" {
#endif

#endif
