#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32f1xx.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"
#include "display.h"
#include "config.h"
#include "pid.h"

void Enable_Clocks(void);
void ADC1_Init(void);
void TIM1_Init(void);
void TIM2_Init(void);
void TIM3_Init(void);
void GPIO_Init(void);
void ADC1_Init(void);

enum {
    NOTHING = 0,    // не включено никаких режимов работы: ПИД-регулятор тянет температуру к целевой (target_temp), на дисплее отображается текущее значение температуры
    DOUBLE = 1,     // одновременное нажатие и удержание двух кнопок, на дисплее отображается мощность нагрева (в относительных единицах)
    SINGLER = 2,    // нажатие с удержанием правой кнопки, на дисплее отображается целевая температура, которая растет на 2 градуса за секунду
    SINGLEL = 3     // нажатие с удержанием левой кнопки, на дисплее отображается целевая температура, которая убывает на 2 градуса за секунду
};

extern uint8_t mode;
extern uint8_t flag_interrupt_tim2;

void delay_ms(uint32_t ms);
void execute_orders(void);
uint32_t get_borehole_value(void);

#endif