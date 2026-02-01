#ifndef TASK11_H
#define TASK11_H

typedef struct {
    char** data;
    int size;
    int capacity;
} StringArray;

void add_string(StringArray* arr, const char* str);
void remove_string(StringArray* arr, int index);
int find_string(StringArray* arr, const char* str);
void task11(void);

#endif