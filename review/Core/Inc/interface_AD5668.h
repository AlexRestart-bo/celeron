#ifndef __INTERFACE_AD5668_H__
#define __INTERFACE_AD5668_H__

#include "stm32l4xx_hal.h"
#include <stdbool.h>
#include "AD5668.h"
#include "var.h"



void AD5668_Init(void);
void AD5668_SetValue(uint32_t chanel, int value);
void AD5668_WriteValue(uint32_t chanel, int value);
void AD5668_Reset(void);
	
#endif
