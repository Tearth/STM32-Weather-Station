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

int GP2_Read()
{
	int total = 0;
	for(int i=0; i<GP2_MEASUREMENTS_COUNT; i++)
	{
		GPIO_SetBits(GPIOB, GP2_ILED_PIN);
		DelayMicroseconds(280);

		total += ADC_Read(ADC1, ADC_Channel_14, ADC_SampleTime_61Cycles5);
		GPIO_ResetBits(GPIOB, GP2_ILED_PIN);

		Delay(2);
	}

	float volts = (total / GP2_MEASUREMENTS_COUNT) * 3.3f / 4096;
	float milivolts = (volts * 1000) - GP2_NO_DUST_MILIVOLTS;

	return milivolts <= 0 ? 0 : milivolts * GP2_DUST_VOLT_RATIO;
}
