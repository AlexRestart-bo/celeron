#ifndef TASK16_H
#define TASK16_H

typedef struct {
    char* name;
    int (*function)(int, int);
} Command;

void task16(void);

#endif