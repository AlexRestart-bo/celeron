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
 * @brief Функция разрезает строку на части по разделительному символу sb
 * @param numberWords >= 1
*/

WordLt* cutting_string(char* whole_string, ui* numberWords, ui str_size, char sb){
    WordLt* sw = (WordLt*)malloc((*numberWords)*sizeof(WordLt));    // выделение памяти под количество слов, предполагается, что каждое слово имеет хотябы один символ
    if (sw == NULL) {
        printf("Error with selecting memory in function cutting_string()\n");
        return NULL;
    }
    int n = 0;
    ui word_size = 1;
    sw[n].word = (char*)malloc(word_size*sizeof(char));
    if(sw[n].word == NULL) return sw;
    // переменная place - местоположение текущего обрабатываемого символа в строке input_line
    for (int m = 0; m < str_size; m++){
        if (whole_string[m] == '\0') {
            sw[n].word[word_size - 1] = '\0';  // слово должно заканчиваться нуль-терминатором
            sw[n].wl = word_size - 1;
            break;
        }else if (whole_string[m] == sb) {
            WordLt* addition = (WordLt*)realloc(sw, ++(*numberWords)*sizeof(WordLt));
            if (addition == NULL) return sw;
            sw = addition;
            sw[n].wl = word_size - 1;                               // при вычислении длины строки функцией strlen() нуль-терминатор не считается
            sw[n].word[word_size - 1] = '\0';                       // однако все равно нужно закончить нуль-терминатором
            word_size = 1;                                          // заново начинаем счет букв для нового слова
            sw[++n].word = (char*)malloc(word_size*sizeof(char));   // выделяем память под новое слово (снала для одного символа)
            if (sw[n].word == NULL) {
                printf("Error with memory selecting (in cutting_string())\n");
                break;
            }
            continue;
        }
        char* yam = (char*)realloc(sw[n].word, ++word_size*sizeof(char));
        if (yam == NULL) {
            printf("Error with memory selecting\n");
            break;
        }
        sw[n].word = yam;
        sw[n].word[word_size - 2] = whole_string[m];
    }
    return sw;
}

// Важно: если для вычисления размера строки используется strlen(), то нуль-терминатор не засчитывается
bool isequalstr(char* s1, ui size1, char* s2, ui size2){  
    if (size1 == size2){
        for (int i = 0; i < size1; i++) {
            if(s1[i] != s2[i]) return false;
        }
    }else return false;
    return true;
}

void execute_orders(WordLt*  ord_sequence, ui sequence_size, Stack* st){
    char* pushc = "push";
    char* popc = "pop";
    char* isemptyc = "isempty";
    char* printc = "print";

    for (int i = 0; i < sequence_size; i++){
        if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, pushc, strlen(pushc))){
            i++;
            push(st, string_to_int(ord_sequence[i].word, ord_sequence[i].wl));
        }else if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, popc, strlen(popc))) {
            i++;
            pop(st, string_to_int(ord_sequence[i].word, ord_sequence[i].wl));
        }else if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, isemptyc, strlen(isemptyc))) is_empty(st);
        else if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, printc, strlen(printc))) print_stack(st); 
    }
}

void task8_1(void){
    printf("If you want to push number in stack, used constructions \"push {value}\", where value is integer number\n"
    "If you want to pop number in stack, used constructions \"pop {index}\", where index is index of item in stack\n"
    "If you want to print stack, used constructions \"print\"\n"
    "If you want to check stack is empty, used constructions \"isempty\"\n"
    "You can to input all orders in the one line\n"
    "If you want to exit, print \"q\"\n");
    char* quit = "q";
    Stack *st = create_stack(4);
    int length_line = 1;
    char *input_line = (char*)malloc(length_line*sizeof(char));
    char sb = ' ';      // разделительный символ между словами
    read_line(&input_line, &length_line);
    while (!isequalstr(input_line, length_line, quit, strlen(quit) + 1)){        // q - выход из программы.
        ui numw = 1;
        WordLt* set_words = cutting_string(input_line, &numw, length_line, sb);
        if(set_words == NULL) continue;
        execute_orders(set_words,numw, st);
        for (int i = 0; i < numw; i++) free(set_words[i].word);
        free(set_words);
        read_line(&input_line, &length_line);
    }
    free(input_line);
    free(st->array);
    free(st);
    
}

