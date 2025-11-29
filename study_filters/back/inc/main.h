#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#define PI 3.14159265358979323846

typedef struct {
    float ref_frequency;    // Частота опорного сигнала (Гц)
    float sampling_rate;    // Частота дискретизации (Гц)
    float phase;           // Фаза опорного сигнала (радианы)
    float time_index;      // Текущее время для генерации опорного сигнала
    float i_prev, q_prev;  // Предыдущие значения для фильтрации
    float alpha;           // Коэффициент сглаживания ФНЧ
} sync_detector_t;

void sync_detector_init(sync_detector_t* det, float ref_freq, float fs, float filter_alpha);
void sync_detector_process(sync_detector_t* det, float input_sample, float* amplitude, float* phase_out);
void generate_square(float *data, int size_data);
void generate_sinf(float *data, int size_data, int freq, float amplitude, float offset);
void generate_linear(float *data, int size_data, float k, float b);
void generate_x2(float *data, int size_data, float x1, float x2);

void unification_generators(float *res_gen, float *gen1, float *gen2, int size_res_gen, int size_gen1, int size_gen2, int OPERATION);

void generate_noise(float *data, int size_data, float amplitude);

float campute_hyper(float omega);

float shift_phase(float omega);

void stand_wave_collect(void);

#endif