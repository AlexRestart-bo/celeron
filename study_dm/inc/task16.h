#ifndef TASK16_H
#define TASK16_H

#define RISING(i) if (++i >= sequence_size) return

typedef struct {
    char* name;
    int (*function)(int, int);
} Command;

void task16(void);

#endif