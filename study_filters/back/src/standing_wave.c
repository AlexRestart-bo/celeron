#include "main.h"

#define SHIFT 0//0.01*PI
#define SEGMENT 0.5
#define LAMBDA 3.0*powf(10, -6)
#define VEL 3.0*powf(10, 8)

#define snap 10         // число снимков на периоде 
#define accuracy 100    // точность отрисовки -> сто точек по оси z на периоде

float collect_prof[snap][accuracy*10];

float calculate_elect_vector(float t, float z){
    return cos(2*PI*(t - z)) + SEGMENT*cosf(2*PI*(t + z) + SHIFT);
    //return cosf(2*PI*(VEL/LAMBDA)*t - 2*PI*z/LAMBDA) + SEGMENT*cosf(2*PI*(VEL/LAMBDA)*t + 2*PI*z/LAMBDA + SHIFT);
}

/**
 * период части, состоящей из координаты можно сосчитать как k(z - z0) = 2*PI
 * период части, включающей время omega*(t - t0) = 2*PI
 * Время будет статическим параметром, а координата - динамическим: в определенное время tx мы строим 
 * вид волны, перебирая координаты z так, чтобы увидеть несколько периодов. 
 * Время будем менять в пределах периода -> t = 0.1*i*(1/frequency), 0 <= i <= 10
 */

void stand_wave_collect(void){
    for(int i = 0; i < snap; i++){
        for (int j = 0; j < accuracy*10; j++){
            collect_prof[i][j] = calculate_elect_vector(i*(1.0 / (float)snap), 
                j*(1.0/(float)accuracy));
            //collect_prof[i][j] = calculate_elect_vector(i*(1.0 / (float)snap)*(LAMBDA/VEL), 
            //    j*(1.0/(float)accuracy)*LAMBDA);
        }
    }
    FILE *file_wave;
    file_wave = fopen("std_wave.txt", "w");

    for (int j = 0; j < accuracy*10; j++){
        for(int i = 0; i < snap; i++) fprintf(file_wave, "%f\t", collect_prof[i][j]);
        fprintf(file_wave, "\n");
    }

    fclose(file_wave);
}