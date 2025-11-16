#include "main.h"

#define C 2.0*powf(10, -8)
#define L 10.0*powf(10, -5)
#define ZL 50

#define BRACKET (1 - L*C*omega*omega)

float module_x(float x);

/**
 * В следующих четырех функциях представлены разбитые на части для удобства кусочки передаточной функции,
 * вид которой можно записать, как H = p / (x + jy). Параметр y возвращает функция campute_y(), параметр
 * x возвращает функция campute_x(), параметр p возвращает функция campute_p(). Функция campute_hyper()
 * возвращает модуль передаточной функции.
 * Схема четырехполюсника, который здесь описан представлена в "doc/circuitry/bandpass_filter1.jpg".
 */

float campute_x(float omega){
    return ZL*C*omega*BRACKET*BRACKET - ZL*C*C*L*powf(omega, 3);
}

float campute_y(float omega){
    return BRACKET*(2*C*L*powf(omega, 2) - BRACKET*BRACKET);
}

float campute_p(float omega){
    return -ZL*C*C*L*powf(omega, 3);
}

float campute_hyper(float omega){
    float x = campute_x(omega);
    float y = campute_y(omega);
    return module_x(campute_p(omega))/sqrtf(x*x + y*y);
}

float module_x(float x){
    if (x < 0) x*=-1;
    return x;
}