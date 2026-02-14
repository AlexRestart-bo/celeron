#include "main.h"

void USART1_Init(void) {
    // 1. Тактирование уже включено в Enable_Clocks()
    // 2. GPIO уже настроено в GPIO_Config()
    
    // 3. Настройка USART
    USART1->BRR = 8000000 / 115200; // Baud rate для 8MHz
    
    // 4. Включение USART, передатчика и приемника
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void USART1_SendChar(uint8_t data) {
    while (!(USART1->SR & USART_SR_TXE)); // Ждем готовности
    USART1->DR = data;
}

uint8_t USART1_ReceiveChar(void) {
    while (!(USART1->SR & USART_SR_RXNE)); // Ждем данные
    return USART1->DR;
}

void USART1_SendString(uint8_t *str, uint8_t size){
    for (int i = 0; i < size; i++){
        USART1_SendChar(*(str + i));
    }
}