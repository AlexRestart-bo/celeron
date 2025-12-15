#include "main.h"

char* create_char_array(int basic_size){
    char* arr = malloc(basic_size*sizeof(char));
    return arr;
}

/**
 * Функция записывает в динамический масив weight натуральных чисел
 * Записывает данные в файл "data/task1_out1" и выводит в терминал.
*/
void task1_lowdiff(int weight){       // low difficultty - низкая сложность  задания
    int* natural_nums = (int*)malloc(weight*sizeof(int));
    if (natural_nums == NULL) {
        printf("Error with selecting memory");
        return;
    } 
    for (int i = 0; i < weight; i++)
        *(natural_nums + i) = i + 1;

    FILE *filex;
    filex = fopen("data/task1_out1.txt", "w");
    if (filex == NULL) {
        free(natural_nums);
        return;
    }
    for (int i = 0; i < weight; i++) {
        if (i > 0 &&  i % 10 == 0) fprintf(filex, "\n");
        fprintf(filex, "%i\t", natural_nums[i]);
    }
    fprintf(filex, "\n");
    fclose(filex);

    for (int i = 0; i < weight; i++) {
        if (i > 0 && !(i % 10)) printf("\n");
        printf("%i\t", natural_nums[i]);
    }
    printf("\n");
    free(natural_nums);
}

void task1_meddiff(int* nums_array, int array_size){        // medium difficulty - средняя сложность задания
    FILE *filex;
    filex = fopen("data/task1_out2.txt", "w");
    if (filex == NULL) return;
    for (int i = 0; i < array_size; i++) {
        if (i > 0 &&  i % 10 == 0) fprintf(filex, "\n");
        fprintf(filex, "%i\t", nums_array[i]);
    }
    fprintf(filex, "\n");
    fclose(filex);

    for (int i = 0; i < array_size; i++) {
        if (i > 0 && !(i % 10)) printf("\n");
        printf("%i\t", nums_array[i]);
    }
    printf("\n");
}

SpacePoints* create_space_points(int basic_size){
    SpacePoints* sppt = (SpacePoints*)malloc(sizeof(SpacePoints));
    if (sppt == NULL) return NULL;
    sppt->pt = (Point*)malloc(basic_size*sizeof(Point));
    if(sppt->pt == NULL) {
        free(sppt->pt);
        free(sppt);
        return NULL;
    }
    sppt->capacity = basic_size;
    sppt->counter = 0;
    return sppt;
}
// Добавляет точку в массив SpacePoints
void add_point_to_space(SpacePoints* sppt, int x, int y){
    if (sppt->capacity < sppt->counter + 1){
        int new_capacity = sppt->capacity*2;
        Point* tp = (Point*)realloc(sppt->pt, new_capacity*sizeof(Point));
        if (tp == NULL) return;
        sppt->pt = tp;
    }
    sppt->pt[sppt->counter].x = x;
    sppt->pt[sppt->counter].y = y;
    sppt->counter++;
}

float transfer(Point pt1, Point pt2){
    return sqrtf((float)((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y)));
}

void* mem_realloc_basic(void* ptr, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }
    
    void* new_ptr = malloc(new_size);
    if (new_ptr == NULL) {
        return NULL;
    }
    
    if (ptr != NULL) {
        // Копируем минимум из старого и нового размеров
        size_t copy_size = (old_size < new_size) ? old_size : new_size;
        memcpy(new_ptr, ptr, copy_size);
        free(ptr);
    }
    
    return new_ptr;
}
