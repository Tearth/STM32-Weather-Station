#include <library/delay.h>

void Delay(unsigned int milliseconds)
{
	uint64_t current_sysclock = SYSCLOCK_GetSystemClock();
	while(SYSCLOCK_GetSystemClock() < current_sysclock + milliseconds);
}
