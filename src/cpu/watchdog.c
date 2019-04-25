#include <cpu/watchdog.h>

bool watchdog_enabled;

bool WATCHDOG_Enable(uint8_t prescaler)
{
	if(watchdog_enabled) return false;
	watchdog_enabled = true;

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prescaler);
	IWDG_SetReload(0xFFF);
	IWDG_ReloadCounter();
	IWDG_Enable();

	return true;
}

void WATCHDOG_Reset()
{
	IWDG_ReloadCounter();
}
