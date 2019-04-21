#ifndef ADC_H
#define ADC_H

#include <stm32f30x.h>
#include <stdbool.h>

#define ADC_COUNT 4

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct ADC_Definition
{
	ADC_TypeDef *Definition;

	uint32_t Clock;
	uint32_t ClockDivider;
} ADC_Definition;

bool ADC_Enable(ADC_TypeDef *adcx);
bool ADC_Disable(ADC_TypeDef *adcx);
int ADC_Read(ADC_TypeDef *adcx, uint8_t channel);
ADC_Definition *ADC_GetDefinition(ADC_TypeDef *adcx);

#ifdef __cplusplus
}
#endif

#endif
