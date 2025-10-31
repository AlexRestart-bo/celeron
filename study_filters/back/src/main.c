#include <stdio.h>
#include "main.h"

sync_detector_t det;

int main(void){
    float input_data[DATA_SIZE];
    float output_data[DATA_SIZE];
    float phases_output[DATA_SIZE];

    generate_signal(input_data, DATA_SIZE);
    sync_detector_init(&det, REF_FREQ, FS, ALPHA);
    for (int i = 0; i < DATA_SIZE; i++){
        sync_detector_process(&det, input_data[i], output_data + i, phases_output + i);
    }

    FILE *file;
    file = fopen("out.txt", "w");
    if (file == NULL){
        return 0;
    }

    for (int i = 0; i < DATA_SIZE; i++) fprintf(file, "%i\t%f\t%f\t%f\n", i + 1, input_data[i], output_data[i], phases_output[i]);

    fclose(file);

    return 0;
}