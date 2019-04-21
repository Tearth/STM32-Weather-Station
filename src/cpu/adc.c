#include <cpu/adc.h>

ADC_Definition AdcDefinitions[ADC_COUNT] =
{
	{
		.Definition = ADC1,
		.Clock = RCC_AHBPeriph_ADC12,
		.ClockDivider = RCC_ADC12PLLCLK_Div2,
	},
	{
		.Definition = ADC2,
		.Clock = RCC_AHBPeriph_ADC12,
		.ClockDivider = RCC_ADC12PLLCLK_Div2,
	},
	{
		.Definition = ADC3,
		.Clock = RCC_AHBPeriph_ADC34,
		.ClockDivider = RCC_ADC34PLLCLK_Div2,
	},
	{
		.Definition = ADC4,
		.Clock = RCC_AHBPeriph_ADC34,
		.ClockDivider = RCC_ADC34PLLCLK_Div2,
	},
};

bool ADC_Enable(ADC_TypeDef *adcx)
{
	ADC_InitTypeDef adc;
	ADC_Definition *definition;

	if(definition = ADC_GetDefinition(adcx), definition == 0)
	{
		return false;
	}

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);

	ADC_StructInit(&adc);
	adc.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
	adc.ADC_ExternalTrigConvEvent = ADC_ExternalTrigEventEdge_None;
	adc.ADC_NbrOfRegChannel = 1;
	ADC_Init(adcx, &adc);

	ADC_StructInit(&adc);
	ADC_VoltageRegulatorCmd(adcx, ENABLE);
	ADC_SelectCalibrationMode(adcx, ADC_CalibrationMode_Single);
	ADC_StartCalibration(adcx);
	while(ADC_GetCalibrationStatus(adcx) != RESET );

	ADC_Cmd(adcx, ENABLE);

	return true;
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
