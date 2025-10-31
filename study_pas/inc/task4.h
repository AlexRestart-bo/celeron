#ifndef __TASK4_H
#define __TASK4_H


/*enum posts{
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
};*/

typedef struct {
    char *name;
    int len_name;
    char *post;
    int len_post;
    int salary;
    int experience;    //ages 
} Employee;

Employee* create_employees_array(int init_size);
void add_employee(Employee **collaborators, size_t *size_array);
void print_employees(Employee collaborators[], size_t size_arr);

#endif