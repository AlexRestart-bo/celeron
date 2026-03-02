#include "main.h"

void GPIO_Init(void){
    // for ADC Analog mode
    GPIOB->CRL &= ~(GPIO_CRL_CNF1); 
    GPIOB->CRL &= ~GPIO_CRL_MODE1;
    // for PWM - output mode 50 MHz
    GPIOB->CRH &= ~(0b11 << GPIO_CRH_CNF13_Pos);
    GPIOB->CRH &= ~(0b11 << GPIO_CRH_MODE13_Pos);
    // Alternate function Push-Pull 
    GPIOB->CRH |= GPIO_CRH_CNF13_1;
    GPIOB->CRH |= GPIO_CRH_MODE13;
    // for Numeric Display
    // segment A (PA0)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF0_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE0_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF0;
    GPIOA->CRL |= GPIO_CRL_MODE0;
    // segment B (PA1)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF1_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE1_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF1;
    GPIOA->CRL |= GPIO_CRL_MODE1;
    // segment C (PA2)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF2_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE2_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF2;
    GPIOA->CRL |= GPIO_CRL_MODE2;
    // segment D (PA3)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF3_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE3_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF3;
    GPIOA->CRL |= GPIO_CRL_MODE3;
    // segment E (PA4)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF4_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE4_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF4;
    GPIOA->CRL |= GPIO_CRL_MODE4;
    // segment F (PA5)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF5_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE5_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    GPIOA->CRL |= GPIO_CRL_MODE5;
    // segment G (PA6)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF6_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE6_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF6;
    GPIOA->CRL |= GPIO_CRL_MODE6;
    // segment DP (PA7)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF7_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE7_Pos);
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    GPIOA->CRL |= GPIO_CRL_MODE7;
    // anode A1 (PA8)
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_CNF8_Pos);
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_MODE8_Pos);
    GPIOA->CRH &= ~GPIO_CRH_CNF8;
    GPIOA->CRH |= GPIO_CRH_MODE8;
    // anode A2 (PA9)
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_CNF9_Pos);
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_MODE9_Pos);
    GPIOA->CRH &= ~GPIO_CRH_CNF9;
    GPIOA->CRH |= GPIO_CRH_MODE9;
    // anode A3 (PA10)
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_CNF10_Pos);
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_MODE10_Pos);
    GPIOA->CRH &= ~GPIO_CRH_CNF10;
    GPIOA->CRH |= GPIO_CRH_MODE10;
    // common reset
    GPIOA->BSRR = GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3 | GPIO_BSRR_BR4
                    | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7 | GPIO_BSRR_BR8 | GPIO_BSRR_BR9
                    | GPIO_BSRR_BR10;
    
    // for Buttons - output mode 50 MHz

    // PB14 response for right button (raising target temperature)
    GPIOB->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);
    GPIOB->CRH |= GPIO_CRH_CNF14_1;
    // Включаем подтяжку к питанию (pull-up) через регистр ODR
    // Запись 1 в соответствующий бит ODR активирует подтяжку для pull-up режима.
    GPIOB->ODR |= GPIO_ODR_ODR14;

    // PB15 response for left button (falling target temperature)
    GPIOB->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
    GPIOB->CRH |= GPIO_CRH_CNF15_1;
    GPIOB->ODR |= GPIO_ODR_ODR15;
}

void common_reset(void){
    // common reset PAx 
    GPIOA->BSRR = GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3 | GPIO_BSRR_BR4
                    | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7 | GPIO_BSRR_BR8 | GPIO_BSRR_BR9
                    | GPIO_BSRR_BR10;
}