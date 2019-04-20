#ifndef HWINFO_H
#define HWINFO_H

#define UUID_ADDRESS 0x1FFFF7AC

#include <stdint.h>
#include <cpu/hwinfo.h>

typedef struct CPU_UUID
{
	uint32_t Coordinates;
	uint32_t Lot;
	uint32_t Wafer;
} CPU_UUID;

void HWINFO_GetUUID(CPU_UUID *cpuUuid);

#endif
