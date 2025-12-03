#ifndef ASSISTFUNCS_H
#define ASSISTFUNCS_H

char* read_line(char** fill_str, int* size_str);
int string_to_int(char* input_str, int str_size);
int pow_int(int count, unsigned int degree);
void convert_series(int**nums, int *size_nums, char* data_char, int size_data, char symbol);
char* dup(char* original);
char* union_string(char* s1, char* s2);

#endif