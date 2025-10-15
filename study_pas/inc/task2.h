#ifndef __TASK2_H
#define __TASK2_H

/**
 * @brief Структура, представляющая студента
 * 
 * @var name - ФИО студента (максимум 50 символов)
 * @var age - Возраст студента (должен быть > 0)
 * @var average_mark - Средний балл (диапазон 0.0 - 5.0)
 */

struct Student {
    char name[50];
    int age;
    float average_mark; 
};

void data_entry(void);
void data_output(void);
void search_better(void);
int pow_int(int a, int p);

#endif