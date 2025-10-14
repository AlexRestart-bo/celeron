#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "task2.h"

struct Student students[100];
int current_student = 0;

char noc(int num);
void read_line(char *str, int str_size);
int pow_int(int a, int p);


/**
 * @brief Добавляет нового студента в базу данных
 * 
 * @details Функция читает ввод пользователя, парсит данные и добавляет
 *          запись в массив students. Проверяет корректность ввода и
 *          обрабатывает ошибки.
 * 
 * @param[in] void Не принимает параметров
 * @return void Не возвращает значения
 * 
 * @note Максимальное количество студентов - 100
 * @warning Не thread-safe! Не использовать в многопоточной среде
 * 
 * @code
 * data_entry();  // Вызов функции
 * @endcode
 * 
 * @see data_output(), search_better()
 */

void data_entry(void){
    //Инициализация переменных нулями для предотвращения попадания мусора
    char mod = 0;
    char inp_st[100] = {0};
    char age[10] = {0};
    char average_whole[10] = {0};
    char average_fract[10] = {0};
    int remember = 0;
    char *endptr;
    float uncomp;
    printf("Enter the data without spaces: \n");
    //читаем данные, введенные пользователем
    read_line(inp_st, (int)(sizeof(inp_st)/sizeof(inp_st[0])));

    // Основной цикл парсинга - обрабатываем 5 полей:
    // 0: имя, 1: возраст, 2: целая часть балла, 3: дробная часть, 4: вычисления
    while (mod < 5){
        if (mod == 0){
            int i = 0;
            for (; remember + i < (int)sizeof(inp_st) && inp_st[remember + i] != '\0'; i++){
                if (inp_st[i + remember] == ' '){
                    mod++;
                    remember += i + 1;
                    break;
                }
                students[current_student].name[i] = inp_st[remember + i];
            }
            students[current_student].name[i] = '\0';    //необходимо каждую строку заканчивать нуль-терминатором
        }else if(mod == 1){
            int i = 0;
            for (; remember + i < (int)sizeof(inp_st) && inp_st[remember + i] != '\0'; i++){
                if (inp_st[i + remember] == ' '){
                    mod++;
                    remember += i + 1;
                    break;
                }
                age[i] = inp_st[remember + i];
            }
            age[i] = '\0';   //нуль-терминатор в конце строки
        }else if(mod == 2){
            students[current_student].age = (int)strtol(age, &endptr, 10);
            int i = 0;
            for (; remember + i < (int)sizeof(inp_st) && inp_st[remember + i] != '\0'; i++){
                if (inp_st[i + remember] == '.'){
                    mod++;
                    remember += i + 1;
                    break;
                }
                average_whole[i] = inp_st[remember + i];
            }
            average_whole[i] = '\0'; //нуль-терминатор в конце строки
        }else if(mod == 3){
            uncomp = (float)strtol(average_whole, &endptr, 10);
            int i = 0;
            for (; remember + i < (int)sizeof(inp_st); i++){
                if (inp_st[i + remember] == '\0'){
                    mod++;
                    remember += i + 1;
                    break;
                }
                average_fract[i] = inp_st[remember + i];
            } 
            average_fract[i] = '\0'; //нуль-терминатор в конце строки
        }else if(mod == 4){
            int fract_num = (int)strtol(average_fract, &endptr, 10);
            char num_char = noc(fract_num);
            uncomp += (float)fract_num/(float)(pow_int(10, (int)num_char));
            students[current_student].average_mark = uncomp;
            mod++;
        }
    }
    current_student++;
}

void data_output(void){
    for (int i = 0; i < current_student; i++){
        printf("Student %i :\t%s\t%d\t%f\n", i, students[i].name, students[i].age, students[i].average_mark);
    }
}

/*
 * Алгоритм поиска лучшего студента:
 * 1. Начинаем с предположения, что первый студент - лучший
 * 2. Последовательно сравниваем с остальными
 * 3. Обновляем индекс лучшего при нахождении большего балла
 * 
 * Сложность: O(n), где n - количество студентов
 */

void search_better(void){
    int better_stud = 0;
    for (int i = 1; i < current_student; i++){
        if (students[i].average_mark > students[better_stud].average_mark) better_stud = i;
    }
    printf("Student %i is better :\t%s\t%d\t%f\n", better_stud, students[better_stud].name, 
        students[better_stud].age, students[better_stud].average_mark);
}

/**
 * @brief Находит число цифр в числе типа int
 * 
 * @details Число делится по модулю, пока не останется лишь ноль
 * 
 * @param[in] int принимает число типа int
 * @return void Не возвращает значения
 * 
 */

char noc(int num){
    int runnum = num;
    if(runnum == 0) return 1;
    char num_char = 0;
    while(runnum != 0){
        runnum /= 10;
        num_char++;
    }
    return num_char;
}



void read_line(char *str, int str_size){
    int j = 0;
    char c;
    while((c = getchar()) != '\n' && c != EOF && j < str_size){
        *(str + j) = (char)c;
        j++;
    }
    *(str + j) = '\0';
}

int pow_int(int a, int p){
    int pi = 1;
    while(p > 0){
        pi *= a;
        p--;
    }
    return pi;
}