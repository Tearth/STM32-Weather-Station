#include <drivers/gp2.h>

bool gp2_enabled;

bool GP2_Enable()
{
	GPIO_InitTypeDef gpio;

	if(gp2_enabled) return false;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	ADC_Enable(ADC1);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GP2_DATA_PIN;
	gpio.GPIO_Mode = GPIO_Mode_AN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GP2_PORT, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GP2_ILED_PIN;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GP2_PORT, &gpio);

	GPIO_ResetBits(GPIOB, GP2_ILED_PIN);

	gp2_enabled = true;
	return true;
}

bool GP2_Disable()
{
	if(!gp2_enabled) return false;
	ADC_Disable(ADC1);

	gp2_enabled = false;
	return true;
}

float GP2_Read()
{
	GPIO_SetBits(GPIOB, GP2_ILED_PIN);
	DelayMicroseconds(280);

	int value = ADC_Read(ADC1, ADC_Channel_14, ADC_SampleTime_1Cycles5);
	GPIO_ResetBits(GPIOB, GP2_ILED_PIN);

	float volts = value * 3.3f / 4096;
	float milivolts = (volts * 1000) - 500;

	return milivolts <= 0 ? 0 : milivolts * 0.2f;
}
