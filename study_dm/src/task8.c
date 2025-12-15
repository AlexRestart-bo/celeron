#include "main.h"

Stack* create_stack(unsigned int init_capacity){
    Stack* st = (Stack*)malloc(sizeof(Stack));
    if(st == NULL){
        printf("Error with generating stack\n");
        return NULL;
    }

    st->array = (int*)malloc(init_capacity*sizeof(int));
    if(st->array == NULL){
        printf("Error with generating stack\n");
        free(st->array);
        free(st);
        return NULL;
    }

    st->capacity = init_capacity;
    st->size_array = 0;
    return st;
}

void push(Stack *st, int value){}

