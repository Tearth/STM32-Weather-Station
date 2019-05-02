#include <drivers/tsl2581.h>

bool tsl_enabled;

bool TSL2581_Enable()
{
	if(tsl_enabled) return false;
	if(!I2C_Enable(TSL2586_I2C_INTERFACE))
	{
		return false;
	}

	tsl_enabled = true;
	return true;
}

bool TSL2581_Disable()
{
	if(!tsl_enabled) return false;
	if(!I2C_Disable(TSL2586_I2C_INTERFACE))
	{
		return false;
	}

	tsl_enabled = false;
	return true;
}

void TSL2581_WriteRegisterValue(uint8_t registerAddress, uint8_t registerValue)
{
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_BUSY) != RESET);

	I2C_TransferHandling(TSL2586_I2C_INTERFACE, TSL2586_I2C_ADDRESS << 1, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_TXIS) == RESET);

	I2C_SendData(TSL2586_I2C_INTERFACE, 0x80 | registerAddress);
	I2C_SendData(TSL2586_I2C_INTERFACE, registerValue);

	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_TC) == RESET);

	I2C_ClearFlag(TSL2586_I2C_INTERFACE, I2C_ICR_STOPCF);
}
