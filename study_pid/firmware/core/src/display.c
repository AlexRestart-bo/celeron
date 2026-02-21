#include "main.h"

// ниже написана последовательность для включения символов на дисплее, 
// которая будет применяться в регистре GPIOA->BSRR
#define SEGA GPIO_BSRR_BS0
#define SEGB GPIO_BSRR_BS1
#define SEGC GPIO_BSRR_BS2
#define SEGD GPIO_BSRR_BS3
#define SEGE GPIO_BSRR_BS4
#define SEGF GPIO_BSRR_BS5
#define SEGG GPIO_BSRR_BS6
#define SEGDP GPIO_BSRR_BS7

#define DIG1 GPIO_BSRR_BS8
#define DIG2 GPIO_BSRR_BS9
#define DIG3 GPIO_BSRR_BS10

#define CHARS_WITHPOINT SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG | SEGDP
#define CHAR0_WITHPOINT SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGDP
#define CHAR1_WITHPOINT SEGB | SEGC | SEGDP
#define CHAR2_WITHPOINT SEGA | SEGB | SEGD | SEGE | SEGG | SEGDP
#define CHAR3_WITHPOINT SEGA | SEGB | SEGC | SEGD | SEGG | SEGDP
#define CHAR4_WITHPOINT SEGB | SEGC | SEGF | SEGG | SEGDP
#define CHAR5_WITHPOINT SEGA | SEGC | SEGD | SEGF | SEGG | SEGDP
#define CHAR6_WITHPOINT SEGA | SEGC | SEGD | SEGE | SEGF | SEGG | SEGDP
#define CHAR7_WITHPOINT SEGA | SEGB | SEGC | SEGDP
#define CHAR8_WITHPOINT SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG | SEGDP
#define CHAR9_WITHPOINT SEGA | SEGB | SEGC | SEGD | SEGF | SEGG | SEGDP

#define CHARS_WITHOUTPOINT SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG
#define CHAR0_WITHOUTPOINT SEGA | SEGB | SEGC | SEGD | SEGE | SEGF
#define CHAR1_WITHOUTPOINT SEGB | SEGC
#define CHAR2_WITHOUTPOINT SEGA | SEGB | SEGD | SEGE | SEGG
#define CHAR3_WITHOUTPOINT SEGA | SEGB | SEGC | SEGD | SEGG
#define CHAR4_WITHOUTPOINT SEGB | SEGC | SEGF | SEGG
#define CHAR5_WITHOUTPOINT SEGA | SEGC | SEGD | SEGF | SEGG
#define CHAR6_WITHOUTPOINT SEGA | SEGC | SEGD | SEGE | SEGF | SEGG
#define CHAR7_WITHOUTPOINT SEGA | SEGB | SEGC
#define CHAR8_WITHOUTPOINT SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG
#define CHAR9_WITHOUTPOINT SEGA | SEGB | SEGC | SEGD | SEGF | SEGG

#define INDEX_ZERO 0
#define INDEX_ONE 1

uint32_t first_fig = 0, second_fig = 0, third_fig = 0;

void apply_changes_to_display(const uint8_t total, const uint8_t fract, const uint8_t comma);

void update_value(const float degrees){
    uint8_t comma = INDEX_ZERO;
    if (degrees < 0 || degrees >= 99.95) return NULL;
    uint8_t total = (uint8_t)degrees;           // получение целой части
    uint8_t fract = 0;                          // дробная часть
    if (degrees >= 9.995)                       // если получается, можно дать два знака после запятой
        fract = (uint8_t)(((degrees*10 - (float)(total*10)) >= 0.5f) ? (degrees*10 - total*10) + 1 : (degrees*10 - total*10)); 
    else{
        fract = (uint8_t)(((degrees*100 - total*100) >= 0.5f) ? (degrees*100 - total*100) + 1 : (degrees*100 - total*100));
        comma = INDEX_ONE;
    }
        
    apply_changes_to_display(total, fract, comma);
}

// функция готовит нужный набор для дисплея
void apply_changes_to_display(const uint8_t total, const uint8_t fract, const uint8_t comma){
    uint8_t first =  total / 10;
    uint8_t second = total % 10;
    uint8_t third = fract;
    
    first_fig = second_fig = third_fig = 0;
    switch (first){         // Запятая находится на DIG1, остается два знака после запятой
        case 0:
            first_fig |= (comma) ? CHAR0_WITHPOINT : CHAR0_WITHOUTPOINT;
            break;
        case 1:
            first_fig |= (comma) ? CHAR1_WITHPOINT : CHAR1_WITHOUTPOINT;
            break;
        case 2:
            first_fig |= (comma) ? CHAR2_WITHPOINT : CHAR2_WITHOUTPOINT;
            break;
        case 3:
            first_fig |= (comma) ? CHAR3_WITHPOINT : CHAR3_WITHOUTPOINT;
            break;
        case 4:
            first_fig |= (comma) ? CHAR4_WITHPOINT : CHAR4_WITHOUTPOINT;
            break;
        case 5:
            first_fig |= (comma) ? CHAR5_WITHPOINT : CHAR5_WITHOUTPOINT;
            break;
        case 6:
            first_fig |= (comma) ? CHAR6_WITHPOINT : CHAR6_WITHOUTPOINT;
            break;
        case 7:
            first_fig |= (comma) ? CHAR7_WITHPOINT : CHAR7_WITHOUTPOINT;
            break;
        case 8:
            first_fig |= (comma) ? CHAR8_WITHPOINT : CHAR8_WITHOUTPOINT;
            break;
        case 9:
            first_fig |= (comma) ? CHAR9_WITHPOINT : CHAR9_WITHOUTPOINT;
            break;
        default:
            first_fig |= (comma) ? CHAR0_WITHPOINT : CHAR0_WITHOUTPOINT;
            break;
    }

    switch (second){        // Запятая находится на DIG2, остается один знак после запятой
        case 0:
            second_fig |= (comma) ? CHAR0_WITHOUTPOINT : CHAR0_WITHPOINT;
            break;
        case 1:
            second_fig |= (comma) ? CHAR1_WITHOUTPOINT : CHAR1_WITHPOINT;
            break;
        case 2:
            second_fig |= (comma) ? CHAR2_WITHOUTPOINT : CHAR2_WITHPOINT;
            break;
        case 3:
            second_fig |= (comma) ? CHAR3_WITHOUTPOINT : CHAR3_WITHPOINT;
            break;
        case 4:
            second_fig |= (comma) ? CHAR4_WITHOUTPOINT : CHAR4_WITHPOINT;
            break;
        case 5:
            second_fig |= (comma) ? CHAR5_WITHOUTPOINT : CHAR5_WITHPOINT;
            break;
        case 6:
            second_fig |= (comma) ? CHAR6_WITHOUTPOINT : CHAR6_WITHPOINT;
            break;
        case 7:
            second_fig |= (comma) ? CHAR7_WITHOUTPOINT : CHAR7_WITHPOINT;
            break;
        case 8:
            second_fig |= (comma) ? CHAR8_WITHOUTPOINT : CHAR8_WITHPOINT;
            break;
        case 9:
            second_fig |= (comma) ? CHAR9_WITHOUTPOINT : CHAR9_WITHPOINT;
            break;
        default:
            second_fig |= (comma) ? CHAR0_WITHOUTPOINT : CHAR0_WITHPOINT;
            break;
    }

    switch (third)
    {
    case 0:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 1:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 2:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 3:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 4:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 5:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 6:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 7:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 8:
        third |= CHAR0_WITHOUTPOINT;
        break;
    case 9:
        third |= CHAR0_WITHOUTPOINT;
        break;
    default:
        third |= CHAR0_WITHOUTPOINT;
        break;
    }
}

//void switch_number_with_comma(uint32_t* reg_value){}