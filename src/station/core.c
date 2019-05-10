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

	float humidity, dhtTemperature;
	if(!DHT22_GetHumidityAndTemperature(&humidity, &dhtTemperature))
	{
		return false;
	}

	printf("Humidity = %.2f %%, Temperature = %.2f C\r\n", humidity, dhtTemperature);

	printf("Reading insolation...\r\n");
	int insolation = TSL2581_ReadInsolation();
	printf("Insolation = %d Lux\r\n", insolation);

	printf("Reading temperature...\r\n");
	float bmpTemperature = BMP280_ReadTemperature();
	printf("Temperature = %f C\r\n", bmpTemperature);

	printf("Reading internal temperature...\r\n");
	float internalTemperature = CPUTEMP_Read();
	printf("Internal temperature = %f C\r\n", internalTemperature);

	printf("Reading pressure...\r\n");
	float pressure = BMP280_ReadPressure();
	printf("Pressure = %f hPa\r\n", pressure);

	printf("Reading air quality...\r\n");
	int airQuality = GP2_Read();
	printf("Air quality = %d ug/m3\r\n", airQuality);

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
	char buffer[512];
	sprintf(buffer, "temperature=%f&internal_temperature=%f&pressure=%f&humidity=%f&insolation=%d&air_quality=%d",
			bmpTemperature, internalTemperature, pressure, humidity, insolation, airQuality);

	if(!ESP8266_SendPOST(SERVER_IP, 80, SERVER_PATH, buffer))
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
