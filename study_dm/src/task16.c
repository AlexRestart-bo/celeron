#include "main.h"

static const int funcs = 4;

int add (int a, int b) {return a + b;}
int sub (int a, int b) {return a - b;}
int mul (int a, int b) {return a * b;}
int divide (int a, int b) {
    if (b == 0) return b;
    else return a / b;
}

void execute_operation(WordLt*  ord_sequence, ui sequence_size, Command* cmds){
    int a, b;
    for (int i = 0; i < sequence_size; i++){
        for (int j = 0; j < funcs; j++){
            if (isequalstr(ord_sequence[i].word, ord_sequence[i].wl, cmds[j].name, strlen(cmds[j].name))){
                RISING(i);
                a = string_to_int(ord_sequence[i].word, ord_sequence[i].wl);
                RISING(i);
                b = string_to_int(ord_sequence[i].word, ord_sequence[i].wl);
                printf("%i\n", cmds[j].function(a, b));
            }
        }
    }
}

Command* create_commands_set(void){
    Command* cmds = (Command*)malloc(funcs*sizeof(Command));
    if (cmds == NULL){
        msg_error_sellecting_mem("cmds");
        return NULL;
    }
    const int length_cmd = 4;
    for (int i = 0; i < funcs; i++){
        cmds[i].name = (char*)malloc(length_cmd*sizeof(char));
        if(!cmds[i].name){
            msg_error_sellecting_mem("cmds[i].name");
            while(--i >= 0) free(cmds[i].name);
            free(cmds);
        }
    }
    cmds[0].function = add;
    cmds[1].function = sub;
    cmds[2].function = mul;
    cmds[3].function = divide;
    strcpy(cmds[0].name, "add");
    strcpy(cmds[1].name, "sub");
    strcpy(cmds[2].name, "mul");
    strcpy(cmds[3].name, "div");
    return cmds;
}

void free_commands_set(Command* cmds){
    for (int i = 0; i < funcs; i++){
        free(cmds[i].name);
    }
    free(cmds);
}

void task16(void){
    printf("If you want to add two numbers, used constructions \"add {value1} {value2}\"\n"
    "If you want to subtract value2 from value1, used constructions \"sub {value1} {value2}\"\n"
    "If you want to multiply numbers, used constructions \"mul {value1} {value2}\"\n"
    "If you want to divide numbers, used constructions \"div {value1} {value2}\",\n"
    "where value1 and value2 are integer numbers\n"
    "You can to input all orders in the one line\n"
    "If you want to exit, print \"q\"\n");
    char* quit = "q";
    Command* cmds = create_commands_set();
    int length_line = 1;
    char *input_line = (char*)malloc(length_line*sizeof(char));
    char sb = ' ';      // разделительный символ между словами
    read_line(&input_line, &length_line);
    while (!isequalstr(input_line, length_line, quit, strlen(quit) + 1)){        // q - выход из программы.
        ui numw = 1;
        WordLt* set_words = cutting_string(input_line, &numw, length_line, sb);
        if(set_words == NULL) continue;
        execute_operation(set_words, numw, cmds);
        for (int i = 0; i < numw; i++) free(set_words[i].word);
        free(set_words);
        read_line(&input_line, &length_line);
    }
    free(input_line);
    free_commands_set(cmds);
}