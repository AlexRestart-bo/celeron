#include "main.h"

void add_string(StringArray* arr, const char* str){
    if (arr->size >= arr->capacity){
        int new_cap = arr->capacity*2;
        char** ta = (char**)realloc(arr->data, new_cap*sizeof(char*));
        if (ta == NULL) {
            printf("Не удалось добавить новую строку в массив!\n");
            return;
        }
        arr->data = ta;
        arr->capacity = new_cap;
    }
    if (arr->data[arr->size] == NULL){
        arr->data[arr->size] = (char*)malloc((strlen(str) + 1)*sizeof(char));
        if (arr->data[arr->size] == NULL) {
            printf("Не удалось добавить новую строку в массив!\n");
            return;
        }
    }else{
        char* ts = (char*)realloc(arr->data[arr->size], (strlen(str) + 1)*sizeof(char));
        if (ts == NULL) {
            printf("Не удалось добавить новую строку в массив!\n");
            return;
        }
        arr->data[arr->size] = ts;
    }
    strcpy(arr->data[arr->size++], str);
}

/**
 *  Копирование строки производится из места arr->data[index1] в arr->data[index2]
 *  При копировании изменяемая строка может как увеличивать, так и уменьшать размер
*/
void copy_string(StringArray* arr, int index1, int index2){
    if (!arr || !arr->data) {
    printf("Некорректный массив\n");
    return;
    }
    if (index1 < 0 || index2 < 0 || index1 == index2 || index1 > arr->size - 1 || 
        index2 > arr->size - 1) return;   // проверка на дурака (если аргументы неправильные)
    if (arr->data[index1] == NULL){
        printf("Исходная строка со значением NULL!\n");
        return;
    }
    int len1 = strlen(arr->data[index1]) + 1;
    char* ts = (char*)realloc(arr->data[index2], len1*sizeof(char));
    if (ts == NULL){
        printf("Не удалось скопировать строку \"%s\" в строку \"%s\"\n", arr->data[index1], arr->data[index2]);
        return;
    }
    arr->data[index2] = ts;
    strcpy(arr->data[index2], arr->data[index1]);
}

void remove_string(StringArray* arr, int index){
    for (int i = index; i < arr->size - 1; i++){
        copy_string(arr, i + 1, i);
    }
    free(arr->data[arr->size - 1]);
    arr->data[arr->size - 1] = NULL;
    arr->size--;
}

int find_string(StringArray* arr, const char* str){
    for (int i = 0; i < arr->size; i++){
        if (!strcmp(arr->data[i], str)){
            printf("Number of finding string is %i\t%s\n", i, str);
            return i;
        }
    }
}

void print_starr(StringArray* arr){
    for (int i = 0; i < arr->size; i++){
        if (arr->data[i] != NULL)
            printf("%i.%s\n", i, arr->data[i]);
    }
}

void free_string_array(StringArray* arr){
    for (int i = 0; i < arr->size; i++){
        if (arr->data[i] != NULL)
            free(arr->data[i]);
    }
    free(arr);
}

void task11(){
    int init_size = 8;
    StringArray* arr = (StringArray*)malloc(sizeof(StringArray));
    if (!arr) return;
    *arr->data = (char*)malloc(init_size*sizeof(char));
}