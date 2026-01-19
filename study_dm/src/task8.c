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

void push(Stack *st, int value){
    if (st->size_array + 1 > st->capacity){
        int new_capacity = st->capacity*2;
        int *tpa = (int*)realloc(st->array, new_capacity*sizeof(int));
        if (tpa == NULL) return;
        st->array = tpa;
        st->capacity = new_capacity;
    }
    st->array[st->size_array++] = value;
}

int pop(Stack *st, int index){
    int ii = index;
    //push(st, 0);
    // индекс должен быть меньше, чем индекс нового добавленного в конец массива нуля (это делается для удобства)
    if (index < 0 || index >= st->size_array) return ii;
    else if (index == st->size_array - 1) {
        st->size_array--;
        return ii;
    }
    for (int i = index; i < st->size_array; i++) st->array[i] = st->array[i + 1];
    st->size_array--;
    return ii;
}

int peek(Stack *st, int index){
    if (index < 0 || index >= st->size_array) return 0;   
    return st->array[index];  
}

void is_empty(Stack *st){
    if (st->size_array == 0) printf("Stack is empty\n");
    else printf("Stack is not empty\n");
}

void print_stack(Stack *st){
    for (int i = 0; i < st->size_array; i++) printf("Index = %i\tValue = %i\n", i, st->array[i]); 
}

void task8_1(void){
    int error = NOERR;  // проверка на ошибку выделения памяти
    Stack *st = create_stack(4);
    int length_line = 1;
    char *input_line = (char*)malloc(length_line*sizeof(char));
    char sb = ' ';      // разделительный символ между словами
    while (read_line(&input_line, &length_line) != "q"){        // q - выход из программы.
        int place = 0;
        int spl = 0;
        // подсчет количества разделяющих символов (количества слов минус один)
        for (int i = 0; i < length_line; i++)
            if (input_line[i] == sb) spl++;
        if (spl == 0) break;
        // выделение памяти под количество слов, предполагается, что каждое слово имеет хотябы один символ
        char **sws = (char**)malloc(++spl*sizeof(char*));
        if (sws == NULL) break;
        for(int j = 0; j < spl; j++) {
            sws[j] = (char*)malloc(sizeof(char));
            if (sws[j] == NULL) {
                for (int k = j; k >= 0; k--) free(sws[k]);
                free(sws);
                error = ERR;
                break;
            }
        }
        if(error == ERR) break;
        // переменная place - местоположение текущего обрабатываемого символа в строке input_line
        for (int m = place; m < spl && input_line[m] != '\0'; m++){
            if (input_line[m] == sb) {

            }
        }
    }
    /*
    is_empty(st);
    push(st, 4567);
    push(st, 13);
    push(st, 57);
    push(st, -46);
    push(st, 0);
    push(st, -1);
    is_empty(st);
    print_stack(st);
    pop(st, 2);
    printf("pop(st, 0) = %i\n", pop(st, 0));
    print_stack(st);
    */
    free(st->array);
    free(st);
}

