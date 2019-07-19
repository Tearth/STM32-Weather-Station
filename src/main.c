#include <stdio.h>
#include <cpu/usart.h>
#include <cpu/hwinfo.h>
#include <cpu/adc.h>
#include <cpu/cputemp.h>
#include <cpu/watchdog.h>
#include <cpu/timer.h>
#include <station/core.h>

// Ticks * 10 s
#define TICKS_BEFORE_MEASUREMENT 6 * 5

volatile bool MeasurementFlag = false;
volatile int TimerTicks;

void TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
    	TimerTicks++;

    	if(TimerTicks == TICKS_BEFORE_MEASUREMENT)
    	{
    		MeasurementFlag = true;
        	TimerTicks = 0;
    	}

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

int main(void)
{
	CPUID cpuid;
	CPU_UUID uuid;

	USART_Enable(USART2, 115200);

	HWINFO_GetCPUID(&cpuid);
	HWINFO_GetUUID(&uuid);

	WATCHDOG_Enable(IWDG_Prescaler_256);
	SYSCLOCK_Enable();

	CPUTEMP_Enable();

	TimerTicks = TICKS_BEFORE_MEASUREMENT - 1;
	TIMER_Enable(TIM3, 10000);

	printf("STM32 NUCLEO-F303RE (UUID: 0x%X%X%X)\r\n", uuid.Value[0], uuid.Value[1], uuid.Value[2]);

	while(1)
	{
		__WFI();

		if(MeasurementFlag)
		{
			MeasurementFlag = false;

			printf("\r\n");
			printf("Measurement START [%lld]\r\n", SYSCLOCK_GetSystemClock());

			if(!Core_DoMeasurementsAndSend())
			{
				printf("ERROR\r\n");
				while(1);
			}

			printf("Measurement STOP [%lld]\r\n", SYSCLOCK_GetSystemClock());
			printf("\r\n");
		}

		WATCHDOG_Reset();
	}
}
