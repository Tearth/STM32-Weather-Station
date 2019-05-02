#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stm32f30x.h>

#define I2C_COUNT 2

typedef struct I2C_Definition
{
	int Id;
	I2C_TypeDef *Definition;

	void (*I2CClockCmd)(uint32_t, FunctionalState);
	uint32_t I2CClock;

	GPIO_TypeDef *GpioPort;

	uint32_t SDAPin;
	uint32_t SCLPin;
} I2C_Definition;

#ifdef __cplusplus
 extern "C" {
#endif

bool I2C_Enable(I2C_TypeDef *i2cx);
bool I2C_Disable(I2C_TypeDef *i2cx);
I2C_Definition *I2C_GetDefinition(I2C_TypeDef *i2cx);

#ifdef __cplusplus
}
#endif

#endif
