#include <station/core.h>

bool Core_DoMeasurementsAndSend()
{
	if(!I2C_Enable(I2C1))
	{
		return false;
	}

	printf("Enabling modules...\r\n");
	if(!ESP8266_Enable() || !DHT22_Enable() || !TSL2581_Enable() || !BMP280_Enable() || !GP2_Enable())
	{
		return false;
	}
	Delay(2000);

	printf("Reading humidity...\r\n");

	float humidity, temperature;
	if(!DHT22_GetHumidityAndTemperature(&humidity, &temperature))
	{
		return false;
	}

	printf("Humidity = %.2f %%, Temperature = %.2f C\r\n", humidity, temperature);

	printf("Reading insolation...\r\n");
	printf("Insolation = %d Lux\r\n", TSL2581_ReadInsolation());

	printf("Reading temperature...\r\n");
	printf("Temperature = %f C\r\n", BMP280_ReadTemperature());

	printf("Reading pressure...\r\n");
	printf("Pressure = %f hPa\r\n", BMP280_ReadPressure());

	printf("Reading air quality...\r\n");
	printf("Air quality = %f ug/m3\r\n", GP2_Read());

	printf("Setting mode...\r\n");
	if(!ESP8266_SetMode(ESP8266_Mode_Client))
	{
		return false;
	}

	printf("Connecting to %s...\r\n", WIFI_SSID);
	if(!ESP8266_Connect(WIFI_SSID, WIFI_PASSWORD))
	{
		return false;
	}

	printf("Sending measurements...\r\n");
	if(!ESP8266_SendPOST(SERVER_IP, 80, SERVER_PATH, "val1=123&val2=456"))
	{
		return false;
	}

	printf("Disabling modules...\r\n");
	if(!ESP8266_Disable() || !DHT22_Disable() || !TSL2581_Disable() || !BMP280_Disable() || !GP2_Disable())
	{
		return false;
	}

	if(!I2C_Disable(I2C1))
	{
		return false;
	}

	return true;
}
