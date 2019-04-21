#include <cpu/watchdog.h>

void WATCHDOG_Enable(uint8_t prescaler)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prescaler);
	IWDG_SetReload(0xFFF);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void WATCHDOG_Disable()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
	IWDG_Disable();
}

void WATCHDOG_Reset()
{
	IWDG_ReloadCounter();
}
