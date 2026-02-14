#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32f1xx.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

void Enable_Clocks(void);
void USART1_Init(void);
void USART1_SendChar(uint8_t data);
uint8_t USART1_ReceiveChar(void);
void GPIO_Config(void);
void USART1_SendString(uint8_t *str, uint8_t size);
void delay_ms(int sec);
void ADC1_Init(void);
uint16_t ADC_Read(void);
void TIM1_Init(void);

typedef struct {
    uint16_t buffer[1000];
    int counter_data;
    bool readiness;
} Oscillation;

Oscillation first_fluct;

extern int count;

#endif