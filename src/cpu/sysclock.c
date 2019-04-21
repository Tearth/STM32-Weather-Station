#include <cpu/sysclock.h>

uint64_t sysclock;

bool SYSCLOCK_Enable()
{
	SysTick_Config(SystemCoreClock / 1000);
}

uint64_t SYSCLOCK_GetSystemClock()
{
	return sysclock;
}

void SysTick_Handler()
{
	sysclock++;
}
