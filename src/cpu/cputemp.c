#include <cpu/cputemp.h>

void CPUTEMP_Enable()
{
	ADC_Enable(ADC1);
	ADC_Cmd(ADC1, ENABLE);
	ADC_TempSensorCmd(ADC1, ENABLE);
}

void CPUTEMP_Disable()
{
	ADC_Cmd(ADC1, DISABLE);
	ADC_TempSensorCmd(ADC1, DISABLE);
}

float CPUTEMP_Read()
{
	int measure = ADC_Read(ADC1, ADC_Channel_16);

	uint16_t ts30Val = *(uint16_t *)TS30;
	uint16_t ts110Val = *(uint16_t *)TS110;

	float temperature = measure - ts30Val;
	temperature *= 110 - 30;
	temperature /= ts110Val - ts30Val;
	temperature += 30;

	return temperature;
}
