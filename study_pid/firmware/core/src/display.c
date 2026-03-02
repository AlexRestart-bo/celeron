#include "main.h"

FigVars set_figs = {0, 0, 0};


static void apply_changes_to_display(const uint8_t total, const uint8_t fract, const uint8_t comma);
void switch_number_with_comma(uint32_t* reg_value, const uint8_t figure);
void switch_number_without_comma(uint32_t* reg_value, const uint8_t figure);

void update_value(const float degrees){
    uint8_t comma = INDEX_ZERO;
    if (degrees < 0 || degrees >= MAX_DISPLAY_TEMP) return;
    uint8_t total = (uint8_t)degrees;           // получение целой части
    uint8_t fract = 0;                          // дробная часть
    if (degrees >= 9.995)                       // если получается, можно дать два знака после запятой
        fract = (uint8_t)((degrees - total) * 100 + 0.5f); 
    else{
        fract = (uint8_t)((degrees - total) * 10 + 0.5f);
        comma = INDEX_ONE;
    }
        
    apply_changes_to_display(total, fract, comma);
}

// функция готовит нужный набор для дисплея
static void apply_changes_to_display(const uint8_t total, const uint8_t fract, const uint8_t comma){
    uint8_t first =  total / 10;
    uint8_t second = total % 10;
    uint8_t third = fract;
    
    set_figs.first_fig = set_figs.second_fig = set_figs.third_fig = 0;
    if (comma){         // Запятая находится на DIG1, остается два знака после запятой
        switch_number_with_comma(&set_figs.first_fig, first);
        switch_number_without_comma(&set_figs.second_fig, second);
    }else{              // Запятая находится на DIG2, остается один знак после запятой
        switch_number_without_comma(&set_figs.first_fig, first);
        switch_number_with_comma(&set_figs.second_fig, second);
    }                   // DIG3 всегда остается без запятой
    switch_number_without_comma(&set_figs.third_fig, third);
}

/*  Функция готовит значение *reg_value, для того чтобы включить одну цифру дисплея. Здесь цифра будет с точкой. */
void switch_number_with_comma(uint32_t* reg_value, const uint8_t figure){
    switch (figure)
    {
    case 0:
        *reg_value |= CHAR0_WITHPOINT;
        break;
    case 1:
        *reg_value |= CHAR1_WITHPOINT;
        break;
    case 2:
        *reg_value |= CHAR2_WITHPOINT;
        break;
    case 3:
        *reg_value |= CHAR3_WITHPOINT;
        break;
    case 4:
        *reg_value |= CHAR4_WITHPOINT;
        break;
    case 5:
        *reg_value |= CHAR5_WITHPOINT;
        break;
    case 6:
        *reg_value |= CHAR6_WITHPOINT;
        break;
    case 7:
        *reg_value |= CHAR7_WITHPOINT;
        break;
    case 8:
        *reg_value |= CHAR8_WITHPOINT;
        break;
    case 9:
        *reg_value |= CHAR9_WITHPOINT;
        break;
    default:
        *reg_value |= CHAR0_WITHPOINT;
        break;
    }
}

/*  Функция готовит значение *reg_value, для того чтобы включить одну цифру дисплея. Здесь цифра будет без точки. */
void switch_number_without_comma(uint32_t* reg_value, const uint8_t figure){
    switch (figure)
    {
    case 0:
        *reg_value |= CHAR0_WITHOUTPOINT;
        break;
    case 1:
        *reg_value |= CHAR1_WITHOUTPOINT;
        break;
    case 2:
        *reg_value |= CHAR2_WITHOUTPOINT;
        break;
    case 3:
        *reg_value |= CHAR3_WITHOUTPOINT;
        break;
    case 4:
        *reg_value |= CHAR4_WITHOUTPOINT;
        break;
    case 5:
        *reg_value |= CHAR5_WITHOUTPOINT;
        break;
    case 6:
        *reg_value |= CHAR6_WITHOUTPOINT;
        break;
    case 7:
        *reg_value |= CHAR7_WITHOUTPOINT;
        break;
    case 8:
        *reg_value |= CHAR8_WITHOUTPOINT;
        break;
    case 9:
        *reg_value |= CHAR9_WITHOUTPOINT;
        break;
    default:
        *reg_value |= CHAR0_WITHOUTPOINT;
        break;
    }
}