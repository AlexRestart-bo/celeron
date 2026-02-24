#include "main.h"

static int** create_triangular_array(int n){
    int** trar = (int**)malloc(n*sizeof(int*));
    if (trar == NULL){
        msg_error_sellecting_mem("trar in create_triangular_array() function");
        return NULL;
    }
    for (int i = 0; i < n; i++){
        trar[i] = (int*)malloc((i + 1)*sizeof(int));
        if (trar[i] == NULL){
            msg_error_sellecting_mem("trar[i]");
            while(i) free(trar[--i]);
            free(trar);
            return NULL;
        }
    }
    return trar;
}

static void fill_triangular_array(int** arr, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < i + 1; j++)
            arr[i][j] = ((i + j) % 2 == 0) ? i*j : -1*i*j;
    }
}

static void print_triangular_array(int** arr, int n){
    printf("Your triangular array:\n");
    for (int i = 0; i < n; i++){
        for (int j = 0; j < i + 1; j++)
            printf("%i\t", arr[i][j]);
        printf("\n");
    }
}

static void free_triangular_array(int** arr, int n){
    for (int i = 0; i < n; i++)
        free(arr[i]);
    free(arr);
}

void task15(void){
    const int n = 10;
    int** triangl = create_triangular_array(n);
    fill_triangular_array(triangl, n);
    print_triangular_array(triangl, n);
    free_triangular_array(triangl, n);
}
