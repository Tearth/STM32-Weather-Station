#include <cpu/timer.h>

TIMER_Definition TimerDefinitions[TIMERS_COUNT] =
{
	{
		.Id = 0,
		.Definition = TIM1,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM1,
		.IRQChannel = TIM1_CC_IRQn
	},
	{
		.Id = 1,
		.Definition = TIM2,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM2,
		.IRQChannel = TIM2_IRQn
	},
	{
		.Id = 2,
		.Definition = TIM3,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM3,
		.IRQChannel = TIM3_IRQn
	},
	{
		.Id = 3,
		.Definition = TIM4,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM4,
		.IRQChannel = TIM4_IRQn
	},
	{
		.Id = 4,
		.Definition = TIM6,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM6,
		.IRQChannel = TIM6_DAC1_IRQn
	},
	{
		.Id = 5,
		.Definition = TIM7,
		.TimerClockCmd = RCC_APB1PeriphClockCmd,
		.TimerClock = RCC_APB1Periph_TIM7,
		.IRQChannel = TIM7_IRQn
	},
	{
		.Id = 6,
		.Definition = TIM8,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM8,
		.IRQChannel = TIM8_UP_IRQn
	},
	{
		.Id = 7,
		.Definition = TIM15,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM15,
		.IRQChannel = TIM1_BRK_TIM15_IRQn
	},
	{
		.Id = 8,
		.Definition = TIM16,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM16,
		.IRQChannel = TIM1_UP_TIM16_IRQn
	},
	{
		.Id = 9,
		.Definition = TIM17,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM17,
		.IRQChannel = TIM1_TRG_COM_TIM17_IRQn
	},
	{
		.Id = 10,
		.Definition = TIM20,
		.TimerClockCmd = RCC_APB2PeriphClockCmd,
		.TimerClock = RCC_APB2Periph_TIM20,
		.IRQChannel = TIM20_UP_IRQn
	},
};

bool timers_enabled[TIMERS_COUNT];

bool TIMER_Enable(TIM_TypeDef *timx, unsigned int milliseconds)
{
	TIMER_Definition *definition;
	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;

	if(definition = TIMER_GetDefinition(timx), definition == 0)
	{
		return false;
	}

	if(timers_enabled[definition->Id]) return false;
	timers_enabled[definition->Id] = true;

	definition->TimerClockCmd(definition->TimerClock, ENABLE);

	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = (SystemCoreClock / 2000) - 1;
	tim.TIM_Period = (milliseconds * 2) - 1;
	TIM_TimeBaseInit(timx, &tim);

	nvic.NVIC_IRQChannel = definition->IRQChannel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	TIM_ITConfig(timx, TIM_IT_Update, ENABLE);
	TIM_Cmd(timx, ENABLE);

	return true;
}

bool TIMER_Disable(TIM_TypeDef *timx)
{
	TIMER_Definition *definition;

	if(definition = TIMER_GetDefinition(timx), definition == 0)
	{
		return false;
	}

	if(!timers_enabled[definition->Id]) return false;
	timers_enabled[definition->Id] = false;

	TIM_ITConfig(timx, TIM_IT_Update, DISABLE);
	TIM_Cmd(timx, DISABLE);

	return true;
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
