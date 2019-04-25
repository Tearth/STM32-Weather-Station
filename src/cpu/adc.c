#include <cpu/adc.h>

ADC_Definition AdcDefinitions[ADC_COUNT] =
{
	{
		.Id = 0,
		.Definition = ADC1,
		.Clock = RCC_AHBPeriph_ADC12,
		.ClockDivider = RCC_ADC12PLLCLK_Div2,
	},
	{
		.Id = 1,
		.Definition = ADC2,
		.Clock = RCC_AHBPeriph_ADC12,
		.ClockDivider = RCC_ADC12PLLCLK_Div2,
	},
	{
		.Id = 2,
		.Definition = ADC3,
		.Clock = RCC_AHBPeriph_ADC34,
		.ClockDivider = RCC_ADC34PLLCLK_Div2,
	},
	{
		.Id = 3,
		.Definition = ADC4,
		.Clock = RCC_AHBPeriph_ADC34,
		.ClockDivider = RCC_ADC34PLLCLK_Div2,
	},
};

bool adc_enabled[ADC_COUNT];

bool ADC_Enable(ADC_TypeDef *adcx)
{
	ADC_InitTypeDef adc;
	ADC_Definition *definition;

	if(definition = ADC_GetDefinition(adcx), definition == 0)
	{
		return false;
	}

	if(adc_enabled[definition->Id]) return false;
	adc_enabled[definition->Id] = true;

	RCC_AHBPeriphClockCmd(definition->Clock, ENABLE);
	RCC_ADCCLKConfig(definition->ClockDivider);

	ADC_StructInit(&adc);
	adc.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
	adc.ADC_ExternalTrigConvEvent = ADC_ExternalTrigEventEdge_None;
	adc.ADC_NbrOfRegChannel = 1;
	ADC_Init(adcx, &adc);

	ADC_StructInit(&adc);
	ADC_VoltageRegulatorCmd(adcx, ENABLE);
	ADC_SelectCalibrationMode(adcx, ADC_CalibrationMode_Single);
	ADC_StartCalibration(adcx);
	while(ADC_GetCalibrationStatus(adcx) != RESET);

	ADC_Cmd(adcx, ENABLE);

	return true;
}

bool ADC_Disable(ADC_TypeDef *adcx)
{
	ADC_Definition *definition;

	if(definition = ADC_GetDefinition(adcx), definition == 0)
	{
		return false;
	}

	if(adc_enabled[definition->Id]) return false;
	adc_enabled[definition->Id] = false;

	ADC_Cmd(adcx, DISABLE);
	return true;
}

int ADC_Read(ADC_TypeDef *adcx, uint8_t channel)
{
    ADC_RegularChannelConfig(adcx, channel, 1, ADC_SampleTime_61Cycles5);
    ADC_StartConversion(adcx);
    while (!ADC_GetFlagStatus(adcx, ADC_FLAG_EOC));

    return ADC_GetConversionValue(adcx);
}

ADC_Definition *ADC_GetDefinition(ADC_TypeDef *adcx)
{
	for(int i = 0; i < ADC_COUNT; i++)
	{
		if (AdcDefinitions[i].Definition == adcx)
		{
			return &AdcDefinitions[i];
		}
	}

	return 0;
}
