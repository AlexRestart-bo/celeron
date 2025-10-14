#include <stdio.h>

char *word1 = "Precipitate";
char *word2 = "Consumption";
char *word3 = "Recognize";

void out_word(char *word);
void printf_words(void);

void printf_words(void){
    out_word(word1);
    printf("\taddr1 = %p\n", (void*)word1);
    out_word(word2);
    printf("\taddr1 = %p\n", (void*)word2);
    out_word(word3);
    printf("\taddr1 = %p\n", (void*)word3);
}

void test_str_1(char *word){
    char *begin = word;
    printf("begin word = %p\n", (void*)begin);
}

void test_str_2(char *word){
    char solo = 7;
    char *psolo = &solo;
    word = psolo;
    printf("begin word1 = %p\n", (void*)word);
    printf("%d\n", *(psolo + 9));
}

void out_word(char *word){
    char *wdpt = word;
    while(*wdpt != '\0'){
        printf("%c", *(wdpt++));
    }
}

