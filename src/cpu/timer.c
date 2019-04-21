#include <cpu/timer.h>

TIMER_Definition TimerDefinitions[TIMERS_COUNT] =
{
	{
		.Id = 0,
		.Definition = TIM1,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM1,
	},
	{
		.Id = 1,
		.Definition = TIM2,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM2,
	},
	{
		.Id = 2,
		.Definition = TIM3,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM3,
	},
	{
		.Id = 3,
		.Definition = TIM4,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM4,
	},
	{
		.Id = 4,
		.Definition = TIM6,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM6,
	},
	{
		.Id = 5,
		.Definition = TIM7,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM7,
	},
	{
		.Id = 6,
		.Definition = TIM8,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM8,
	},
	{
		.Id = 7,
		.Definition = TIM15,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM15,
	},
	{
		.Id = 8,
		.Definition = TIM16,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM16,
	},
	{
		.Id = 9,
		.Definition = TIM17,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM17,
	},
	{
		.Id = 10,
		.Definition = TIM20,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM20,
	},
};

bool TIMER_Enable(TIM_TypeDef timx)
{

}

TIMER_Definition *TIMER_GetDefinition(TIM_TypeDef *timx)
{
	for(int i = 0; i < TIMERS_COUNT; i++)
	{
		if (TimerDefinitions[i].Definition == timx)
		{
			return &TimerDefinitions[i];
		}
	}

	return 0;
}
