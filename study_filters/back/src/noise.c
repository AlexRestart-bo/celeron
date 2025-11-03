#include "main.h"

void generate_noise(float *data, int size_data, float amplitude){
    srand(time(NULL));
    for (int i = 0; i < size_data; i++) data[i] = amplitude*((float)rand()/(float)RAND_MAX);
}