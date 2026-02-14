#include "main.h"

int main(void) {

}

void Enable_Clocks(void){
    // GPIO clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  // PORT A
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;  // PORT C
    
    // Peripheral clocks
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // USART1
    // Включение тактирования ADC1 и GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;
    // TIM1 clocks
    RCC->APB2ENR |= (1 << 11);
}

void delay_ms(int sec){
    for (int i = 0; i < sec*100000; i++);
}
