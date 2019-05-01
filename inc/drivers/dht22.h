#ifndef DHT22_H
#define DHT22_H

#include <stm32f30x.h>
#include <stdbool.h>
#include <library/delay.h>
#include <library/types.h>

#define DHT22_DATA_PIN GPIO_Pin_5
#define DHT22_DATA_PORT GPIOC

bool DHT22_Enable();
bool DHT22_Disable();
void DHT22_PrepareDataPinToRead();
void DHT22_PrepareDataPinToWrite();
uint8_t DHT22_ReadByte();

bool DHT22_GetHumidityAndTemperature(float *humidity, float *temperature);

#endif
