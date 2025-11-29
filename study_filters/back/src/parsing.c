#include "main.h"

void sync_detector_init(sync_detector_t* det, float ref_freq, float fs, float filter_alpha) {
    det->ref_frequency = ref_freq;
    det->sampling_rate = fs;
    det->phase = 0.0f;
    det->time_index = 0.0f;
    det->i_prev = 0.0f;
    det->q_prev = 0.0f;
    det->alpha = filter_alpha;
}

// Обработка одного отсчета
void sync_detector_process(sync_detector_t* det, float input_sample, float* amplitude, float* phase_out) {
    // Генерация опорных сигналов
    float angle = 2.0f * PI * det->ref_frequency * det->time_index + det->phase;
    float ref_i = cosf(angle);  // Синфазная составляющая
    float ref_q = sinf(angle);  // Квадратурная составляющая
    
    // Умножение на опорный сигнал
    float mixed_i = input_sample * ref_i;
    float mixed_q = input_sample * ref_q;
    
    // Простой ФНЧ (экспоненциальное сглаживание)
    float i_filtered = det->alpha * mixed_i + (1.0f - det->alpha) * det->i_prev;
    float q_filtered = det->alpha * mixed_q + (1.0f - det->alpha) * det->q_prev;
    
    // Сохранение для следующей итерации
    det->i_prev = i_filtered;
    det->q_prev = q_filtered;
    det->time_index += 1.0f / det->sampling_rate;
    
    // Вычисление амплитуды и фазы
    *phase_out = atan2f(q_filtered, i_filtered);
    *amplitude = 2.0f * sqrtf(i_filtered * i_filtered + q_filtered * q_filtered)*sinf(*phase_out);
}
