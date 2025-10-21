#ifndef __TASK4_H
#define __TASK4_H


enum posts{
    DIRECTOR = 0,
    ASSISTANT = 1,
    MANAGER = 2,
    ACCOUNTANT = 3,
    HEAD = 4,
    VICE = 5,
    DESIGNER = 6,
    DEVELOPER = 7,
    DOCTOR = 8,
    POLICMAN = 9,
    ENJINEER = 10,
    TECHNICIAN = 11
};

typedef struct {
    char *name;
    char *post;
    long salary;
    char experience;    //ages
    
} Employee;

#endif