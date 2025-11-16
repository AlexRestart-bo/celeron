#include <stdio.h>
#include "main.h"

static inline void imposed_data(void);

sync_detector_t det;

int main(void){
    imposed_data();
    return 0;
}

static inline void imposed_data(void){
    float pristine_signal[DATA_SIZE];
    float aux_data[DATA_SIZE];
    float input_data[DATA_SIZE];
    float output_data[10][DATA_SIZE];
    float phases_output[DATA_SIZE];
    float noise_data[DATA_SIZE];

    generate_sinf(pristine_signal, DATA_SIZE, 1000, 1, 0);
    generate_sinf(aux_data, DATA_SIZE, 100, 1, 0);
    unification_generators(input_data, pristine_signal, aux_data, DATA_SIZE, DATA_SIZE, DATA_SIZE, MULTIPLY);
    generate_noise(noise_data, DATA_SIZE, 1000);
    unification_generators(input_data, input_data, noise_data, DATA_SIZE, DATA_SIZE, DATA_SIZE, PLUS);
    //generate_sinf(aux_data, DATA_SIZE, 100, 1, 0);
    //unification_generators(input_data, input_data, aux_data, DATA_SIZE, DATA_SIZE, DATA_SIZE, MULTIPLY);

    for(int i = 0; i < 10; ++i){
        sync_detector_init(&det, REF_FREQ, FS, 0.01*i);
        for (int j = 0; j < DATA_SIZE; j++){
            sync_detector_process(&det, input_data[j], &output_data[i - 1][j], phases_output + j);
        }
    }

    FILE *file;
    file = fopen("out.txt", "w");
    if (file == NULL){
        return;
    }

    for (int i = 0; i < DATA_SIZE; i++) 
        fprintf(file, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 350*pristine_signal[i], output_data[0][i], output_data[1][i], 
            output_data[2][i], output_data[3][i], output_data[4][i], output_data[5][i], output_data[6][i], 
            output_data[7][i], output_data[8][i], output_data[9][i]);
    fclose(file);

    FILE *filex;
    filex = fopen("band_out.txt", "w");
    
    for (int i = -2; i < 1300; i++)fprintf(filex, "%f\t%f\n", powf(10, i*0.01)/(2.0*PI), campute_hyper(powf(10, i*0.01)));

    fclose(filex);

    filex = fopen("ps.txt", "w");
    
    for (int i = -2; i < 1300; i++)fprintf(filex, "%f\t%f\n", powf(10, i*0.01)/(2.0*PI), shift_phase(powf(10, i*0.01)));

    fclose(filex);

    stand_wave_collect();
}