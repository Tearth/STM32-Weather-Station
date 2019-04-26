#ifndef ESP8266_H
#define ESP8266_H

#define ESP8266_USART_INTERFACE USART1
#define ESP8266_USART_SPEED 9600

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stm32f30x.h>
#include <cpu/usart.h>

typedef struct ESP8266_FirmwareInfo
{
	char SDKVersionInfo[64];
	char ATVersionInfo[64];
	char CompileTime[64];
} ESP8266_FirmwareInfo;

bool ESP8266_Enable();
int ESP8266_SendCommand(const char *str);
int ESP8266_ReceiveData(char *buf);
bool ESP8266_WaitForOK();
bool ESP8266_Reset();
bool ESP8266_SetEcho(bool enabled);
bool ESP8266_GetFirmware(ESP8266_FirmwareInfo* firmwareInfo);

#endif
