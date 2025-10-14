#include <stdlib.h>
#include <stdio.h>

#include "main.h"

void test_mem_1(void){
    int *ptr = (int*)malloc(sizeof(int));
    if (ptr == NULL){
        printf("Error with selecting memory!\n");
    }
    *ptr = 56;
    printf("Output: %d", *ptr);
}

void read_input_text_to(char *str, char symb){
    int sz = 0;
    char c;  
    scanf("%c", &c);
    while (1){
        char *add_str = (char*)realloc((void*)str, (size_t)(sz + 1));
        if(add_str == NULL){
            printf("Error with selecting memory!\n");
            free(str);
            return;
        }
        str = add_str;
        if(c != symb){
            *(str + sz) = c;
            scanf("%c", &c);
            sz++;
        }else{
            *(str + sz) = '\0';
            break;
        }
    }
}