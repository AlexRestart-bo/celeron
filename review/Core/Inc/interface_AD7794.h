#ifndef __INTERFACE_AD7794_H__
#define __INTERFACE_AD7794_H__

#include "stm32l4xx_hal.h"
#include <stdbool.h>
#include "AD7794.h"
#include "var.h"

void AD7794_UPR_Reset(void);
void AD7794_UPR_SendData(uint8_t* sendData, uint8_t count);
void AD7794_UPR_ReadData(uint8_t reg, uint8_t* pData, uint8_t count);
void AD7794upr_Setup_IO(void);
void AD7794upr_Setup_VCSEL_TEMP(void);
void AD7794upr_Setup_AUX1_TEMP(void);
void AD7794upr_Setup_ADC_TEMP(void);
int AD7794upr_GetValue(void);
bool AD7794upr_GetFlagCompleted(void);

void AD7794_MDL_Reset(void);
void AD7794_MDL_SendData(uint8_t* sendData, uint8_t count);
void AD7794_MDL_ReadData(uint8_t reg, uint8_t* pData, uint8_t count);
void AD7794mdl_Setup_IO(void);
void AD7794mdl_Setup_CELL_TEMP(void);
void AD7794mdl_Setup_AUX2_TEMP(void);
void AD7794mdl_Setup_ADC_TEMP(void);
int AD7794mdl_GetValue(void);
bool AD7794mdl_GetFlagCompleted(void);

void AD7794_Init(void);

float ConvertToTempNTC_VCSEL(int value, float resist);
float ConvertToTempNTC_CELL(int value, float resist);
float ConvertToTempNTC_AUX1(int value, float resist);
float ConvertToTempNTC_AUX2(int value, float resist);
float ConvertToTempADC(int value);

#endif
