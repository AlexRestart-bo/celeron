#include "main.h"

void TIM1_Init(void){
    // таймер вызывает обработчик события (переполнение) раз в 20 мс
    TIM1->SMCR &= ~TIM_SMCR_SMS;
    TIM1->PSC = 799;
    TIM1->ARR = 199;
    TIM1->DIER |= TIM_DIER_UIE;
    TIM1->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_SetPriority(TIM1_UP_IRQn, 0);
    TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM2_Init(void){
    // таймер вызывает обработчик события (переполнение) раз в 100 мс
    TIM2->SMCR &= ~TIM_SMCR_SMS;
    TIM2->PSC = 799;
    TIM2->ARR = 999;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM3_Init(void){
    TIM3->PSC = 799;
    TIM3->ARR = 999;
    // настраиваем источник TRGO
    //TIM3->CR2 &= ~(0b111 << TIM_CR2_MMS_Pos);     // Очищаем поле MMS
    //TIM3->CR2 |= (0x2 << TIM_CR2_MMS_Pos); // 010: Update event как TRGO [citation:5][citation:7]
    //TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM1_UP_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        TIM1->SR &= ~TIM_SR_UIF;
        // по окончанию преобразования будет вызвано событие ADC1_2_IRQHandler
        ADC1->CR2 |= ADC_CR2_SWSTART;
    }
}

void TIM2_IRQHandler(void){
    // target_temp += 0,2
    // print_new_target_temp - написать на дисплее новую температуру
}
