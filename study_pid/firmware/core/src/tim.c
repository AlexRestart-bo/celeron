#include "main.h"

uint8_t flag_interrupt_tim2 = 0;

void TIM1_Init(void){
    // таймер вызывает обработчик события (переполнение) раз в 20 мс
    TIM1->PSC = 799;
    TIM1->ARR = 199;
    // PWM mode 1
    TIM1->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;
    // Скважность
    TIM1->CCR1 = get_borehole_value();
    // Для STM32F1, TIM1_CH1N включается битом CC1NE в регистре CCER
    TIM1->CCER &= ~TIM_CCER_CC1NP;  // Полярность: активный высокий уровень (обычно)
    TIM1->CCER |= TIM_CCER_CC1NE;   // Включение выхода CH1N
    // Для TIM1 нужно настроить режим Main Output (бит MOE в BDTR)
    TIM1->BDTR |= TIM_BDTR_MOE;  // Включение главного выхода (обязательно для TIM1!)
    TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM2_Init(void){
    // таймер вызывает обработчик события (переполнение) раз в 100 мс
    TIM2->SMCR &= ~TIM_SMCR_SMS;
    TIM2->PSC = 799;
    TIM2->ARR = 999;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM3_Init(void){
    // таймер вызывает обработчик события (переполнение) раз в 50 мс
    TIM3->SMCR &= ~TIM_SMCR_SMS;
    TIM3->PSC = 799;
    TIM3->ARR = 499;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void){
    flag_interrupt_tim2 = 1;
    TIM2->SR &= ~TIM_SR_UIF;
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
        // по окончанию преобразования будет вызвано событие ADC1_2_IRQHandler
        ADC1->CR2 |= ADC_CR2_SWSTART;
    }
}

void execute_orders(void){
    if (flag_interrupt_tim2){
        switch (mode){
            case NOTHING:
                show_current_temp();
                break;
            case DOUBLE:
                show_work_pid();
                break;
            case SINGLEL:
                falling_temp();
                break;
            case SINGLER:
                raising_temp();
                break;
            default:
                break;
        }
        flag_interrupt_tim2 = 0;
    }
}

uint32_t get_borehole_value(void) {
    return TIM1COUNTER * duty_cycle / 100;
}
