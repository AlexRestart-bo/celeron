#include "main.h"

/* matrix должна иметь rows строк и cols рядов*/
int** transpose_matrix(int** matrix, int rows, int cols){
    int** new_matrix = (int**)malloc(rows*sizeof(int*));
    for (int i = 0; i < rows; i++){
        new_matrix[i] = (int*)malloc(cols*sizeof(int));
        if (new_matrix[i] == NULL){
            for (int j = i - 1; j >= 0; j--)
                free(new_matrix[j]);
            free(new_matrix);
            printf("Ошибка выделения памяти под новую матрицу\n");
            return NULL;
        }
    }
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++)
            new_matrix[j][i] = matrix[i][j];
    }
    return new_matrix;
}

// Узнаем лимит стека
void check_stack_limit(int depth) {
    char dummy;
    printf("Глубина: %d, Адрес переменной: %p\n", depth, &dummy);
    
    if (depth >= 0) {
        check_stack_limit(depth + 1);
    }
}

void print_matrix(int** matrix, int rows, int cols){
    printf("\nMatrix:");
    for (int i = 0; i < rows; i++){
        printf("\n");
        for (int j = 0; j < cols; j++)
            printf("%i\t", matrix[i][j]);
    }
}

void task12(){
    const int rows = 3;
    const int cols = 4;
    int mtx[rows][cols] = {
        {-1, -3, 0, 3},
        {5, 6, 0, 12},
        {-4, -9, 32, 11}
    };

    int** trmtx = transpose_matrix(&mtx[0][0], rows, cols);
    print_matrix(&mtx[0][0], rows, cols);
    print_matrix(trmtx, cols, rows);

    //for (int i = 0; i < cols)
}