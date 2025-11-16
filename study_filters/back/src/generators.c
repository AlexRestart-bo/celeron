#include "main.h"

void generate_square(float *data, int size_data){
    unsigned char sq = 0;
    for (int i = 0; i < size_data; i++){
        *(data + i) = 2*sq - 1;
        if(i % 5 == 0) sq = 1 - sq;
    }
}

void generate_sinf(float *data, int size_data, int freq, float amplitude, float offset){
    for (int i = 0; i < size_data; i++){
        *(data + i) = amplitude*sinf(2*PI*freq*i*(1.0f/FS) + offset);
    }
}

void generate_single_pulse(float *sequence, int size_sequence, int sample, float amplitude){
    if (sample > size_sequence - 1 || sample < 0) printf("Error with input data: in fuction generate_single_pulse");
    else *(sequence + sample) = amplitude;
}

void generate_single_jump(float *sequence, int size_sequence, int sample, float amplitude){
    while (sample++ < size_sequence) *(sequence + sample) = amplitude;
}

void generate_exp(float *sequence, int size_sequence, float alpha, float amplitude, int offset){
    for (int i = 0; i < size_sequence; i++) *(sequence + i) = amplitude*powf(alpha, i - offset);
}

void generate_sin(float *sequence, int size_sequence, float frequency, float amplitude, int offset_arg){
    for (int i = 0; i < size_sequence; i++) *(sequence + i) = amplitude*sinf(frequency*i + offset_arg);
}

void unification_generators(float *res_gen, float *gen1, float *gen2, int size_res_gen, int size_gen1, int size_gen2, int OPERATION){
    for (int i = 0; i < size_res_gen; i++){
        if (size_gen1 > i && size_gen2 > i){
            switch (OPERATION)
            {
            case PLUS:
                *(res_gen + i) = gen1[i] + gen2[i];
                break;
            case MINUS:
                *(res_gen + i) = gen1[i] - gen2[i];
                break;
            case MULTIPLY:
                *(res_gen + i) = gen1[i] * gen2[i];
                break;
            default:
                *(res_gen + i) = 0;
                break;
            }
        }
        else 
            *(res_gen + i) = 0;
    }
}