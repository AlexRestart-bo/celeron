#include "main.h"

uint8_t mode = NOTHING;

void delay_ms(uint32_t ms);

int main(void){
    Enable_Clocks();
    GPIO_Init();
    ADC1_Init();
    TIM1_Init();
    TIM2_Init();
    TIM3_Init();
    
    while(1){
        status_rb = Read_Button_PB14();             // 0 - кнопка нажата, 1 - не нажата
        status_lb = Read_Button_PB15();
        if (status_rb == 0 && status_lb == 0){      // отработка нажатия и удержания двух кнопок одновременно
            delay_ms(100);                          // отработка дребезга
            if (status_lb == 0 && status_rb == 0) {
                mode = DOUBLE;
                while ((status_rb = Read_Button_PB14()) == 0 && (status_lb = Read_Button_PB15()) == 0) execute_orders();
                mode = NOTHING;
            }
        }else if(status_rb == 1 && status_lb == 0){
            delay_ms(100);
            if (status_rb == 1 && status_lb == 0){
                mode = SINGLEL;
                while ((status_rb = Read_Button_PB14()) == 1 && (status_lb = Read_Button_PB15()) == 0) execute_orders();
                mode = NOTHING;
            }
        }else if(status_rb == 0 && status_lb == 1){
            delay_ms(100);
            if (status_rb == 0 && status_lb == 1){
                mode = SINGLER;
                while ((status_rb = Read_Button_PB14()) == 0 && (status_lb = Read_Button_PB15()) == 1) execute_orders();
                mode = NOTHING;
            }
        }
        execute_orders();
    }
}

void Enable_Clocks(void){
    // Включение тактирования ADC1, GPIOA, GPIOB, TIM1, TIM2, TIM4
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM4EN; 
}

void delay_ms(uint32_t ms){
    // Рассчет: 1 мс = частота_ядра / 1000 тиков
    uint32_t ticks_per_ms = SystemCoreClock / 1000;

    for (uint32_t i = 0; i < ms; i++)
    {
        SysTick->LOAD = ticks_per_ms - 1; // Устанавливаем счетчик
        SysTick->VAL = 0;                  // Очищаем текущее значение
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Включаем, без прерываний

        // Ждем, пока COUNTFLAG (бит 16) не станет 1
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

        SysTick->CTRL = 0; // Выключаем таймер
    }
}