#include "main.h"

#define R powf(10, 3)
#define C powf(10, -9)

float shift_phase(float omega){
    float a = 1.0 / (R*C);
    return atan2f(2.0*omega*a, (omega*omega - a*a));
}