#include <cpu/usart.h>

USART_Definition UsartDefinitions[USART_COUNT] =
{
	{
		.Id = 0,
		.Definition = USART1,
		.UsartClockCmd = RCC_APB2PeriphClockCmd,
		.UsartClock = RCC_APB2Periph_USART1,
		.GpioPortClock = RCC_AHBPeriph_GPIOA,
		.GpioPort = GPIOA,
		.RxPin = GPIO_Pin_10,
		.TxPin = GPIO_Pin_9,
		.RxPinSource = GPIO_PinSource10,
		.TxPinSource = GPIO_PinSource9
	},
	{
		.Id = 1,
		.Definition = USART2,
		.UsartClockCmd = RCC_APB1PeriphClockCmd,
		.UsartClock = RCC_APB1Periph_USART2,
		.GpioPortClock = RCC_AHBPeriph_GPIOA,
		.GpioPort = GPIOA,
		.RxPin = GPIO_Pin_3,
		.TxPin = GPIO_Pin_2,
		.RxPinSource = GPIO_PinSource3,
		.TxPinSource = GPIO_PinSource2
	},
	{
		.Id = 2,
		.Definition = USART3,
		.UsartClockCmd = RCC_APB1PeriphClockCmd,
		.UsartClock = RCC_APB1Periph_USART3,
		.GpioPortClock = RCC_AHBPeriph_GPIOB,
		.GpioPort = GPIOB,
		.RxPin = GPIO_Pin_11,
		.TxPin = GPIO_Pin_10,
		.RxPinSource = GPIO_PinSource11,
		.TxPinSource = GPIO_PinSource10
	}
};

bool usart_enabled[USART_COUNT];

bool USART_Enable(USART_TypeDef *usartx, unsigned int baudRate)
{
	USART_Definition *definition;
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	if(definition = USART_GetDefinition(usartx), definition == 0)
	{
		return false;
	}

	if(usart_enabled[definition->Id]) return false;

	RCC_AHBPeriphClockCmd(definition->GpioPortClock, ENABLE);
	definition->UsartClockCmd(definition->UsartClock, ENABLE);

	GPIO_PinAFConfig(GPIOA, definition->RxPinSource, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA, definition->TxPinSource, GPIO_AF_7);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = definition->RxPin | definition->TxPin;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(definition->GpioPort, &gpio);

	USART_StructInit(&uart);
	uart.USART_BaudRate = baudRate;
	USART_Init(usartx, &uart);

	USART_Cmd(usartx, ENABLE);

	usart_enabled[definition->Id] = true;
	return true;
}

bool USART_Disable(USART_TypeDef *usartx)
{
	USART_Definition *definition;
	if(definition = USART_GetDefinition(usartx), definition == 0)
	{
		return false;
	}

	if(usart_enabled[definition->Id]) return false;

	definition->UsartClockCmd(definition->UsartClock, DISABLE);
	USART_Cmd(usartx, DISABLE);

	usart_enabled[definition->Id] = false;
	return true;
}

void USART_SendChar(USART_TypeDef *usartx, char c)
{
	while (USART_GetFlagStatus(usartx, USART_FLAG_TXE) == RESET);
	USART_ClearFlag(usartx, USART_FLAG_ORE);

	USART_SendData(usartx, c);
}

char USART_ReceiveChar(USART_TypeDef *usartx)
{
	while (USART_GetFlagStatus(usartx, USART_FLAG_RXNE) == RESET);
	USART_ClearFlag(usartx, USART_FLAG_ORE);

	return USART_ReceiveData(usartx);
}

int USART_SendString(USART_TypeDef *usartx, const char *str)
{
	const char *ptr = str;
	int length = 0;

	while(*ptr != 0)
	{
		USART_SendChar(usartx, *ptr++);
		length++;
	}

	return length;
}

int USART_ReceiveString(USART_TypeDef *usartx, char *buffer)
{
	char c;
	int length = 0;

	do
	{
		c = USART_ReceiveChar(usartx);
		buffer[length++] = c;
	}
	while (c != 0x0A);

	buffer[length] = 0;
	return length;
}

bool USART_IsDataReadyToReceive(USART_TypeDef* usartx)
{
	return USART_GetFlagStatus(usartx, USART_FLAG_RXNE) == SET;
}

USART_Definition *USART_GetDefinition(USART_TypeDef *usartx)
{
	for(int i = 0; i < USART_COUNT; i++)
	{
		if (UsartDefinitions[i].Definition == usartx)
		{
			return &UsartDefinitions[i];
		}
	}

	return 0;
}
