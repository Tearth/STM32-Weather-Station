#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stm32f30x.h>

#define TIMERS_COUNT 11

typedef struct TIMER_Definition
{
	int Id;
	TIM_TypeDef *Definition;

	void (*TimerClockCmd)(uint32_t, FunctionalState);
	uint32_t TimerClock;
	uint8_t IRQChannel;
} TIMER_Definition;

#ifdef __cplusplus
 extern "C" {
#endif

bool TIMER_Enable(TIM_TypeDef *timx, unsigned int milliseconds);
TIMER_Definition *TIMER_GetDefinition(TIM_TypeDef *usartx);

#ifdef __cplusplus
}
#endif

#endif
