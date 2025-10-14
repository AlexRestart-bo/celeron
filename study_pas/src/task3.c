#include "main.h"

int* create_array(int initial_size){
    if(initial_size <= 0) return NULL;
    int *parr = malloc(initial_size * (int)sizeof(*parr));
    return parr;
}

void add_element(int **arr, int *size, int *capacity, int value){}

void remove_element(int *arr, int *size, int index){}

void resize_array(int **arr, int *capacity);