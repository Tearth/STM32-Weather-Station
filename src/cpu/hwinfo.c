#include <cpu/hwinfo.h>

void HWINFO_GetUUID(CPU_UUID *cpuUuid)
{
	uint32_t *uuid = UUID_ADDRESS;

	cpuUuid->Coordinates = uuid[0];
	cpuUuid->Lot = uuid[1];
	cpuUuid->Wafer = uuid[2];
}
