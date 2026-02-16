#include "main.h"

// Массивы, поступающие на вход должны быть отсортированы в порядке неубывания
int* merge_sorted_arrays(int* arr1, int size1, int* arr2, int size2, int* result_size){
    int counter1 = 0;
    int counter2 = 0;
    if (size1 < 1 || size2 < 0) return NULL;
    int size_union = size1 + size2;
    int* sorted_array = (int*)malloc(size_union*sizeof(int));
    if (sorted_array == NULL){
        msg_error_sellecting_mem("sorted_array");
        return NULL;
    }
    for (int i = 0; i < size_union; i++){
        if (counter1 < size1 && counter2 < size2)
            sorted_array[i] = (arr1[counter1] <= arr2[counter2]) ? arr1[counter1++] : arr2[counter2++];
        else if (counter1 < size1)
            sorted_array[i] = arr1[counter1++];
        else if (counter2 < size2)
            sorted_array[i] = arr2[counter2++];
        else
            break;
        *result_size = i + 1;
    }
    return sorted_array;
}

void print_array(int* arr, int size){
    printf("Your array:\n");
    for (int i = 0; i < size; i++)
        (i > 0 && i % 10 == 0) ? printf("\n%i\t", arr[i]) : printf("%i\t", arr[i]);
}

void task13(void){

    int arr1[5] = {-4, -1, 2, 2, 7};
    int arr2[7] = {-2, 0, 3, 4, 4, 7, 9};

    int result_size = 0;
    int* sr = merge_sorted_arrays(arr1, 5, arr2, 7, &result_size);
    if (sr == NULL) return;
    print_array(sr, result_size);
}
