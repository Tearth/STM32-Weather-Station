#include <cpu/cputemp.h>

bool cputemp_enabled = false;

bool CPUTEMP_Enable()
{
	if(cputemp_enabled) return false;
	cputemp_enabled = true;

	ADC_Enable(ADC1);
	ADC_Cmd(ADC1, ENABLE);
	ADC_TempSensorCmd(ADC1, ENABLE);

	return true;
}

bool CPUTEMP_Disable()
{
	if(!cputemp_enabled) return false;
	cputemp_enabled = false;

	ADC_Cmd(ADC1, DISABLE);
	ADC_TempSensorCmd(ADC1, DISABLE);

	return true;
}

float CPUTEMP_Read()
{
	int total = 0;
	for(int i=0; i<CPUTEMP_MEASUREMENTS_COUNT; i++)
	{
		total += ADC_Read(ADC1, ADC_Channel_16, ADC_SampleTime_181Cycles5);
		Delay(2);
	}

	uint16_t ts30Val = *(uint16_t *)TS30;
	uint16_t ts110Val = *(uint16_t *)TS110;

	float temperature = total / CPUTEMP_MEASUREMENTS_COUNT - ts30Val;
	temperature *= 110 - 30;
	temperature /= ts110Val - ts30Val;
	temperature += 30;

	return temperature;
}
