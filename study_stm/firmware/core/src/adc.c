#include "main.h"

void ADC1_Init(void){

    // Включение тактирования
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;
    
    // Настройка PA0
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    
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
}

uint16_t ADC_Read(void) {
    // Ожидание окончания преобразования
    while(!(ADC1->SR & ADC_SR_EOC));
    
    // Чтение результата
    return ADC1->DR;
}