#include "main.h"

Employee* create_employees_array(int init_size){
    Employee *worker = malloc(init_size*sizeof(Employee));
    if (worker == NULL) return -1; 
    return worker;
}

void add_employee(Employee **collaborators, size_t *size_array){
    Employee *tmp = realloc(*collaborators, (*size_array + 1)*sizeof(Employee));
    if(tmp == NULL){
        printf("Не удалось увеличить размер массива (сейчас он состоит из %i элементов)", *size_array);
        return;
    }
    *collaborators = tmp;
    (*size_array)++;
    char c;
    //всегда оставляем место для нуль-терминатора
    char name_counter = 1;
    //читаем имя до пробела
    while((c = getchar()) != ' ' && c != EOF && c != '\n'){
        char *name_tmp = realloc(collaborators[*size_array]->name, (++name_counter)*sizeof(char));
        if(name_tmp == NULL){
            collaborators[*size_array]->name[name_counter - 2] = '\0';
            collaborators[*size_array]->len_name = name_counter - 1;
            return;
        }
        collaborators[*size_array]->name = name_tmp;
        collaborators[*size_array]->name[name_counter - 2] = c;
    }
    collaborators[*size_array]->name[name_counter - 1] = '\0';
    collaborators[*size_array]->len_name = name_counter;
    //читаем дожность до пробела
    char post_counter = 1;
    while((c = getchar()) != ' ' && c != EOF && c != '\n'){
        char *post_tmp = realloc(collaborators[*size_array]->post, (++post_counter)*sizeof(char));
        if(post_tmp == NULL){
            collaborators[*size_array]->post[post_counter - 2] = '\0';
            collaborators[*size_array]->len_post = post_counter -1;
            return;
        }
        collaborators[*size_array]->post = post_tmp;
        collaborators[*size_array]->post[post_counter - 2] = c;
    }
    collaborators[*size_array]->post[post_counter - 1] = '\0';
    collaborators[*size_array]->len_post = name_counter;
    //после чтения числа как строки оно преобразуется к типу int
    char salary_counter = 1;
    char *salary_str = malloc(sizeof(char));
    while((c = getchar()) != ' ' && c != EOF && c != '\n'){
        char *salary_tmp = realloc(salary_str, (salary_counter++ + 1)*sizeof(char));
        if(salary_tmp == NULL){
            *(salary_str + salary_counter - 2) = '\0';
            return;
        }
        salary_str = salary_tmp;
        *(salary_str + salary_counter - 2) = c;
    }
    *(salary_str + salary_counter - 1) = '\0';
    collaborators[*size_array]->salary = convert_str_int(salary_str, salary_counter);
    free(salary_str);

    char exper_counter = 1;
    char *exper_str = malloc(sizeof(char));
    while((c = getchar()) != ' ' && c != EOF && c != '\n'){
        char *exper_tmp = realloc(exper_str, (exper_counter++ + 1)*sizeof(char));
        if(exper_tmp == NULL){
            *(exper_str + exper_counter - 2) = '\0';
            return;
        }
        exper_str = exper_tmp;
        *(exper_str + exper_counter - 2) = c;
    }
    *(exper_str + exper_counter - 1) = '\0';
    collaborators[*size_array]->experience = convert_str_int(exper_str, exper_counter);
    free(exper_str);
}


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

void remove_employee(Employee **collaborators, size_t *size_array, int index){

}

void search_by_name(){}

void sorting_by_salary(){}

void saiving_to_file(){}

void uploading_from_file(){}

void store_string(){}

void print_employees(Employee collaborators[], size_t size_arr){
    while(--size_arr){
        printf("Emp%i\tName: %s\tPost: %sSalary: %i, Experience: %i\n", (int)size_arr, *(collaborators)[size_arr].name, 
        *(collaborators)[size_arr].post, collaborators[size_arr].salary, collaborators[size_arr].experience);
    }
}
