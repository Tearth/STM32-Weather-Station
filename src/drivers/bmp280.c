#include <drivers/bmp280.h>

int bmp280_calibration_data[12];
int bmp_280_t_fine;

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

	int var1, var2, T;

	unsigned short dig_T1 = bmp280_calibration_data[BMP280_CALIB_T1_INDEX];
	signed short dig_T2 = bmp280_calibration_data[BMP280_CALIB_T2_INDEX];
	signed short dig_T3 = bmp280_calibration_data[BMP280_CALIB_T3_INDEX];

	var1 = ((((raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11;
	var2 = (((((raw >> 4) - dig_T1) * ((raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14;
	bmp_280_t_fine = var1 + var2;
	T = (bmp_280_t_fine * 5 + 128) >> 8;

	return T / 100.0f;
}

float BMP280_ReadPressure()
{
	int msb = BMP280_ReadRegisterValue(BMP280_REGISTER_PRESS_MSB);
	int lsb = BMP280_ReadRegisterValue(BMP280_REGISTER_PRESS_LSB);
	int xlsb = BMP280_ReadRegisterValue(BMP280_REGISTER_PRESS_XLSB);

	int raw = (msb << 12) | (lsb << 4) | (xlsb >> 4);

	unsigned short dig_P1 = bmp280_calibration_data[BMP280_CALIB_P1_INDEX];
	signed short dig_P2 = bmp280_calibration_data[BMP280_CALIB_P2_INDEX];
	signed short dig_P3 = bmp280_calibration_data[BMP280_CALIB_P3_INDEX];
	signed short dig_P4 = bmp280_calibration_data[BMP280_CALIB_P4_INDEX];
	signed short dig_P5 = bmp280_calibration_data[BMP280_CALIB_P5_INDEX];
	signed short dig_P6 = bmp280_calibration_data[BMP280_CALIB_P6_INDEX];
	signed short dig_P7 = bmp280_calibration_data[BMP280_CALIB_P7_INDEX];
	signed short dig_P8 = bmp280_calibration_data[BMP280_CALIB_P8_INDEX];
	signed short dig_P9 = bmp280_calibration_data[BMP280_CALIB_P9_INDEX];

	double var1, var2, p;
	var1 = ((double)bmp_280_t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);

	if (var1 == 0.0)
	{
		return 0;
	}

	p = 1048576.0 - (double)raw;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;

	return p / 100.0f + BMP280_MY_ALTITUDE * 0.12f;
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
