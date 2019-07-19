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
#include <drivers/dht22.h>
#include <drivers/tsl2581.h>
#include <drivers/bmp280.h>
#include <drivers/gp2.h>

bool Core_DoMeasurementsAndSend();
bool Core_EnableModules();
bool Core_DisableModules();

#endif
