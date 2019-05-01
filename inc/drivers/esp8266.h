#ifndef ESP8266_H
#define ESP8266_H

#define ESP8266_USART_INTERFACE USART1
#define ESP8266_USART_SPEED 9600

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stm32f30x.h>
#include <cpu/usart.h>
#include <library/delay.h>

typedef struct ESP8266_FirmwareInfo
{
	char SDKVersionInfo[64];
	char ATVersionInfo[64];
	char CompileTime[64];
} ESP8266_FirmwareInfo;

typedef enum ESP8266_Mode
{
	ESP8266_Mode_Client = 1,
	ESP8266_Mode_AccessPoint = 2,
	ESP8266_Mode_ClientAndAccessPoint = 3
} ESP8266_Mode;

bool ESP8266_Enable();
int ESP8266_SendCommand(const char *str);
int ESP8266_ReceiveData(char *buf);
bool ESP8266_WaitForAck();

bool ESP8266_IsConnected();
bool ESP8266_Reset();
bool ESP8266_SetEcho(bool enabled);
bool ESP8266_GetFirmware(ESP8266_FirmwareInfo* firmwareInfo);
bool ESP8266_SetMode(ESP8266_Mode mode);
bool ESP8266_Connect(const char *ssid, const char *password);
bool ESP8266_Disconnect();
bool ESP8266_GetIPAndMAC(char *ip, char *mac);
int ESP8266_SendPing(char *target);
bool ESP8266_SendGET(char *host, int port, char *target, char *responseBuf);
bool ESP8266_OpenTCPConnection(char *host, int port);
bool ESP8266_CloseTCPConnection();

#endif
