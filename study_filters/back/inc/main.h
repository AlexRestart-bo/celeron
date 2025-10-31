#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define PI 3.14159265358979323846

#define REF_FREQ 5000
#define FS 20000
#define ALPHA 0.1
#define DATA_SIZE 1000

typedef struct {
    float ref_frequency;    // Частота опорного сигнала (Гц)
    float sampling_rate;    // Частота дискретизации (Гц)
    float phase;           // Фаза опорного сигнала (радианы)
    float time_index;      // Текущее время для генерации опорного сигнала
    float i_prev, q_prev;  // Предыдущие значения для фильтрации
    float alpha;           // Коэффициент сглаживания ФНЧ
} sync_detector_t;

void sync_detector_init(sync_detector_t* det, float ref_freq, float fs, float filter_alpha);
void sync_detector_process(sync_detector_t* det, float input_sample, 
                          float* amplitude, float* phase_out);
void generate_signal(float *data, int size_data);

#endif