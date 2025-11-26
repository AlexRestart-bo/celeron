#include "main.h"


char* create_char_array(int basic_size){
    char* arr = malloc(basic_size*sizeof(char));
    return arr;
}

/**
 * читает одну строку (до символа перехода на новую строку)
 * и возвращает указатель на прочитанную строку
 * параметр size_str >= 1
*/

char* read_line(char** fill_str, int* size_str){
    if (*fill_str == NULL) return NULL;
    int c;
    while ((c = getchar())!='\n' && c != EOF && c != '\0')
    {
        *(*fill_str + *size_str - 1) = c;
        char *tmp_str = realloc(*fill_str, (++*size_str)*sizeof(char));
        if (tmp_str == NULL){
            *(*fill_str + *size_str - 2) = '\0';
            printf("Error with selecting memory\n");
            printf("%s\n", *fill_str);
            return *fill_str;
        }
    }
    *(*fill_str + *size_str - 1) = '\0';
    return *fill_str;
}

/**
 * преобразует строку в целое число
*/

int string_to_int(char* input_str, int str_size){
    int out_number = 0;
    int j = 0;
    int factor = 1;
    if(input_str[j] == '-') {
        factor*=-1;
        j++;
    }
    while(j < str_size && input_str[j] != '\0'){
        int num = input_str[j] - '0';       // АСКИ-коды цифр расположены подряд 
        if (num >= 0 && num <= 9){
            out_number = out_number*10 + num;
        }else{
            printf("Error with input data\n");
            return out_number;
        }
        j++;
    }
    return out_number;
}


/**
 * функция подходит только для возведения целых чисел в положительные целые степени
 * работает функция рекурсивно
*/
int pow_int(int count, unsigned int degree){
    if (degree == 0) return 1;
    else return count*pow_int(count, degree - 1);
}

/**
 * Функция записывает в динамический масив weight натуральных чисел
 * Записывает данные в файл "data/task1_out1" и выводит в терминал.
*/
void task1_lowdiff(int weight){       // low difficultty - низкая сложность  задания
    int* natural_nums = (int*)malloc(weight*sizeof(int));
    if (natural_nums == NULL) {
        printf("Error with selecting memory");
        return;
    } 
    for (int i = 0; i < weight; i++)
        *(natural_nums + i) = i + 1;

    FILE *filex;
    filex = fopen("data/task1_out1.txt", "w");
    if (filex == NULL) {
        free(natural_nums);
        return;
    }
    for (int i = 0; i < weight; i++) {
        if (i > 0 &&  i % 10 == 0) fprintf(filex, "\n");
        fprintf(filex, "%i\t", natural_nums[i]);
    }
    fprintf(filex, "\n");
    fclose(filex);

    for (int i = 0; i < weight; i++) {
        if (i > 0 && !(i % 10)) printf("\n");
        printf("%i\t", natural_nums[i]);
    }
    printf("\n");
    free(natural_nums);
}

void task1_meddiff(){          // medium difficulty - средняя сложность задания

}

StringData* create_string_data(char* data, int size_data){
    StringData* sd = (StringData*)malloc(sizeof(StringData));
    if(sd == NULL) {
        free(sd);
        return NULL;
    }
    sd->data = (char*)malloc(size_data*sizeof(char));
    if (sd->data == NULL){
        free(sd->data);
        free(sd);
        printf("Error with selecting memory\n");
        retun NULL;
    }
    strcpy(sd->data, data);
    sd->data_size = size_data;
    return sd;
}

StringDataArray* create_sda(int initial_capacity){
    StringDataArray* sda = (StringDataArray*)malloc(sizeof(StringDataArray));
    if(sda == NULL) return NULL;
    sda->string = malloc(initial_capacity*sizeof(StringData*));
    if(sda->string == NULL){
        free(sda->string);
        free(sda);
        return NULL;
    }
    sda->size = initial_capacity;
}

void add_single_str(StringDataArray** sda, char* data, int data_size){
    int new_size = (*sda)->size + 1;
    StringDataArray **new_sda = (StringDataArray*)realloc(sda, new_size*sizeof(StringDataArray*));
    if (new_sda == NULL) {
        printf("Occured error\n");
        return;
    }
    sda = new_sda;
    StringData *sd =  create_string_data(data, data_size);
    if (sd == NULL) return;
    (*sda)->string[(*sda)->size - 1] = sd;
}

void slice_string(){}