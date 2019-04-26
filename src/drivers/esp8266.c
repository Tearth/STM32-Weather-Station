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
	while(memcmp("\r\n", buf, 2) == 0 || memcmp("busy", buf, 4) == 0);
	return length;
}

bool ESP8266_WaitForAck()
{
	char buffer[128];
	while(1)
	{
		ESP8266_ReceiveData(buffer);

		if(memcmp("OK\r\n", buffer, 4) == 0)
		{
			return true;
		}
		else if(memcmp("ERROR\r\n", buffer, 7) == 0 || memcmp("FAIL\r\n", buffer, 6) == 0)
		{
			return false;
		}
	}
}

bool ESP8266_IsConnected()
{
	ESP8266_SendCommand("AT");
	return ESP8266_WaitForAck();
}

bool ESP8266_Reset()
{
	ESP8266_SendCommand("AT+RST");
	return ESP8266_WaitForAck();
}

bool ESP8266_SetEcho(bool enabled)
{
	ESP8266_SendCommand(enabled ? "ATE1" : "ATE0");
	return ESP8266_WaitForAck();
}

bool ESP8266_GetFirmware(ESP8266_FirmwareInfo* firmwareInfo)
{
	ESP8266_SendCommand("AT+GMR");

	ESP8266_ReceiveData(firmwareInfo->ATVersionInfo);
	ESP8266_ReceiveData(firmwareInfo->SDKVersionInfo);
	ESP8266_ReceiveData(firmwareInfo->CompileTime);

	return ESP8266_WaitForAck();
}

bool ESP8266_SetMode(ESP8266_Mode mode)
{
	char buf[16];
	sprintf(buf, "AT+CWMODE=%d", mode);

	ESP8266_SendCommand(buf);
	return ESP8266_WaitForAck();
}

bool ESP8266_Connect(const char *ssid, const char *password)
{
	char buf[64];
	sprintf(buf, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);

	ESP8266_SendCommand(buf);
	return ESP8266_WaitForAck();
}

bool ESP8266_Disconnect()
{
	ESP8266_SendCommand("AT+CWQAP");
	return ESP8266_WaitForAck();
}

bool ESP8266_GetIPAndMAC(char *ip, char *mac)
{
	char line_ip[64], line_mac[64];

	ESP8266_SendCommand("AT+CIFSR");
	ESP8266_ReceiveData(line_ip);
	ESP8266_ReceiveData(line_mac);
	bool ack = ESP8266_WaitForAck();

	sscanf(line_ip, "+CIFSR:STAIP,\"%[^\"]\"\r\n", ip);
	sscanf(line_mac, "+CIFSR:STAMAC,\"%[^\"]\"\r\n", mac);

	return ack;
}

int ESP8266_SendPing(char *ip)
{
	char buf[64];
	int ping;

	sprintf(buf, "AT+PING=\"%s\"", ip);

	ESP8266_SendCommand(buf);
	ESP8266_ReceiveData(buf);
	bool ack = ESP8266_WaitForAck();

	if(!ack)
	{
		return -1;
	}

	sscanf(buf, "%d\r\n", &ping);
	return ping;
}
