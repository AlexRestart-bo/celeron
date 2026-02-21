#include "main.h"

void GPIO_Config(void){
    // for ADC Analog mode
    GPIOB->CRL &= ~GPIO_CRL_CNF1; 
    GPIOB->CRL &= ~GPIO_CRL_MODE1;
    // for Numeric Display
    // segment A (PA0)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF0_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE0_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF0;
    GPIOA->CRL |= GPIO_CRL_MODE0;
    // segment B (PA1)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF1_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE1_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF1;
    GPIOA->CRL |= GPIO_CRL_MODE1;
    // segment C (PA2)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF2_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE2_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF2;
    GPIOA->CRL |= GPIO_CRL_MODE2;
    // segment D (PA3)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF3_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE3_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF3;
    GPIOA->CRL |= GPIO_CRL_MODE3;
    // segment E (PA4)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF4_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE4_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF4;
    GPIOA->CRL |= GPIO_CRL_MODE4;
    // segment F (PA5)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF5_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE5_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF5;
    GPIOA->CRL |= GPIO_CRL_MODE5;
    // segment G (PA6)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF6_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE6_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF6;
    GPIOA->CRL |= GPIO_CRL_MODE6;
    // segment DP (PA7)
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_CNF7_Pos);
    GPIOA->CRL &= ~(0b11 << GPIO_CRL_MODE7_Pos);
    GPIOA->CRL |= ~GPIO_CRL_CNF7;
    GPIOA->CRL |= GPIO_CRL_MODE7;
    // anode A1 (PA8)
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_CNF8_Pos);
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_MODE8_Pos);
    GPIOA->CRH |= ~GPIO_CRH_CNF8;
    GPIOA->CRH |= GPIO_CRH_MODE8;
    // anode A2 (PA9)
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_CNF9_Pos);
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_MODE9_Pos);
    GPIOA->CRH |= ~GPIO_CRH_CNF9;
    GPIOA->CRH |= GPIO_CRH_MODE9;
    // anode A3 (PA10)
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_CNF10_Pos);
    GPIOA->CRH &= ~(0b11 << GPIO_CRH_MODE10_Pos);
    GPIOA->CRH |= ~GPIO_CRH_CNF10;
    GPIOA->CRH |= GPIO_CRH_MODE10;
    // common reset
    GPIOA->BSRR = GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3 | GPIO_BSRR_BR4
                    | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7 | GPIO_BSRR_BR8 | GPIO_BSRR_BR9
                    | GPIO_BSRR_BR10;
}