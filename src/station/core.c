#include <station/core.h>

bool Core_DoMeasurementsAndSend()
{
	uint64_t uptime = SYSCLOCK_GetSystemClock();
	if(!Core_EnableModules())
	{
		printf("Error when trying to enable modules\r\n");
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
	sprintf(buffer, "temperature=%f&internal_temperature=%f&pressure=%f&humidity=%f&insolation=%d&air_quality=%d&uptime=%lld",
			bmpTemperature, internalTemperature, pressure, humidity, insolation, airQuality, uptime);

	if(!ESP8266_SendPOST(SERVER_IP, 80, SERVER_PATH, buffer))
	{
		return false;
	}

	if(!Core_DisableModules())
	{
		printf("Error when trying to disable modules\r\n");
	}

	return true;
}

bool Core_EnableModules()
{
	printf("Enabling I2C...\r\n");
	if(!I2C_Enable(I2C1))
	{
		return false;
	}

	printf("Enabling ESP8266...\r\n");
	if(!ESP8266_Enable())
	{
		return false;
	}

	printf("Enabling DHT22...\r\n");
	if(!DHT22_Enable())
	{
		return false;
	}

	printf("Enabling TSL2581...\r\n");
	if(!TSL2581_Enable())
	{
		return false;
	}

	printf("Enabling BMP280...\r\n");
	if(!BMP280_Enable())
	{
		return false;
	}

	printf("Enabling GP2...\r\n");
	if(!GP2_Enable())
	{
		return false;
	}

	return true;
}

bool Core_DisableModules()
{
	printf("Disabling ESP8266...\r\n");
	if(!ESP8266_Disable())
	{
		return false;
	}

	printf("Disabling DHT22...\r\n");
	if(!DHT22_Disable())
	{
		return false;
	}

	printf("Disabling TSL2581...\r\n");
	if(!TSL2581_Disable())
	{
		return false;
	}

	printf("Disabling BMP280...\r\n");
	if(!BMP280_Disable())
	{
		return false;
	}

	printf("Disabling GP2...\r\n");
	if(!GP2_Disable())
	{
		return false;
	}

	printf("Disabling I2C...\r\n");
	if(!I2C_Disable(I2C1))
	{
		return false;
	}

	return true;
}
