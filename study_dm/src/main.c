#include "main.h"

static inline void task1(void);
static inline void task2(void);
static inline void task3(void);
static inline void task4(void);
static inline void task5(void);

int main(void){
    task5();
    return 0;
}

static inline void task1(void){
    int inpint = 0;
    int basic_size = 1;
    char* input = create_char_array(basic_size);
    read_line(&input, &basic_size);
    inpint = string_to_int(input, basic_size);
    free(input);
    task1_lowdiff(inpint);

    int basic_new = 1;
    char* new_input = malloc(basic_new*sizeof(char));
    read_line(&new_input, &basic_new);
    int nums_size = 1;
    int* nums_sequence = (int*)malloc(nums_size*sizeof(int));
    convert_series(&nums_sequence, &nums_size, new_input, basic_new, ' ');
    task1_meddiff(nums_sequence, nums_size);
    free(nums_sequence);
    free(new_input);
}

static inline void task2(void){
    char* cpprt = dup("What do you mean?");
    printf("Copy string = %s\n", cpprt);
    free(cpprt);
}

static inline void task3(void){
    char* unst = union_string("Hello! ", "How are you?\n");
    printf("Union string = %s\n", unst);
    free(unst);
}

static inline void task4(void){
    printf("Введите через пробел два числа: количество строк и количество столбцов\n");
    int srd = 1;
    char* rd = (char*)malloc(srd*sizeof(char));
    read_line(&rd, &srd);
    int lw_size = 1;
    int *lw = (int*)malloc(lw_size*sizeof(int));
    convert_series(&lw, &lw_size, rd, srd, ' ');
    if (lw_size < 2) return;
    int rows = lw[0];
    int cols = lw[1];
        // Выделяем память для массива указателей
    int **arr = (int**)malloc(rows * sizeof(int*));
    if (arr == NULL) {
        free(lw);
        free(rd);
        return;
    }
    
    // Для каждой строки выделяем память для столбцов
    for (int i = 0; i < rows; i++) {
        arr[i] = (int*)malloc(cols * sizeof(int));
        if (arr[i] == NULL) {
            // Освобождаем уже выделенную память при ошибке
            for (int j = 0; j < i; j++) free(arr[j]);
            free(arr);
            free(lw);
            free(rd);
            return;
        }
    }

    for (int i = 0; i < rows; i++){
        read_line(&rd, &srd);
        convert_series(&lw, &lw_size, rd, srd, ' ');    
        if(lw_size < cols) {
            free(arr);
            free(lw);
            free(rd);
            return;
        }
        for (int j = 0; j < cols; j++) arr[i][j] = lw[j];
    }

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++) printf("%i\t", arr[i][j]);
        printf("\n");
    }
    free(arr);
    free(lw);
    free(rd);
}

static inline void task5(void){
    printf("Введите число точек\n");
    int srd = 1;
    char* rd = (char*)malloc(srd*sizeof(char));
    read_line(&rd, &srd);
    int lw_size = 1;
    int *lw = (int*)malloc(lw_size*sizeof(int));
    convert_series(&lw, &lw_size, rd, srd, ' ');
    if (lw_size < 1) return;
    int points_nums = lw[0];
    printf("points_nums = %i\n", points_nums);
    SpacePoints* stars = create_space_points(1);
    if (stars == NULL) return;
    for (int i = 0; i < points_nums; i++) {
        read_line(&rd, &srd);
        convert_series(&lw, &lw_size, rd, srd, ' ');
        if (lw_size < 2) {
            printf("Неверные данные\n");
            continue;
        }
        add_point_to_space(stars, lw[0], lw[1]);
    }
        // Выделяем память для массива указателей
    float **arr = (float**)malloc((stars->counter + 1) * sizeof(float*));
    if (arr == NULL) {
        free(lw);
        free(rd);
        return;
    }
    
    // Для каждой строки выделяем память для столбцов
    for (int i = 0; i < (stars->counter + 1); i++) {
        arr[i] = (float*)malloc((stars->counter + 1) * sizeof(float));
        if (arr[i] == NULL) {
            // Освобождаем уже выделенную память при ошибке
            for (int j = 0; j < i; j++) free(arr[j]);
            free(arr);
            free(lw);
            free(rd);
            return;
        }
    }
    printf("Numbers of the points = %i\n", (stars->counter + 1));

    for (int i = 0; i < (stars->counter + 1); i++){
        for (int j = 0; j < (stars->counter + 1); j++) arr[i][j] = transfer(stars->pt[i], stars->pt[j]);
    }

    for (int i = 0; i < (stars->counter + 1); i++){
        for (int j = 0; j < (stars->counter + 1); j++) printf("%4.2f\t", arr[i][j]);
        printf("\n");
    }
    for (int i = 0; i < (stars->counter + 1); i++) free(arr[i]);
    free(arr);
    free(lw);
    free(rd);
    free(stars->pt);
    free(stars);
}