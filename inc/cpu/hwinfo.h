#ifndef HWINFO_H
#define HWINFO_H

#define UUID_ADDRESS 0x1FFFF7AC
#define CPUID_ADDRESS 0xE000ED00

#include <stdlib.h>
#include <stdint.h>
#include <cpu/hwinfo.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef union CPU_UUID
{
	int Value[3];
	struct
	{
		int Coordinates;
		int Lot;
		int Wafer;
	} UUID_Details;
} CPU_UUID;

typedef union CPUID
{
	int Value;
	struct
	{
		unsigned int Implementer : 8;
		unsigned int Variant : 4;
		unsigned int Constant : 4;
		unsigned int PartNo : 12;
		unsigned int Revision : 4;
	} ID_Details;
} CPUID;

#ifdef __cplusplus
 extern "C" {
#endif

void HWINFO_GetUUID(CPU_UUID *cpuUuid);
void HWINFO_GetCPUID(CPUID *cpuId);

#ifdef __cplusplus
}
#endif

#endif
