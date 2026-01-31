#include <main.h>

typedef struct {
    char* name;
    char* email;
    int age;
} Person;

typedef struct {
    Person* data;
    int size;
    int capacity;
} PersonsData;

Person* create_person(const char* name, const char* email, int age){
    Person* pp = (Person*)malloc(sizeof(Person));
    if(!pp) return NULL;
    pp->name = (char*)malloc(strlen(name) + 1);
    if (!pp->name) {
        free(pp);
        return NULL;
    }
    pp->email = (char*)malloc(strlen(email) + 1);
    if(!pp->email){
        free(pp->name);
        free(pp);
        return NULL;
    }
    strcpy(pp->name, name);
    strcpy(pp->email, email);
    pp->age = age;
    return pp;
}

Person* copy_person(const Person* src){
    Person* pp = (Person*)malloc(sizeof(Person));
    if(!pp) return NULL;
    pp->name = (char*)malloc(strlen(src->name) + 1);
    if (!pp->name) {
        free(pp);
        return NULL;
    }
    pp->email = (char*)malloc(strlen(src->email) + 1);
    if(!pp->email){
        free(pp->name);
        free(pp);
        return NULL;
    }
    strcpy(pp->name, src->name);
    strcpy(pp->email, src->email);
    pp->age = src->age;
    return pp;
}

static void free_person(Person* person){
    free(person->name);
    free(person->email);
    free(person);
}

/**
 * @param init_cap должен быть больше или равен 1
*/
PersonsData* create_psdata(int init_cap){

    if (init_cap < 1) return NULL;

    PersonsData* pd = (PersonsData*)malloc(sizeof(PersonsData));
    if (!pd) return NULL;
    pd->data = (Person*)malloc(init_cap*sizeof(Person));
    if (!pd->data) {
        free(pd);
        return NULL;
    }
    pd->size = 0;
    pd->capacity = init_cap;
    
    for (int i = 0; i < init_cap; i++){
        pd->data[i].name = NULL;
        pd->data[i].email = NULL;
        pd->data[i].age = 0;
    }
    return pd;
}

void add_person(PersonsData* pd, const char* name, const char* email, int age){
    if(pd->size + 1 >= pd->capacity){       // если не хватает места меняем размер массива
        int new_cap = 2*pd->capacity;
        Person* tp = (Person*)realloc(pd->data, new_cap*sizeof(Person));
        if(!tp){
            printf("Не удалось добавить еще одного пользователя.\n" 
                "Число пользователей в настоящее время: %i", pd->size);
            return;
        }
        pd->data = tp;
        pd->capacity = new_cap;
    }
    pd->data[pd->size].name = (char*)malloc(strlen(name) + 1);
    if (!pd->data[pd->size].name) return;
    pd->data[pd->size].email = (char*)malloc(strlen(name) + 1);
    if (!pd->data[pd->size].email) return;
    strcpy(pd->data[pd->size].name, name);
    strcpy(pd->data[pd->size].email, email);
    pd->data[pd->size++].age = age;
    return;
}

void free_persons_data(PersonsData* pd){
    for (int i = 0; i < pd->size; i++){
        free_person(pd->data + i);
    }
    free(pd);
}

void print_data(PersonsData* pd){
    for (int i = 0; i < pd->size; i++)
        printf("Person %i\tName: %s\tEmail: %s\tAge: %i\n", i , pd->data[i].name, pd->data[i].email, pd->data[i].age);
}

void task10(){
    PersonsData* pd = create_psdata(4);
    add_person(pd, "Gall", "gallogreen@gmail.com", 25);
    add_person(pd, "Jura", "juratempest@remura.com", 24);
    add_person(pd, "Yamada", "yamada@navaho.com", 31);
    add_person(pd, "Jeratto", "jeratto@yasa.com", 29);
    add_person(pd, "Horim", "horim@muck.com", 27);
    print_data(pd);
}