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

	void (*UsartClockCmd)(uint32_t, FunctionalState);
	uint32_t UsartClock;

	uint32_t GpioPortClock;
	GPIO_TypeDef *GpioPort;

	uint16_t RxPin;
	uint16_t TxPin;

	uint8_t RxPinSource;
	uint8_t TxPinSource;
} USART_Definition;

bool USART_Enable(USART_TypeDef *usartx, uint32_t baudRate);
bool USART_Disable(USART_TypeDef *usartx);
void USART_SendChar(USART_TypeDef *usartx, char c);
uint32_t USART_SendString(USART_TypeDef *usartx, const char *str);
USART_Definition *USART_GetDefinition(USART_TypeDef *usartx);

#ifdef __cplusplus
}
#endif

#endif
