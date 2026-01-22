#include <stdio.h>
#include <stdlib.h>

#include "inout.h"
#include "memory.h"
#include "tests.h"

#define part2

int main(void){

    #ifdef part1

    char *str = (char*)malloc(sizeof(char));
    if (str == NULL){
        free(str);
        return 0;
    }

    read_input_text_to(str, '\n');

    
    printf("%s", str);
    free(str);

    #endif

    #ifdef part2

    /*ts1 mer;
    change_struct_1(mer);
    return_struct(mer);*/

    #endif

    return 0;
}