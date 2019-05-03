#ifndef TSL2581_H
#define TSL2581_H

#include <cpu/i2c.h>

#define TSL2586_I2C_INTERFACE I2C1
#define TSL2586_I2C_ADDRESS 0x39

#define TSL2581_COMMAND 0x80
#define TSL2581_COMMAND_CONTROL 0x00
#define TSL2581_COMMAND_TIMING 0x01
#define TSL2581_COMMAND_INTERRUPT 0x02
#define TSL2581_COMMAND_THLLOW_LOW_INT 0x03
#define TSL2581_COMMAND_THLHIGH_LOW_INT 0x04
#define TSL2581_COMMAND_THLLOW_HIGH_INT 0x05
#define TSL2581_COMMAND_THLHIGH_HIGH_INT 0x06
#define TSL2581_COMMAND_ANALOG 0x07
#define TSL2581_COMMAND_ID 0x12
#define TSL2581_COMMAND_CONSTANT 0x13
#define TSL2581_COMMAND_DATA0LOW 0x14
#define TSL2581_COMMAND_DATA0HIGH 0x15
#define TSL2581_COMMAND_DATA1LOW 0x16
#define TSL2581_COMMAND_DATA1HIGH 0x17
#define TSL2581_COMMAND_TIMERLOW 0x18
#define TSL2581_COMMAND_TIMERHIGH 0x19

#define TSL2581_ITIME_2_7 0xFF
#define TSL2581_ITIME_5_4 0xFE
#define TSL2581_ITIME_51_3 0xED
#define TSL2581_ITIME_99_9 0xDB
#define TSL2581_ITIME_199_8 0xB6
#define TSL2581_ITIME_399_6 0x6C
#define TSL2581_ITIME_688_5 0x01

#define TSL2581_GAIN_1 0x00
#define TSL2581_GAIN_8 0x01
#define TSL2581_GAIN_16 0x02
#define TSL2581_GAIN_111 0x03

#define LUX_SCALE 16
#define RATIO_SCALE 9
#define CH_SCALE 16
#define K1C 0x009A
#define B1C 0x2148
#define M1C 0x3d71
#define K2C 0x00c3
#define B2C 0x2a37
#define M2C 0x5b30
#define K3C 0x00e6
#define B3C 0x18ef
#define M3C 0x2db9
#define K4C 0x0114
#define B4C 0x0fdf
#define M4C 0x199a
#define K5C 0x0114
#define B5C 0x0000
#define M5C 0x0000

bool TSL2581_Enable();
bool TSL2581_Disable();
int TSL2581_ReadInsolation();
void TSL2581_WaitForResults();
int TSL2581_CalculateLux(int ch0, int ch1);

void TSL2581_WriteRegisterValue(uint8_t registerAddress, uint8_t registerValue);
uint8_t TSL2581_ReadRegisterValue(uint8_t registerAddress);

#endif
