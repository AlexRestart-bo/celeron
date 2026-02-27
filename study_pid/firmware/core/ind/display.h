#ifndef DISPLAY_H
#define DISPLAY_H

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

#define MAX_DISPLAY_TEMP 99.9f

typedef struct {
    uint32_t first_fig;
    uint32_t second_fig;
    uint32_t third_fig;
} FigVars;

extern FigVars set_figs;

void update_value(const float degrees);     // рассчитывает значение регистра GPIOA->BSRR
void common_reset(void);

#endif