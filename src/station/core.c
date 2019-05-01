#include <station/core.h>

bool Core_DoMeasurementsAndSend()
{
	printf("Enabling WiFi module...\r\n");
	if(!ESP8266_Enable())
	{
		return false;
	}

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

	printf("Disabling WiFi module...\r\n");
	if(!ESP8266_Disable())
	{
		return false;
	}

	return true;
}
