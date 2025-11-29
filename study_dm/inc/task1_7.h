#ifndef TASK1_7_H
#define TASK1_7_H

typedef struct {
    char* data;
    int data_size;
} StringData;

typedef struct {
    int size;
    StringData **string;
} StringDataArray;

char* create_char_array(int basic_size);
char* read_line(char** fill_str, int* size_str);
int string_to_int(char* input_str, int str_size);
int pow_int(int count, unsigned int degree);
void task1_lowdiff(int weight);
void task1_meddiff(int* nums_array, int array_size);
void convert_series(int**nums, int *size_nums, char* data_char, int size_data, char symbol);
char* dup(char* original);

#endif