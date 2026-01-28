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

static Person* create_person(const char* name, const char* email, int age){
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

static Person* copy_person(const Person* src){
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
static PersonsData* create_psdata(int init_cap){

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

static void add_person(PersonsData* pd, const char* name, const char* email, int age){
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
    if (!pd->data[pd->size].name) return NULL;
    pd->data[pd->size].email = (char*)malloc(strlen(name) + 1);
    if (!pd->data[pd->size].email) return NULL;
    strcpy(pd->data[pd->size].name, name);
    strcpy(pd->data[pd->size].email, email);
    pd->data[pd->size++].age = age;
    return;
}