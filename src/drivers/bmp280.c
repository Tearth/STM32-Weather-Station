#include <drivers/bmp280.h>

int bmp280_calibration_data[12];

bool bmp280_calibration_data_loaded;
bool bmp280_enabled;

bool BMP280_Enable()
{
	if(bmp280_enabled) return false;

	if(!bmp280_calibration_data_loaded)
	{
		int addr = 0x88;
		for(int i=0; i<12; i++)
		{
			bmp280_calibration_data[i] = BMP280_ReadRegisterValue(addr) | (BMP280_ReadRegisterValue(addr + 1) << 8);
			addr += 2;
		}

		bmp280_calibration_data_loaded = true;
	}

	BMP280_WriteRegisterValue(0xF4, 0x5D);

	bmp280_enabled = true;
	return true;
}

bool BMP280_Disable()
{
	if(!bmp280_enabled) return false;

	bmp280_enabled = false;
	return true;
}

float BMP280_ReadTemperature()
{
	int msb = BMP280_ReadRegisterValue(BMP280_REGISTER_TEMP_MSB);
	int lsb = BMP280_ReadRegisterValue(BMP280_REGISTER_TEMP_LSB);
	int xlsb = BMP280_ReadRegisterValue(BMP280_REGISTER_TEMP_XLSB);

	int raw = (msb << 12) | (lsb << 4) | (xlsb >> 4);

	int t_fine;
	int var1, var2, T;

	unsigned short dig_T1 = bmp280_calibration_data[BMP280_CALIB_T1_INDEX];
	signed short dig_T2 = bmp280_calibration_data[BMP280_CALIB_T2_INDEX];
	signed short dig_T3 = bmp280_calibration_data[BMP280_CALIB_T3_INDEX];

	var1 = ((((raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11;
	var2 = (((((raw >> 4) - dig_T1) * ((raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;

	return T / 100.0f;
}

void BMP280_WriteRegisterValue(uint8_t registerAddress, uint8_t registerValue)
{
	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_BUSY) != RESET);

	I2C_TransferHandling(BMP280_I2C_INTERFACE, BMP280_I2C_ADDRESS << 1, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_TXIS) == RESET);

	I2C_SendData(BMP280_I2C_INTERFACE, 0x80 | registerAddress);
	I2C_SendData(BMP280_I2C_INTERFACE, registerValue);

	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_TC) == RESET);
	I2C_GenerateSTOP(BMP280_I2C_INTERFACE, ENABLE);
}

uint8_t BMP280_ReadRegisterValue(uint8_t registerAddress)
{
	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_BUSY) != RESET);

	I2C_TransferHandling(BMP280_I2C_INTERFACE, BMP280_I2C_ADDRESS << 1, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_TXIS) == RESET);

	I2C_SendData(BMP280_I2C_INTERFACE, 0x80 | registerAddress);

	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_TC) == RESET);

	I2C_TransferHandling(BMP280_I2C_INTERFACE, BMP280_I2C_ADDRESS << 1, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	I2C_ReceiveData(BMP280_I2C_INTERFACE);
	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_RXNE) == RESET);

	uint8_t value = I2C_ReceiveData(BMP280_I2C_INTERFACE);
	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_RXNE) == RESET);

	while(I2C_GetFlagStatus(BMP280_I2C_INTERFACE, I2C_ISR_STOPF) == RESET);
	I2C_ClearFlag(BMP280_I2C_INTERFACE, I2C_ICR_STOPCF);

	Delay(2);
	return value;
}
