#ifndef TSL2581_H
#define TSL2581_H

#include <cpu/i2c.h>

#define TSL2586_I2C_INTERFACE I2C1
#define TSL2586_I2C_ADDRESS 0x39

bool TSL2581_Enable();
bool TSL2581_Disable();

void TSL2581_WriteRegisterValue(uint8_t registerAddress, uint8_t registerValue);

#endif
