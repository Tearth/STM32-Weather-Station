#include <cpu/hwinfo.h>

void HWINFO_GetUUID(CPU_UUID *cpuUuid)
{
	memcpy(cpuUuid, (void *)UUID_ADDRESS, sizeof(int) * 3);
}

void HWINFO_GetCPUID(CPUID *cpuId)
{
	memcpy(cpuId, (void *)CPUID_ADDRESS, sizeof(int));
}
