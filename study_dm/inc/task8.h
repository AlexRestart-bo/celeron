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
bool isequalstr(char* s1, ui size1, char* s2, ui size2);
WordLt* cutting_string(char* whole_string, ui* numberWords, ui str_size, char sb);

#endif