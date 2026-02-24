#include "main.h"

int add (int a, int b) {return a + b;}
int sub (int a, int b) {return a - b;}
int mul (int a, int b) {return a * b;}
int div (int a, int b) {
    if (b == 0) return b;
    else return a / b;
}

int execute_operation(WordLt*  ord_sequence, ui sequence_size, Stack* st){
    char* addc = "add";
    char* subc = "sub";
    char* mulc = "mul";
    char* divc = "div";

    for (int i = 0; i < sequence_size; i++){
        if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, addc, strlen(addc))){
            i++;
            push(st, string_to_int(ord_sequence[i].word, ord_sequence[i].wl));
        }else if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, subc, strlen(popc))) {
            i++;
            pop(st, string_to_int(ord_sequence[i].word, ord_sequence[i].wl), 
                    string_to_int(ord_sequence[i + 1].word, ord_sequence[i].wl));
        }else if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, isemptyc, strlen(isemptyc))) is_empty(st);
        else if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, printc, strlen(printc))) print_stack(st); 
    }
}

Command* create_commands_set(void){
    
}

void task16(void){
    int (*operation) (int, int);
    printf("If you want to add two numbers, used constructions \"add {value1} {value2}\"\n"
    "If you want to subtract value2 from value1, used constructions \"sub {value1} {value2}\"\n"
    "If you want to multiply numbers, used constructions \"mul {value1} {value2}\"\n"
    "If you want to divide numbers, used constructions \"div {value1} {value2}\",\n"
    "where value1 and value2 are integer numbers\n"
    "You can to input all orders in the one line\n"
    "If you want to exit, print \"q\"\n");
    const char* quit = "q";
    Command* cmds = create_commands_set();
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
}