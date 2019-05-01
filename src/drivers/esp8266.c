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
	char buffer[128];
	sprintf(buffer, "%s\r\n", str);

	return USART_SendString(ESP8266_USART_INTERFACE, buffer);
}

int ESP8266_ReceiveLine(char *buf)
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
		ESP8266_ReceiveLine(buffer);
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

	ESP8266_ReceiveLine(firmwareInfo->ATVersionInfo);
	ESP8266_ReceiveLine(firmwareInfo->SDKVersionInfo);
	ESP8266_ReceiveLine(firmwareInfo->CompileTime);

	return ESP8266_WaitForAck();
}

bool ESP8266_SetMode(ESP8266_Mode mode)
{
	char buf[128];
	sprintf(buf, "AT+CWMODE=%d", mode);

	ESP8266_SendCommand(buf);
	return ESP8266_WaitForAck();
}

bool ESP8266_Connect(const char *ssid, const char *password)
{
	char buf[128];
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
	char line_ip[128], line_mac[128];

	ESP8266_SendCommand("AT+CIFSR");
	ESP8266_ReceiveLine(line_ip);
	ESP8266_ReceiveLine(line_mac);

	sscanf(line_ip, "+CIFSR:STAIP,\"%[^\"]\"\r\n", ip);
	sscanf(line_mac, "+CIFSR:STAMAC,\"%[^\"]\"\r\n", mac);

	return ESP8266_WaitForAck();
}

int ESP8266_SendPing(char *target)
{
	char buf[128];
	int ping;

	sprintf(buf, "AT+PING=\"%s\"", target);

	ESP8266_SendCommand(buf);
	ESP8266_ReceiveLine(buf);

	if(!ESP8266_WaitForAck())
	{
		return -1;
	}

	sscanf(buf, "%d\r\n", &ping);
	return ping;
}

bool ESP8266_SendGET(char *host, int port, char *path)
{
	if(!ESP8266_OpenTCPConnection(host, port)) return false;

	char request_buffer[1024];
	sprintf(request_buffer,
			"GET %s HTTP/1.1\r\n"
			"User-Agent: STM32 Weather Station\r\n"
			"Host: %s\r\n"
			"Connection: Close\r\n\r\n",
			path, host);

	char command_buffer[128];
	sprintf(command_buffer, "AT+CIPSEND=%d", strlen(request_buffer));

	ESP8266_SendCommand(command_buffer);
	ESP8266_WaitForAck();

	USART_SendString(ESP8266_USART_INTERFACE, request_buffer);
	return ESP8266_WaitForTCPClose();
}

bool ESP8266_OpenTCPConnection(char *host, int port)
{
	char buffer[128];
	sprintf(buffer, "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);

	ESP8266_SendCommand(buffer);
	return ESP8266_WaitForAck();
}

bool ESP8266_CloseTCPConnection()
{
	ESP8266_SendCommand("AT+CIPCLOSE");
	return ESP8266_WaitForAck();
}

bool ESP8266_WaitForTCPClose()
{
	char buffer[8192];

	while(1)
	{
		ESP8266_ReceiveLine(buffer);
		printf(buffer);
		if(strstr(buffer, "CLOSED\r\n") != NULL)
		{
			break;
		}
	}

	return true;
}
