#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32f1xx.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"
#include "display.h"

void Enable_Clocks(void);
void GPIO_Config(void);
void ADC1_Init(void);
void TIM1_Init(void);

extern int count;

#endif