#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "task1.h"

#define SIZE_YAMA 100
int yama[SIZE_YAMA];

void fill_array(int *ravage, ui size){
    /*  Массив заполняется случайными числами   */
    srand(time(NULL));
    for (ui i = 0; i < size; i++){
        ravage[i] = rand() % 1000;
    }
}

void search_min_max(int* complete_array, int* min, int* max, ui size_array){
    /*  Ищутся предельные значения в массиве    */
    *min = complete_array[0];
    *max = complete_array[0];
    ui i = 1;
    while (i < size_array){
        if (complete_array[i] < *min) *min = complete_array[i];
        else if (complete_array[i] > *max) *max = complete_array[i];
        i++;
    }
}

void sorted_enum(int* complete_array, ui size_array){
    /*  Сортируется массив в порядке возрастания    */
    int val;
    for (int i = 1; i < (int)size_array; i++){
        for (int j = i; j > 0; j--){
            if (complete_array[j] < complete_array[j - 1]){
                val = complete_array[j];
                complete_array[j] = complete_array[j - 1];
                complete_array[j - 1] = val;
            }else break;
        }
    }
}

int average(int complete_array[], ui size_array){
    /*  Подсчитывается среднее значение в массиве   */
    long accumulate = 0;
    for (ui i = 0; i < size_array; i++){
        accumulate += complete_array[i];
    }
    return (int)(accumulate / size_array);
}

void solonina(void){
    /*  Проверяет закон статистики  */
    int s;
    int iter = 100;
    for(int i = 0; i < iter; i++){
        fill_array(yama, SIZE_YAMA);
        s += 500 - average(yama, SIZE_YAMA);
    }
    s /= iter;
    s += 500;
    printf("Accumulate data %i\n", s);
}

