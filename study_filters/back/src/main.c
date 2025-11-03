#include <stdio.h>
#include "main.h"

sync_detector_t det;

int main(void){
    float test_signal[DATA_SIZE];
    float pristine_signal[DATA_SIZE];
    float aux_data[DATA_SIZE];
    float input_data[DATA_SIZE];
    float output_data[DATA_SIZE];
    float phases_output[DATA_SIZE];
    float noise_data[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++) test_signal[i] = 1;

    //generate_square(input_data, DATA_SIZE);
    // синнус 1000 Гц
    generate_sinf(pristine_signal, DATA_SIZE, 100, 1, 0);
    // утроенный косинус 500 Гц
    generate_sinf(aux_data, DATA_SIZE, 200, 3, 3*PI/2);
    // складываем утроенный косинус и синус
    unification_generators(pristine_signal, pristine_signal, aux_data, DATA_SIZE, DATA_SIZE, DATA_SIZE, PLUS);
    // меандр 1000 Гц
    //generate_square(aux_data, DATA_SIZE);
    // складываем меандр с остальными гармониками
    //unification_generators(pristine_signal, pristine_signal, aux_data, DATA_SIZE, DATA_SIZE, DATA_SIZE, PLUS);
    // модуляция частотой 1000 Гц
    generate_sinf(aux_data, DATA_SIZE, 1000, 1, 0);
    unification_generators(pristine_signal, pristine_signal, aux_data, DATA_SIZE, DATA_SIZE, DATA_SIZE, MULTIPLY);
    unification_generators(test_signal, test_signal, aux_data, DATA_SIZE, DATA_SIZE, DATA_SIZE, MULTIPLY);
    // создаем шум с амплитудой 1000 единиц
    generate_noise(noise_data, DATA_SIZE, 1000);
    // объединяем шум и полезный сигнал
    unification_generators(input_data, pristine_signal, pristine_signal, DATA_SIZE, DATA_SIZE, DATA_SIZE, PLUS);
    sync_detector_init(&det, REF_FREQ, FS, ALPHA);
    for (int i = 0; i < DATA_SIZE; i++){
        sync_detector_process(&det, input_data[i], output_data + i, phases_output + i);
    }

    FILE *file;
    file = fopen("out.txt", "w");
    if (file == NULL){
        return 0;
    }

    for (int i = 0; i < DATA_SIZE; i++) fprintf(file, "%f\t%f\n", input_data[i], output_data[i]);

    fclose(file);
    /*FILE *noise_file;
    noise_file = fopen("noise.txt", "w");
    if(noise_file == NULL){
        return 0;
    }

    generate_noise(noise_data, DATA_SIZE, 5);
    for (int i = 0; i < DATA_SIZE; i++) fprintf(noise_file, "%f\n", noise_data[i]);

    fclose(noise_file);*/

    return 0;
}