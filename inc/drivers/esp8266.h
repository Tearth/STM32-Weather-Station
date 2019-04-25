#ifndef ESP8266_H
#define ESP8266_H

#define ESP8266_USART_INTERFACE USART1
#define ESP8266_USART_SPEED 115200

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stm32f30x.h>
#include <cpu/usart.h>

bool ESP8266_Enable();
int ESP8266_SendCommand(const char *str);
bool ESP8266_SendCommandAndWaitForOK(const char *str);
int ESP8266_ReceiveData(char *buf);

#endif
