#include <drivers/dht22.h>

bool dht22_enabled;

bool DHT22_Enable()
{
	if(dht22_enabled) return false;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	DHT22_PrepareDataPinToWrite();

	GPIO_SetBits(DHT22_DATA_PORT, DHT22_DATA_PIN);

	dht22_enabled = true;
	return true;
}

bool DHT22_Disable()
{
	if(!dht22_enabled) return false;

	dht22_enabled = false;
	return true;
}

void DHT22_PrepareDataPinToRead()
{
	GPIO_InitTypeDef gpio;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = DHT22_DATA_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(DHT22_DATA_PORT, &gpio);
}

void DHT22_PrepareDataPinToWrite()
{
	GPIO_InitTypeDef gpio;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = DHT22_DATA_PIN;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(DHT22_DATA_PORT, &gpio);
}

uint8_t DHT22_ReadByte()
{
	uint8_t result = 0;
	for(int i=7; i>=0; i--)
	{
		while(GPIO_ReadInputDataBit(DHT22_DATA_PORT, DHT22_DATA_PIN) == LOW);
		DelayMicroseconds(50);

		result |= (GPIO_ReadInputDataBit(DHT22_DATA_PORT, DHT22_DATA_PIN) << i);
		while(GPIO_ReadInputDataBit(DHT22_DATA_PORT, DHT22_DATA_PIN) == HIGH);
	}

	return result;
}

bool DHT22_GetHumidityAndTemperature(float *humidity, float *temperature)
{
	GPIO_ResetBits(DHT22_DATA_PORT, DHT22_DATA_PIN);
	DelayMicroseconds(1000);

	GPIO_SetBits(DHT22_DATA_PORT, DHT22_DATA_PIN);
	DelayMicroseconds(40);

	DHT22_PrepareDataPinToRead();

	while(GPIO_ReadInputDataBit(DHT22_DATA_PORT, DHT22_DATA_PIN) == HIGH);
	DelayMicroseconds(80);

	while(GPIO_ReadInputDataBit(DHT22_DATA_PORT, DHT22_DATA_PIN) == LOW);
	DelayMicroseconds(80);

	int integralHumidity = DHT22_ReadByte();
	int decimalHumidity = DHT22_ReadByte();
	int integralTemperature = DHT22_ReadByte();
	int decimalTemperature = DHT22_ReadByte();
	int checksum = DHT22_ReadByte();

	if((uint8_t)(integralHumidity + decimalHumidity + integralTemperature + decimalTemperature) != checksum)
	{
		return false;
	}

	*humidity = ((integralHumidity << 8) + decimalHumidity) / 10.0f;
	*temperature = ((integralTemperature << 8) + decimalTemperature) / 10.0f;

	DHT22_PrepareDataPinToWrite();
	GPIO_SetBits(DHT22_DATA_PORT, DHT22_DATA_PIN);

	return true;
}
