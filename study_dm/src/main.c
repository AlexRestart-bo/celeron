#include "main.h"

static inline void task1(void);

int main(void){
    task1();
    return 0;
}

static inline void task1(void){
    int inpint = 0;
    int basic_size = 1;
    char* input = create_char_array(basic_size);
    read_line(&input, &basic_size);
    inpint = string_to_int(input, basic_size);
    free(input);
    //printf("%i\n", inpint);
    task1_lowdiff(inpint);
}