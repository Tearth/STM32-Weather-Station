#ifndef USART_H
#define USART_H

#define USART_COUNT 3

#include <stdbool.h>
#include <stm32f30x.h>

typedef struct USART_Definition
{
	int Id;
	USART_TypeDef *Definition;

	void (*UsartClockCmd)(uint32_t, FunctionalState);
	uint32_t UsartClock;

	uint32_t GpioPortClock;
	GPIO_TypeDef *GpioPort;

	uint32_t RxPin;
	uint32_t TxPin;

	uint8_t RxPinSource;
	uint8_t TxPinSource;
} USART_Definition;

#ifdef __cplusplus
 extern "C" {
#endif

bool USART_Enable(USART_TypeDef *usartx, unsigned int baudRate);
bool USART_Disable(USART_TypeDef *usartx);
void USART_SendChar(USART_TypeDef *usartx, char c);
char USART_ReceiveChar(USART_TypeDef *usartx);
int USART_SendString(USART_TypeDef *usartx, const char *str);
int USART_ReceiveString(USART_TypeDef *usartx, char *buffer);
bool USART_IsDataReadyToReceive(USART_TypeDef* usartx);
USART_Definition *USART_GetDefinition(USART_TypeDef *usartx);

#ifdef __cplusplus
}
#endif

#endif
