#include "main.h"

void  redirect_data(uint32_t data);

void ADC1_Init(void){
    // Включение тактирования ADC (если не включено)
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    
    // Калибровка ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while(ADC1->CR2 & ADC_CR2_RSTCAL);
    ADC1->CR2 |= ADC_CR2_CAL;
    while(ADC1->CR2 & ADC_CR2_CAL);
    
    // Конфигурация
    ADC1->CR1 = ADC_CR1_EOCIE;  // Включить прерывание по EOC
    ADC1->CR2 = ADC_CR2_ADON | ADC_CR2_CONT;  // Непрерывный режим
    ADC1->SMPR2 = ADC_SMPR2_SMP9;  // Максимальное время выборки
    ADC1->SQR3 |= 9;  // Записываем номер канала (9) в первое (SQ1) преобразование последовательности
    ADC1->SQR1 &= ~(ADC_SQR1_L); // Очищаем биты длины последовательности
    ADC1->SQR1 |= 0; // Устанавливаем длину последовательности = 1 (преобразуем только один канал)
    
    // Запуск непрерывного преобразования
    ADC1->CR2 |= ADC_CR2_ADON;  // Второй раз для запуска в непрерывном режиме
    
    // Включение прерывания в NVIC
    NVIC_EnableIRQ(ADC1_2_IRQn);
    NVIC_SetPriority(ADC1_2_IRQn, 1);
}

void ADC1_2_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_EOC){
        redirect_data((uint32_t)ADC1->DR);
        ADC1->SR &= ~ADC_SR_EOC;
    }
}
/** В то время как в помещении 24.8 градусов на ножке PB1 2.38 В
 * предположим, что напряжение перехода база-эмиттер зависит от температуры линейно: V_be = V_be(T0) + k * (T - T0).
 * В самом частом случае k = -2 mV/*C, тогда рассчитать температуру можно следующим способом:
 *      T = (1/k)(V_be - V_be(T0)) + T0 = -500*(V - 2.38) + 24.8
 */
void  redirect_data(uint32_t data){
    float V_be = ((float)data) * 3.3f / 4096.0f;
    temp = -10.0f*(V_be - 2.38f) + 24.8f;
    // работа ПИД-регулятора
    compute_pid();
}

