#ifndef __INTERFACE_LMX2486_H__
#define __INTERFACE_LMX2486_H__

#include "stm32l4xx_hal.h"
#include "LMX2486.h"
#include "var.h"

void LMX2486_Init(void);
void LMX2486_SendData(uint32_t sendData);
void LMX2486_SetFreq(uint32_t valueN, uint32_t valueFN, uint32_t valueFD);
void CamputeOptionLMX(double real_value, double valueN, int* valueFN, int* valueFD);
void CamputeOptionSetValueLMX(int value);
void CamputeOptionAddValueLMX(int value);

#endif
