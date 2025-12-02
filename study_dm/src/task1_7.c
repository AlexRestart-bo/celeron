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
    int i = 0;      // - number of the symbol
    int c;
    while ((c = getchar())!='\n' && c != EOF && c != '\0'){
        //*(*fill_str + *size_str - 1) = c;
        *(*fill_str + i) = c;
        i++;
        if (i + 2 > *size_str){
            char *tmp_str = realloc(*fill_str, (++*size_str)*sizeof(char));
            if (tmp_str == NULL){
                *(*fill_str + *size_str - 2) = '\0';
                printf("Error with selecting memory\n");
                printf("%s\n", *fill_str);
                return *fill_str;
            }
            *fill_str = tmp_str;
        }
    }
    *(*fill_str + i) = '\0';
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

void task1_meddiff(int* nums_array, int array_size){        // medium difficulty - средняя сложность задания
    FILE *filex;
    filex = fopen("data/task1_out2.txt", "w");
    if (filex == NULL) return;
    for (int i = 0; i < array_size; i++) {
        if (i > 0 &&  i % 10 == 0) fprintf(filex, "\n");
        fprintf(filex, "%i\t", nums_array[i]);
    }
    fprintf(filex, "\n");
    fclose(filex);

    for (int i = 0; i < array_size; i++) {
        if (i > 0 && !(i % 10)) printf("\n");
        printf("%i\t", nums_array[i]);
    }
    printf("\n");
}

/**
 * Функция принимает строку, в которой набор числел, разделенных символом symbol.
 * nums - динамический массив, куда записываются числа, выделенные и преобразованные 
 * в тип int из строковой последовательности символов (char). size_nums - размер
 * массива nums, причем необходимо, чтобы на входе он был size_nus >= 1. 
*/

void convert_series(int**nums, int *size_nums, char* data_char, int size_data, char symbol){
    int i = 0;
    int how_long = 0;
    if (nums == NULL) return;
    int weight_num = 1;
    char* strnum = malloc(weight_num*sizeof(char));
    if (strnum == NULL) return;
    /** Счетчик counter используется для записи символов в строку strnum, которая перезаписывается
     * каждый раз после встречи symbol в строке data_char.
    */
    int counter = 0;
    while(i < size_data){
        // В конце строки обычно не ставят пробел, поетому добавлена проверка на нуль-терминатор,
        // чтобы не отбросить последнее число.
        if (data_char[i] == symbol || data_char[i] == '\0') {
            // добавляем память, только если нехватает 
            if(how_long >= *size_nums) {
                int* tmp_nums = (int*)realloc(*nums, ++(*size_nums)*sizeof(int)); 
                if (tmp_nums == NULL) return;
                *nums = tmp_nums;
            }
            (*nums)[how_long++] = string_to_int(strnum, counter);
            counter = 0;
        }else{
            if (counter < weight_num -1){
                char* tmp = realloc(strnum, weight_num++*sizeof(char)); 
                if(tmp == NULL) return;
                strnum = tmp;
            }
            strnum[counter++] = data_char[i];
        }
        i++;
    }
}
/**
 * Функция принимает в качестве аргумента строку и возвращает указатель на начало копии строки 
 * Копирование происходит в динамически.
*/
char* dup(char* original){
    int counter = strlen(original);
    printf("%s\t%i\n", original, counter);
    int cps = 3;
    char* cp = malloc(cps*sizeof(char));
    for (int i = 0; i < counter; i++){
        if(i + 1 > cps){
            char* tmp = (char*)realloc(cp, ++cps*sizeof(char));
            if (tmp == NULL) return NULL;
        }
        cp[i] = original[i];
    }
    return cp;
}
// Функция принимает две строки, последовательео их объединяет и возвращает указатель на общую строку
char* union_string(char* s1, char* s2){
    if (s1 == NULL || s2 == NULL) return NULL;
    int ss1 = strlen(s1);
    int ss2 = strlen(s2);
    char* unst = (char*)malloc((ss1 + ss2)*sizeof(char));
    for (int i = 0; i < ss1; i++) unst[i] = s1[i];
    for (int i = 0; i < ss2; i++) unst[ss1 + i] = s2[i];
    return unst;
}

SpacePoints* create_space_points(int basic_size){
    SpacePoints* sppt = (SpacePoints*)malloc(sizeof(SpacePoints));
    if (sppt == NULL) return NULL;
    sppt->pt = (Point*)malloc(basic_size*sizeof(Point));
    if(sppt->pt == NULL) {
        free(sppt->pt);
        free(sppt);
        return NULL;
    }
    sppt->capacity = basic_size;
    sppt->counter = 0;
    return sppt;
}
// Добавляет точку в массив SpacePoints
void add_point_to_space(SpacePoints* sppt, int x, int y){
    if (sppt->capacity < sppt->counter + 1){
        int new_capacity = sppt->capacity*2;
        Point* tp = (Point*)realloc(sppt->pt, new_capacity*sizeof(Point));
        if (tp == NULL) return;
        sppt->pt = tp;
    }
    sppt->pt[sppt->counter++].x = x;
    sppt->pt[sppt->counter++].y = y;
}

float transfer(Point pt1, Point pt2){
    return sqrtf((float)((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y)));
}