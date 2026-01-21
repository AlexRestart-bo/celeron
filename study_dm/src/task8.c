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

/**
 * numberWords >= 1
*/

WordLt** cutting_string(char* whole_string, ui* numberWords, ui str_size, char sb){
    ui place = 0;
    // выделение памяти под количество слов, предполагается, что каждое слово имеет хотябы один символ
    //ui numberWords = 1;
    WordLt** sw = (WordLt**)malloc((*numberWords)*sizeof(WordLt*));
    if (sw == NULL) return NULL;

    int n = 0;
    ui word_size = 1;
    sw[n]->word = (char*)malloc(word_size*sizeof(char));
    if(sw[n] == NULL) return sw;
    // переменная place - местоположение текущего обрабатываемого символа в строке input_line
    for (int m = place; m < str_size; m++){
        if (whole_string[m] == sb || whole_string[m] == '\0') {
            if (whole_string[m] == '\0') sw[n]->word[word_size - 1] = '\0';  // слово должно заканчиваться нуль-терминатором
            else {
                WordLt** addition = (WordLt**)realloc(sw, ++(*numberWords)*sizeof(WordLt*));
                if (addition == NULL) return sw;
                sw = addition;
                word_size = 1;      // заново начинаем счет букв для нового слова
                sw[++n]->word = (char*)malloc(word_size*sizeof(char));
                if (sw[n] == NULL) break;
                continue;
            }
        }
        char* yam = (char*)realloc(sw[n]->word, ++word_size*sizeof(char));
        if (yam == NULL) break;
        sw[n]->word = yam;
        sw[n]->word[word_size - 2] = whole_string[m];
    }
    return sw;
}

void execute_orders(WordLt**  ord_sequence, ui sequence_size, Stack* st){
    for (int i = 0; i < sequence_size; i++){
        if(ord_sequence[i]->word == "push") {
            i++;
            push(st, string_to_int(ord_sequence[i]->word, ord_sequence[i]->wl));
        }else if(ord_sequence[i]->word == "pop") {
            i++;
            pop(st, string_to_int(ord_sequence[i]->word, ord_sequence[i]->wl));
        }else if(ord_sequence[i]->word == "isempty") is_empty(st);
        else if(ord_sequence[i]->word == "print") print_stack(st); 
    }
}

void task8_1(void){
    Stack *st = create_stack(4);
    int length_line = 1;
    char *input_line = (char*)malloc(length_line*sizeof(char));
    char sb = ' ';      // разделительный символ между словами
    while (read_line(&input_line, &length_line) != "q"){        // q - выход из программы.
        ui numw = 1;
        WordLt** set_words = cutting_string(input_line, &numw, length_line, sb);
        if(set_words == NULL) continue;
        execute_orders(set_words,numw, st);
        for (int i = 0; i < numw; i++) free(set_words[i]->word);
        free(set_words);
    }
    free(input_line);
    free(st->array);
    free(st);
}

