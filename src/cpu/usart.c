#include <cpu/usart.h>

USART_Definition USART_Definitions[USART_COUNT] =
{
	{
		.Definition = USART1,
		.ClockCmd = RCC_APB2PeriphClockCmd,
		.Clock = RCC_APB2Periph_USART1,
		.PortClock = RCC_AHBPeriph_GPIOA,
		.Port = GPIOA,
		.RxPin = GPIO_Pin_10,
		.TxPin = GPIO_Pin_9,
		.RxPinSource = GPIO_PinSource10,
		.TxPinSource = GPIO_PinSource9
	},
	{
		.Definition = USART2,
		.ClockCmd = RCC_APB1PeriphClockCmd,
		.Clock = RCC_APB1Periph_USART2,
		.PortClock = RCC_AHBPeriph_GPIOA,
		.Port = GPIOA,
		.RxPin = GPIO_Pin_3,
		.TxPin = GPIO_Pin_2,
		.RxPinSource = GPIO_PinSource3,
		.TxPinSource = GPIO_PinSource2
	},
	{
		.Definition = USART3,
		.ClockCmd = RCC_APB1PeriphClockCmd,
		.Clock = RCC_APB1Periph_USART3,
		.PortClock = RCC_AHBPeriph_GPIOB,
		.Port = GPIOB,
		.RxPin = GPIO_Pin_11,
		.TxPin = GPIO_Pin_10,
		.RxPinSource = GPIO_PinSource11,
		.TxPinSource = GPIO_PinSource10
	}
};

bool USART_Enable(USART_TypeDef *usartx, uint32_t baudRate)
{
	USART_Definition *definition = USART_GetDefinition(usartx);
	if(definition == 0)
	{
		return false;
	}

	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	RCC_AHBPeriphClockCmd(definition->PortClock, ENABLE);
	definition->ClockCmd(definition->Clock, ENABLE);

	GPIO_PinAFConfig(GPIOA, definition->RxPinSource, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA, definition->TxPinSource, GPIO_AF_7);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = definition->RxPin | definition->TxPin;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(definition->Port, &gpio);

	USART_StructInit(&uart);
	uart.USART_BaudRate = baudRate;
	USART_Init(usartx, &uart);

	USART_Cmd(usartx, ENABLE);

	return true;
}

USART_Definition *USART_GetDefinition(USART_TypeDef *usartx)
{
	for(int i = 0; i < USART_COUNT; i++)
	{
		if (USART_Definitions[i].Definition == usartx)
		{
			return &USART_Definitions[i];
		}
	}

	return 0;
}
