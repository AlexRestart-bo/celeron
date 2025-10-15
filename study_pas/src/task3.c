#include "main.h"

void read_line_dynamic(char **str, int *str_size);
int convert_str_int(char *str, int size);

int* create_array(int initial_size){
    if(initial_size <= 0) return NULL;
    int *parr = malloc(initial_size * (int)sizeof(*parr));
    return parr;
}

void add_element(int **arr, int *size, int value){
    int *temp = realloc(*arr, ((*size) + 1)*sizeof(int));
    if (temp == NULL) return;
    *arr = temp; 
    (*arr)[(*size)++ - 1] = value;
}

void remove_element(int *arr, int *size, int index){
    for(int i = index; i < (*size) - 1; i++){
        *(arr + i) = *(arr + i + 1);
    }
    //free(&arr[(*size) - 1]);
}

void union_arrays(void){}

void prt_arr(int *arr, int size_arr){
    for(int i = 0; i < size_arr - 1; i++){
        printf("%i ", arr[i]);
    }
    printf("%i\n", arr[size_arr - 1]);
}

/**
 * @brief Добавляет все числа введенные пользователем через пробел в массив чисел
 * 
 * @details Функция читает ввод пользователя, разделяет через пробел, затем преобразует
 *          в число и добавляет
 *          запись в массив primary. Проверяет корректность ввода и
 *          обрабатывает ошибки.
 * 
 * @param[in] int *primary - массив, куда будут сохраняться записанные числа, int size_primary
 *              - размер массива 
 * @return void Не возвращает значения
 * 
 * @note Массивы динамические
 * 
 */

void data_entry_row(int **primary, int *size_primary){
    int size_inp_st = 1;
    char *inp_st = malloc(size_inp_st*sizeof(char));
    if (inp_st == NULL) return;
    printf("Please, entry data!\n");
    read_line_dynamic(&inp_st, &size_inp_st);
    int j = 0;
    char *count = malloc(sizeof(char));
    if (count == NULL){
        free(inp_st);
        return;
    }
    int size_count = 1;
    int k = 0;
    while(inp_st[j] != '\0' && j < size_inp_st){
        if(inp_st[j] == ' '){       //Здесь делится строка на части по пробелу
            j++;
            count[k] = '\0';       //завершаем нуль-терминатором, чтобы функция преобразования нашла конец числа
            int realnum = convert_str_int(count, size_count);
            add_element(primary, size_primary, realnum);
            k = 0;      //особый счетчик для заполнения count[] с начала каждый раз, когда появляется пробел
        }else{          //если не хратает размера массива count[], то добавим память, иначе заполним уже существующую ячейку
            if(size_count < k + 1){
                char *temp = realloc(count, (size_count + 1)*sizeof(char));
                if(temp == NULL) {
                    free(count);
                    free(inp_st);
                    return;
                }
            count = temp;
                size_count++;
            }
            count[k] = inp_st[j];
            k++;
            j++;
        }
    }
    free(count);
    free(inp_st);
}

int convert_str_int(char *str, int size){
    int i = 0;
    char sn = 1;
    int res = 0;
    if(str[i] == '-'){
        sn*=-1;
        i++;
    }
    int c;
    while(str[i] != '\0' && i < size){
        if(( c = (int)(str[i] - '0')) > 0 && c < 9){
            res += c * pow_int(10, size - 1 - i);
        }else{
            printf("Error with input data\n");
            return 0;
        }
        i++;
    }
    return res*sn;
}

/*int pow_int(int a, int p){
    int pi = 1;
    while(p > 0){
        pi *= a;
        p--;
    }
    return pi;
}*/

/**
 * Функция на вход получает указатель на массив состоящий из одного единственного 
 *      элемента типа char. По мере выделения памяти этот элемент будет всегда находиться
 *      в конце массива. При завершении функции мы заменим его нуль-терминатором.
 */

void read_line_dynamic(char **str, int *str_size){
    int j = 0;
    char c;
    while((c = getchar()) != '\n' && c != EOF){
        char *temp = realloc(*str, (j + 1)*sizeof(char));
        if (temp == NULL){
            free(temp);
            return;
        }
        *str = temp;
        (*str_size)++;
        *((*str) + j) = (char)c;
        j++;    //остается еще один не занятый элемент
    }
    *((*str) + (*str_size) - 1) = '\0';      //ставим нуль-теминатор в конце
}