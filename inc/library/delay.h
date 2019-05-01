#ifndef DELAY_H
#define DELAY_H

#include <cpu/sysclock.h>

#define DelayMicroseconds(microseconds) do {\
	asm volatile (\
		"MOV R0,%[loops]\n\t"\
		"1: \n\t"\
		"SUB R0, #1\n\t"\
		"CMP R0, #0\n\t"\
		"BNE 1b \n\t" : : [loops] "r" (8*microseconds) : "memory"\
		);\
} while(0)

#ifdef __cplusplus
 extern "C" {
#endif

void Delay(unsigned int milliseconds);

#ifdef __cplusplus
}
#endif

#endif
