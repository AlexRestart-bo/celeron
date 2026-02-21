#include "main.h"

void TIM1_Init(void){
    TIM1->SMCR &= ~TIM_SMCR_SMS;
    TIM1->PSC = 799;
    //TIM1->CNT = 9;
    TIM1->ARR = 4999;
    TIM1->DIER |= TIM_DIER_UIE;
    TIM1->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_SetPriority(TIM1_UP_IRQn, 0);
    TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM2_Init(void){
    TIM2->SMCR &= ~TIM_SMCR_SMS;
    TIM2->PSC = 799;
    //TIM1->CNT = 9;
    TIM2->ARR = 4999;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM1_UP_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        TIM1->SR &= ~TIM_SR_UIF;
        GPIOC->ODR ^= GPIO_ODR_ODR13;
    }
}