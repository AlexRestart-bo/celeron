#ifndef TASK12_H
#define TASK12_H

typedef struct {
    int** nums;             // двумерный массив со строками разной длины
    int rows;               // количество строк в двумерном массиве nums[i]
    int cols;               // количество чисел в последней строке (текущей строке)
    int min_size_line;      // минимальная длина последовательности целых чисел в массиве nums
} Matrix;

void task12(void);
int** transpose_matrix(const int** matrix, int rows, int cols);
void check_stack_limit(int depth);

#endif