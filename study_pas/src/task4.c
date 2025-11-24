#include "main.h"


/**
 * Удаление сотрудника осуществляется следующим образом:
 *  1. Для каждого поля post и name из структуры Employee происходит сравнение между соседними
 *  (collaborators[index] и collaborators[index + 1]) элементами массива.
 *  2. Пока порядок символа из строки name элемента collaborators[index + 1] 
 *  не превосходит порядок символа из строки элемента collaborators[index], строка collaborators[index].name
 *  будет перезаписываться. Как только длины перестанет хватать, начнется динамическое выделение для
 *  поля collaborators[index].name. Если collaborators[index].len_name > collaborators[index+1].len_name, 
 *  то будет вызвана функция, уменьшающая динамическую память - realloc().
 *  3. Ровно те же действия потребуются и для поля post.
 *  4. В случае полей salary и experience прозводится копирование из соседнего элемента
 *  5. Все эти действия повторяются в цикле до тех пор, пока последний элемент массива
 *  не запишется в предпоследний. Затем размер массива collaborators[] уменьшится на единицу
 *  с помощью realloc().
 */

//void remove_employee(Employee **collaborators, size_t *size_array, int index){}

void search_by_name(){}

void sorting_by_salary(){}

void saiving_to_file(){}

void uploading_from_file(){}

void store_string(){}

/*void print_employees(Employee collaborators[], size_t size_arr){
    while(--size_arr){
        printf("Emp%i\tName: %s\tPost: %sSalary: %i, Experience: %i\n", (int)size_arr, *(collaborators)[size_arr].name, 
        *(collaborators)[size_arr].post, collaborators[size_arr].salary, collaborators[size_arr].experience);
    }
}*/

Employee* create_worker(char *name, int len_name, char *post, int len_post, int sal, int exp){
    Employee *freeloader = malloc(sizeof(Employee));
    if(freeloader == NULL) return NULL;

    freeloader->name = (char*)malloc(len_name);
    freeloader->post = (char*)malloc(len_post);
    if(freeloader->name == NULL || freeloader->post == NULL){
        free(freeloader->post);
        free(freeloader->name);
        free(freeloader);
        return NULL;
    }

    strcpy(freeloader->name, name);
    strcpy(freeloader->post, post);

    freeloader->len_name = len_name;
    freeloader->len_post = len_post;
    freeloader->salary = sal;
    freeloader->experience = exp;

    return freeloader;
}

void write_worker(Employee *emp){
    printf("%s\t%s\t%i\t%i\n", emp->name, emp->post, emp->salary, emp->experience);
}

Employee* copy_dinamic(void){
    //при записи динамической строки последний символ вегда будет нуль-терминатором
    int len_inpst = 1;
    char* input_string = malloc(len_inpst*sizeof(char));
    if(input_string == NULL) return NULL;
    //чтение вводимой пользователем строки
    read_line_dynamic(&input_string, &len_inpst);
    int j = 0;
    char field = 0;
    printf("%s\n", input_string);
    /**
     * within - внутренний счетчик для заполнения полей name и post в структуре Employee
    */
    char within = 0;
    /**
     * в цикле j < len_inpst && input_string[j] != '\0' первое условие выполняется вплоть до последнего
     * элемента j = len_inpst - 1, в то же время последним эллементом является нуль-терминатор, а
     * предспоследним символ ' ' (пробел) - см. функцию read_line_dinamic(). 
     * */ 
    int bs = 1;
    char *beaty_str = malloc(bs);
    if (beaty_str == NULL) return NULL;

    int sp = 1;
    char *second_part = malloc(sp);
    if(second_part == NULL) return NULL;

    int tp = 1;
    char *third_part = malloc(tp);
    if (third_part == NULL) return NULL;

    int fp = 1;
    char *fourth_part = malloc(fp);
    if(fourth_part == NULL) return NULL;

    while(j < len_inpst && input_string[j] != '\0'){
        if (input_string[j] == ' ') {
            j++;
            field++;
            within++;
        }
        switch(field){
            case 0:
                bs++;
                char *tmp_beaty = realloc(beaty_str, bs);     // оставляем последний байт для нуль-терминатора
                if (tmp_beaty == NULL) return NULL;
                beaty_str = tmp_beaty;
                *(beaty_str + bs - 2) = input_string[j];
                break;
            case 1:
                beaty_str[bs - 1] = '\0';
                char *tmp_second = realloc(second_part, ++sp);     // оставляем последний байт для нуль-терминатора
                if (tmp_second == NULL) return NULL;
                second_part = tmp_second;
                *(second_part + sp - 2) = input_string[j];
                break;
            case 2:
                second_part[sp - 1] = '\0';
                char *tmp_third = realloc(third_part, ++tp);     // оставляем последний байт для нуль-терминатора
                if (tmp_third == NULL) return NULL;
                third_part = tmp_third;
                *(third_part + tp - 2) = input_string[j];
                break;
            case 3:
                third_part[tp - 1] = '\0';
                fp++;
                char *tmp_fourth = realloc(fourth_part, fp);     // оставляем последний байт для нуль-терминатора
                if (tmp_fourth == NULL) return NULL;
                fourth_part = tmp_fourth;
                *(fourth_part + fp - 2) = input_string[j];
                break;
            case 4:
                fourth_part[fp - 1] = '\0';
                break;
        }
        j++;
    }
    free(input_string);
    Employee *freeloader = create_worker(beaty_str, bs, second_part, sp, convert_str_int(third_part, tp),
            convert_str_int(fourth_part, fp));
    free(beaty_str);
    free(second_part);
    free(third_part);
    free(fourth_part);
    return freeloader;
}

void free_employee(Employee *slave){
    if (slave != NULL){
        free(slave->name);
        free(slave->post);
        free(slave);
    }
}

EmployeesArray* create_empl_array(int initial_capacity) {
    EmployeesArray *arr = (EmployeesArray*)malloc(sizeof(EmployeesArray));
    if (arr == NULL) return NULL;

    arr->data = (Employee**)malloc(initial_capacity*sizeof(Employee*));
    if (!arr->data) {
        free(arr);
        return NULL;
    }

    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

int add_employee(EmployeesArray *arr, char *name, int name_size, char *post, int size_post, int slr, int expr){
    if (arr->size >=arr->capacity){
        int new_capacity = arr->capacity*2;
        Employee **new_data = (Employee**)realloc(arr->data, new_capacity*sizeof(Employee*));
        if (!new_data) return 0;

        arr->data = new_data;
        arr->capacity = new_capacity;
    }

    Employee *new_empl = create_worker(name, name_size, post, size_post, slr, expr);
    if (!new_empl) return 0;

    arr->data[arr->size++] = new_empl;
}

Employee* get_employee(const EmployeesArray *arr, int index){
    if (index < 0 || index >= arr->size) return NULL;
    return arr->data[index];
}