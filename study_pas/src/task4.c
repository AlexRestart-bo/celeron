#include "main.h"

Employee* create_employees_array(int init_size){
    Employee *worker = malloc(init_size*sizeof(Employee));
    if (worker == NULL) return NULL; 
    worker->len_name = 1;
    worker->name = malloc(worker->len_name*sizeof(char));
    if (worker->name == NULL) return NULL;
    worker->len_post = 1;
    worker->post = malloc(worker->len_post*sizeof(char));
    if (worker->post == NULL) return NULL;
    return worker;
}

void add_employee(Employee **collaborators, size_t *size_array){
    Employee *tmp = realloc(*collaborators, (*size_array + 1)*sizeof(Employee));
    if(tmp == NULL){
        printf("Не удалось увеличить размер массива (сейчас он состоит из %i элементов)", *size_array);
        return;
    }
    printf("Введите последовательно через пробел имя, \
        должность, зарплату и опыт (в днях) сотрудника\n");
    *collaborators = tmp;
    (*size_array)++;
    //при записи динамической строки последний символ вегда будет нуль-терминатором
    int len_inpst = 1;
    char* input_string = malloc(len_inpst*sizeof(char));
    if(input_string == NULL) return;
    //чтение вводимой пользователем строки
    read_line_dynamic(&input_string, &len_inpst);
    int j = 0;
    /**
     * field - номер заполняемого (!!! поля len_name и len_post не являются таковыми) поля структуры
     * Employee. Меняется в пределах [0, 4].
    */
    char field = 0;
    /**
     * within - внутренний счетчик для заполнения полей name и post в структуре Employee
    */
    char within;
    char *written_salary = malloc(sizeof(char));
    if (written_salary == NULL) return;

    char *written_expr = malloc(sizeof(char));
    if (written_expr == NULL) return;
    /**
     * переменные index_wrt_salary и index_wrt_expr означают лишь порядок в строковом массиве 
     * (0, 1, ..., index_wrt_salary) written_salary[] и written_expr[] соответственно. Значит,
     * размер этих массивов будет составлять index_wrt_salary + 1 и index_wrt_expr + 1 
     * соответственно.
    */
    char index_wrt_salary = 0;
    char index_wrt_expr = 0;
    /**
     * в цикле j < len_inpst && input_string[j] != '\0' первое условие выполняется вплоть до последнего
     * элемента j = len_inpst - 1, в то же время последним эллементом является нуль-терминатор, а
     * предспоследним символ ' ' (пробел) - см. функцию read_line_dinamic(). 
     * */ 
    
    while(j < len_inpst && input_string[j] != '\0'){
        if (input_string[j] == ' ') {
            field++;
            within = 0;
        }
        switch(field){
            case 0:
                if (within < 100){
                    char *tmp_name = realloc(collaborators[*size_array]->name, 
                        (collaborators[*size_array]->len_name + 1)*sizeof(char));
                    if (tmp_name == NULL) return;
                    collaborators[*size_array]->name = tmp_name;
                    collaborators[*size_array]->len_name++;
                    collaborators[*size_array]->name[within++] = input_string[j];
                } 
                break;
            case 1:
                if(within == 0) collaborators[*size_array]->name[collaborators[*size_array]->len_name - 1] = '\0';
                if (within < 100){
                    char *tmp_post = realloc(collaborators[*size_array]->post, 
                        (collaborators[*size_array]->len_post + 1)*sizeof(char));
                    if (tmp_post == NULL) return;
                    collaborators[*size_array]->post = tmp_post;
                    collaborators[*size_array]->len_post++;
                    collaborators[*size_array]->post[within++] = input_string[j];
                } 
                break;
            case 2:
                if(index_wrt_salary == 0)
                    collaborators[*size_array]->post[collaborators[*size_array]->len_post - 1] = '\0';
                if (index_wrt_salary < 9){
                    char *tmp_wrtsl = realloc(written_salary, (index_wrt_salary + 1)*sizeof(char));
                    if (tmp_wrtsl == NULL) return;
                    written_salary = tmp_wrtsl;
                    written_salary[index_wrt_salary++] = input_string[j];
                }
                break;
            case 3:
                if (index_wrt_expr < 9){
                    char *tmp_wrtex = realloc(written_expr, (index_wrt_expr + 1)*sizeof(char));
                    if (tmp_wrtex == NULL) return;
                    written_expr = tmp_wrtex;
                    written_expr[index_wrt_expr++] = input_string[j];
                }
                break;
            case 4:
                // небязательный шаг
                written_salary[index_wrt_salary] = '\0';
                written_expr[index_wrt_expr] = '\0';
                collaborators[*size_array]->salary = convert_str_int(written_salary, index_wrt_salary + 1);
                collaborators[*size_array]->experience = convert_str_int(written_expr, index_wrt_expr + 1);
                free(written_salary);
                free(written_expr);
                break;
        }
        j++;
    }
    free(input_string);
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

void remove_employee(Employee **collaborators, size_t *size_array, int index){}

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
