#ifndef TASK8_H
#define TASK8_H

typedef int ui;

typedef struct {
    int *array;
    int size_array;
    int capacity;
} Stack;

typedef struct {
    char* word;
    ui wl;  // word length
} WordLt;

void task8_1(void);

#endif