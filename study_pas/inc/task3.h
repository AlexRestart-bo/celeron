#ifndef __TASK3_H
#define __TASK3_H

int* create_array(int initial_size);
void add_element(int **arr, int *size, int value);
void remove_element(int **arr, int *size, int index);
void prt_arr(int *arr, int size_arr);
void data_entry_row(int **primary, int *size_primary);
void read_line_dynamic(char **str, int *str_size);
int convert_str_int(char *str, int size);

#endif