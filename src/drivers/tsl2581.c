#include <drivers/tsl2581.h>

bool tsl_enabled;

bool TSL2581_Enable()
{
	if(tsl_enabled) return false;
	if(!I2C_Enable(TSL2586_I2C_INTERFACE))
	{
		return false;
	}

	TSL2581_WriteRegisterValue(TSL2581_COMMAND_CONTROL, 0x01);
	TSL2581_WriteRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_TIMING, TSL2581_ITIME_99_9);

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

	TSL2581_WriteRegisterValue(TSL2581_COMMAND_CONTROL, 0x00);

	tsl_enabled = false;
	return true;
}

int TSL2581_ReadInsolation()
{
	TSL2581_WriteRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_CONTROL, 0x03);
	TSL2581_WaitForResults();

	int ch0Low = TSL2581_ReadRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_DATA0LOW);
	int ch0High = TSL2581_ReadRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_DATA0HIGH);
	int ch1Low = TSL2581_ReadRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_DATA1LOW);
	int ch1High = TSL2581_ReadRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_DATA1HIGH);

	TSL2581_WriteRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_CONTROL, 0x01);

	return TSL2581_CalculateLux((ch0High << 8) | ch0Low, (ch1High << 8) | ch1Low);
}

void TSL2581_WaitForResults()
{
	while((TSL2581_ReadRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_CONTROL) & 16) == 0);
}

int TSL2581_CalculateLux(int ch0, int ch1)
{

}

void TSL2581_WriteRegisterValue(uint8_t registerAddress, uint8_t registerValue)
{
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_BUSY) != RESET);

	I2C_TransferHandling(TSL2586_I2C_INTERFACE, TSL2586_I2C_ADDRESS << 1, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_TXIS) == RESET);

	I2C_SendData(TSL2586_I2C_INTERFACE, 0x80 | registerAddress);
	I2C_SendData(TSL2586_I2C_INTERFACE, registerValue);

	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_TC) == RESET);
	I2C_GenerateSTOP(TSL2586_I2C_INTERFACE, ENABLE);
}

uint8_t TSL2581_ReadRegisterValue(uint8_t registerAddress)
{
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_BUSY) != RESET);

	I2C_TransferHandling(TSL2586_I2C_INTERFACE, TSL2586_I2C_ADDRESS << 1, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_TXIS) == RESET);

	I2C_SendData(TSL2586_I2C_INTERFACE, 0x80 | registerAddress);

	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_TC) == RESET);

	I2C_TransferHandling(TSL2586_I2C_INTERFACE, TSL2586_I2C_ADDRESS << 1, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	I2C_ReceiveData(TSL2586_I2C_INTERFACE);
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_RXNE) == RESET);

	uint8_t value = I2C_ReceiveData(TSL2586_I2C_INTERFACE);
	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_RXNE) == RESET);

	while(I2C_GetFlagStatus(TSL2586_I2C_INTERFACE, I2C_ISR_STOPF) == RESET);
	I2C_ClearFlag(TSL2586_I2C_INTERFACE, I2C_ICR_STOPCF);

	return value;
}
