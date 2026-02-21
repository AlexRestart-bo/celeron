#include "main.h"

void ADC1_Init(void){
    // Калибровка ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    delay_ms(1);
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while(ADC1->CR2 & ADC_CR2_RSTCAL);
    ADC1->CR2 |= ADC_CR2_CAL;
    while(ADC1->CR2 & ADC_CR2_CAL);
    
    // Конфигурация
    ADC1->CR1 = 0;
    ADC1->CR2 = ADC_CR2_ADON | ADC_CR2_CONT;  // Непрерывный режим
    ADC1->SQR1 = 0;
    ADC1->SQR3 = 0;
    ADC1->SMPR2 = ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_2; // 239.5 циклов
    
    // Запуск непрерывного преобразования
    ADC1->CR2 |= ADC_CR2_ADON;

    // Вклюсение прерывания ADC1/ADC2 global interrupt
    NVIC_EnableIRQ(ADC1_2_IRQn);
    NVIC_SetPriority(ADC1_2_IRQn, 0);
}

uint16_t ADC_Read(void) {
    // Ожидание окончания преобразования
    while(!(ADC1->SR & ADC_SR_EOC));
    
    // Чтение результата
    return ADC1->DR;
}