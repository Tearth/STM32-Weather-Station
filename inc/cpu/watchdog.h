#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stm32f30x.h>

#ifdef __cplusplus
 extern "C" {
#endif

void WATCHDOG_Enable(uint8_t prescaler);
void WATCHDOG_Disable();
void WATCHDOG_Reset();

#ifdef __cplusplus
}
#endif

#endif
