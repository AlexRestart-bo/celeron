#include "main.h"

FigVars set_figs = {0, 0, 0};

static void apply_changes_to_display(const uint8_t total, const uint8_t fract, const uint8_t comma);
void switch_number_with_comma(uint32_t* reg_value, const uint8_t figure);
void switch_number_without_comma(uint32_t* reg_value, const uint8_t figure);
static uint8_t get_power(int num);
int pow_int_to_int (int num, int power);
int round_up(float num, int symb);

void update_value(const float degrees){
    if (degrees < 0 || degrees >= MAX_DISPLAY_TEMP) return;

    uint8_t power_num = get_power((int)degrees);
    int rval = 0;
    uint8_t comma = INDEX_ZERO;
    if (power_num == 1){
        comma = INDEX_ONE;
        rval = round_up(degrees, 2);
    }else if(power_num == 2){
        rval = round_up(degrees, 1);
    }
    
    uint8_t total = (uint8_t)(comma ? (rval / 100) : (rval / 10));           // получение целой части
    uint8_t fract = (uint8_t)(comma ? (rval % 100) : (rval % 10));           // дробная часть может состоять как из двух цифр, так и из одной

    apply_changes_to_display(total, fract, comma);
}

// функция готовит нужный набор для дисплея
static void apply_changes_to_display(const uint8_t total, const uint8_t fract, const uint8_t comma){
    uint8_t first = 0, second = 0, third = 0;
    if (comma){
        first =  total;
        second = fract / 10;
        third = fract % 10;
    }else{
        first =  total / 10;
        second = total % 10;
        third = fract;    
    }
    
    set_figs.first_fig = set_figs.second_fig = set_figs.third_fig = 0;
    if (comma){         // Запятая находится на DIG1, остается два знака после запятой
        switch_number_with_comma(&set_figs.first_fig, first);
        switch_number_without_comma(&set_figs.second_fig, second);
    }else{              // Запятая находится на DIG2, остается один знак после запятой
        switch_number_without_comma(&set_figs.first_fig, first);
        switch_number_with_comma(&set_figs.second_fig, second);
    }                   // DIG3 всегда остается без запятой
    switch_number_without_comma(&set_figs.third_fig, third);
    // для индикатора каждый сегмент включается, если есть подтяжка к земле??
    set_figs.first_fig = ~set_figs.first_fig;
    set_figs.second_fig = ~set_figs.second_fig;
    set_figs.third_fig = ~set_figs.third_fig;
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

// возвращает степень числа
static uint8_t get_power(int num){
    uint8_t pow = 0;
    while (num){
        num /= 10;
        pow++;
    }
    return pow;
}

// округляет дробное число
// @param symb - знак после запятой, до которого нужно округлить
int round_up(float num, int symb){
    if (symb < 0) return 0;
    int mul_num = (int)(num*pow_int_to_int(10, symb + 1));
    if (mul_num % 10 >= 5)
        mul_num += 10;
    mul_num /= 10;
    return mul_num;
}

int pow_int_to_int (int num, int power){
    if (power == 0)
        return 1;
    else 
        return num*pow_int_to_int(num, power - 1);
}