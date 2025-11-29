#include "main.h"

static inline void task1(void);
static inline void task2(void);

int main(void){
    //task1();
    task2();
    return 0;
}

static inline void task1(void){
    int inpint = 0;
    int basic_size = 1;
    char* input = create_char_array(basic_size);
    read_line(&input, &basic_size);
    inpint = string_to_int(input, basic_size);
    free(input);
    task1_lowdiff(inpint);

    int basic_new = 1;
    char* new_input = malloc(basic_new*sizeof(char));
    read_line(&new_input, &basic_new);
    int nums_size = 1;
    int* nums_sequence = (int*)malloc(nums_size*sizeof(int));
    convert_series(&nums_sequence, &nums_size, new_input, basic_new, ' ');
    task1_meddiff(nums_sequence, nums_size);
    free(nums_sequence);
    free(new_input);
}

static inline void task2(void){
    char* cpprt = dup("What do you mean?");
    printf("Copy string = %s\n", cpprt);
}