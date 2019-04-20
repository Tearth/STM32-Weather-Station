#ifndef USART_H
#define USART_H

#define USART_COUNT 3

#include <stdbool.h>
#include <stm32f30x.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct USART_Definition
{
	USART_TypeDef *Definition;

	void (*ClockCmd)(uint32_t, FunctionalState);
	uint32_t Clock;

	GPIO_TypeDef *Port;
	uint32_t PortClock;
	uint16_t RxPin;
	uint16_t TxPin;
	uint8_t RxPinSource;
	uint8_t TxPinSource;
} USART_Definition;

bool USART_Enable(USART_TypeDef *usartx, uint32_t baudRate);
USART_Definition *USART_GetDefinition(USART_TypeDef *usartx);

#ifdef __cplusplus
}
#endif

#endif
