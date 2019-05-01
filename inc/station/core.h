#ifndef CORE_H
#define CORE_H

#include <stdbool.h>
#include <config.h>
#include <cpu/usart.h>
#include <cpu/hwinfo.h>
#include <cpu/adc.h>
#include <cpu/cputemp.h>
#include <cpu/watchdog.h>
#include <cpu/timer.h>
#include <library/delay.h>
#include <drivers/esp8266.h>

bool Core_DoMeasurementsAndSend();

#endif
