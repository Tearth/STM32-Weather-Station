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
	TSL2581_WriteRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_TIMING, TSL2581_ITIME_399_6);
	TSL2581_WriteRegisterValue(TSL2581_COMMAND | TSL2581_COMMAND_ANALOG, TSL2581_GAIN_1);

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
	unsigned long chScale0 = (1 << CH_SCALE);
	unsigned long chScale1 = chScale0;
	unsigned long channel0 = (ch0 * chScale0) >> CH_SCALE;
	unsigned long channel1 = (ch1 * chScale1) >> CH_SCALE;

	unsigned long ratio1 = 0;
	if (channel0 != 0)
	{
		ratio1 = (channel1 << (RATIO_SCALE+1)) / channel0;
	}

	unsigned long ratio = (ratio1 + 1) >> 1;
	unsigned int b, m;

	if ((ratio >= 0) && (ratio <= K1C))
	{
		b = B1C;
		m = M1C;
	}
	else if (ratio <= K2C)
	{
		b = B2C;
		m = M2C;
	}
	else if (ratio <= K3C)
	{
		b = B3C;
		m = M3C;
	}
	else if (ratio <= K4C)
	{
		b = B4C;
		m = M4C;
	}
	else if (ratio > K5C)
	{
		b = B5C;
		m = M5C;
	}

	unsigned long temp = (channel0 * b) - (channel1 * m) + (1 << (LUX_SCALE-1));
	unsigned long lux = temp >> LUX_SCALE;

	return lux;
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
