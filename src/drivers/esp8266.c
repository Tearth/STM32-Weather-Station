#include <drivers/esp8266.h>

bool ESP8266_Enable()
{
	if(!USART_Enable(ESP8266_USART_INTERFACE, ESP8266_USART_SPEED))
	{
		return false;
	}

	if(!ESP8266_SendCommandAndWaitForOK("ATE0"))
	{
		return false;
	}

	if(!ESP8266_SendCommandAndWaitForOK("AT+RST"))
	{
		return false;
	}

	return true;
}

int ESP8266_SendCommand(const char *str)
{
	char buffer[128];
	sprintf(buffer, "%s\r\n", str);

	return USART_SendString(ESP8266_USART_INTERFACE, buffer);
}

bool ESP8266_SendCommandAndWaitForOK(const char *str)
{
	char buffer[128];
	int length;

	ESP8266_SendCommand(str);
	length = ESP8266_ReceiveData(buffer);

	return memcmp("OK", buffer, length - 2) == 0;
}

int ESP8266_ReceiveData(char *buf)
{
	USART_ReceiveString(ESP8266_USART_INTERFACE, buf);
	return USART_ReceiveString(ESP8266_USART_INTERFACE, buf);
}

int ESP8266_GetFirmware(char *buf)
{
	ESP8266_SendCommand("AT+GMR");
	return ESP8266_ReceiveData(buf);
}
