#include "main.h"


float target_temp = 30.0f;      // сначала целевая температура равна 30 градусов
float temp = 0;                 // значение температуры в градусах Цельсия
volatile uint8_t status_lb = 1;          // status of left button 
volatile uint8_t status_rb = 1;          // status of right button
uint8_t duty_cycle = 10;        // сважность по умолчанию 10 %
// Функция для чтения состояния кнопки.
// Возвращает 0, если кнопка нажата (на пине низкий уровень), и 1, если отжата.
uint8_t Read_Button_PB14(void) {
    if (GPIOB->IDR & GPIO_IDR_IDR14) {
        return 1; // Кнопка не нажата (высокий уровень)
    } else {
        return 0; // Кнопка нажата (низкий уровень)
    }
}

uint8_t Read_Button_PB15(void) {
    if (GPIOB->IDR & GPIO_IDR_IDR15) {
        return 1; // Кнопка не нажата (высокий уровень)
    } else {
        return 0; // Кнопка нажата (низкий уровень)
    }
}

void show_current_temp(void){
    update_value(temp);
    enable_display();
}

void show_work_pid(void){
    update_value((float)get_borehole_value());
    enable_display();
}

void falling_temp(void){
    target_temp -= TEMP_STEP;
    if (target_temp <= TEMP_MIN) target_temp = TEMP_MIN;
    update_value(target_temp);
    enable_display(); 
}

void raising_temp(void){
    target_temp += TEMP_STEP;
    if (target_temp >= TEMP_MAX) target_temp = TEMP_MAX;
    update_value(target_temp);
    enable_display(); 
}

void enable_display(void){
    static uint8_t fig = 0;
    common_reset();
    switch (fig)
    {
    case 0:
        GPIOA->BSRR = set_figs.first_fig | DIG1;
        break;
    case 1:
        GPIOA->BSRR = set_figs.second_fig | DIG2;
        break;
    case 2:
        GPIOA->BSRR = set_figs.third_fig | DIG3;
        break;
    default:
        break;
    }
    fig = (fig + 1) % 3;
}
