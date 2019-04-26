#include <drivers/esp8266.h>

bool ESP8266_Enable()
{
	if(!USART_Enable(ESP8266_USART_INTERFACE, ESP8266_USART_SPEED))
	{
		return false;
	}

	ESP8266_Reset();

	Delay(500);
	return ESP8266_SetEcho(false) && ESP8266_IsConnected();
}

int ESP8266_SendCommand(const char *str)
{
	char buffer[32];
	sprintf(buffer, "%s\r\n", str);

	return USART_SendString(ESP8266_USART_INTERFACE, buffer);
}

int ESP8266_ReceiveData(char *buf)
{
	int length;
	do
	{
		length = USART_ReceiveString(ESP8266_USART_INTERFACE, buf);
	}
	while(memcmp("\r\n", buf, 2) == 0);
	return length;
}

bool ESP8266_WaitForOK()
{
	char buffer[128];
	if(!ESP8266_ReceiveData(buffer) == 4 && memcmp("OK", buffer, 2) == 0)
	{
		printf(buffer);
		return false;
	}
	return true;
}

bool ESP8266_IsConnected()
{
	ESP8266_SendCommand("AT");
	return ESP8266_WaitForOK();
}

bool ESP8266_Reset()
{
	ESP8266_SendCommand("AT+RST");
	return ESP8266_WaitForOK();
}

bool ESP8266_SetEcho(bool enabled)
{
	ESP8266_SendCommand(enabled ? "ATE1" : "ATE0");
	return ESP8266_WaitForOK();
}

bool ESP8266_GetFirmware(ESP8266_FirmwareInfo* firmwareInfo)
{
	ESP8266_SendCommand("AT+GMR");

	ESP8266_ReceiveData(firmwareInfo->ATVersionInfo);
	ESP8266_ReceiveData(firmwareInfo->SDKVersionInfo);
	ESP8266_ReceiveData(firmwareInfo->CompileTime);

	return ESP8266_WaitForOK();
}

bool ESP8266_SetMode(ESP8266_Mode mode)
{
	char buf[16];
	sprintf(buf, "AT+CWMODE=%d", mode);

	ESP8266_SendCommand(buf);
	return ESP8266_WaitForOK();
}

bool ESP8266_Connect(const char *ssid, const char *password)
{
	char buf[64];
	sprintf(buf, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);

	ESP8266_SendCommand(buf);
	while(!ESP8266_WaitForOK());

	return true;
}

bool ESP8266_Disconnect()
{
	ESP8266_SendCommand("AT+CWQAP");
	return ESP8266_WaitForOK();
}
