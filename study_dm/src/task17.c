#include "main.h"

Cache* create_cache(int init_capacity){
    if (init_capacity < 1) {
        printf("Incorrect argument \"init_capacity\"\n");
        return NULL;
    }
    Cache* scache = (Cache*)malloc(sizeof(Cache));
    if (scache == NULL){
        msg_error_sellecting_mem("scache");
        return NULL;
    }
    scache->entries = (CacheEntry*)malloc(init_capacity*sizeof(CacheEntry));
    if(!scache->entries) {
        msg_error_sellecting_mem("scache->entries");
        free(scache);
        return NULL;
    }
    scache->capacity = init_capacity;
    scache->size = 0;
    scache->head = 0;
    for (int i = 0; i < scache->capacity; i++)
        scache->entries[i].key = NULL;
    return scache;
}

void add_item_to_cache(Cache* scache, const char* key, const int value){
    bool is_busy = false;           // проверка была ли занята ячейка до записи новых значений
    if(scache->entries[scache->head].key != NULL){
        is_busy = true;
        free(scache->entries[scache->head].key);
        scache->entries[scache->head].key = NULL;
    }
    scache->entries[scache->head].key = malloc(strlen(key)+1);
    if (!scache->entries[scache->head].key){
        msg_error_sellecting_mem("key");
        return;
    }
    strcpy(scache->entries[scache->head].key, key);
    scache->entries[scache->head].value = value;
    scache->head = (scache->head + 1) % scache->capacity;
    if(!is_busy) scache->size++;
}

/*  Функция возвращает индекс найденной совокупности в кольцевом буфере scache->entries[]   */
int search_item(Cache* scache, const char* key){
    for (int i = ((scache->head - 1 + scache->capacity) % scache->capacity); 
        i < ((scache->head + 1 - scache->size + scache->capacity) % scache->capacity); i++){
        if(isequalstr(key, strlen(key), scache->entries[i].key, strlen(scache->entries[i].key)))
            return i;
    }
    return -1;
}

void free_cache(Cache* scache){
    for (int i = 0; i < scache->capacity; i++){
        if(scache->entries[i].key != NULL)
            free(scache->entries[i].key);
    }
    for (int i = 0; i < scache->capacity; i++)
        free(scache->entries);
    free(scache);
}

void write_cache(Cache* scache){
    for (int i = scache->size - 1; i >= 0; i--){
        int j = ((scache->head - i + scache->capacity) % scache->capacity);
        if (j >= 0 && j < scache->capacity)
            printf("Item %i\tKey = %s\tValue = %i\n", j, scache->entries[j].key, scache->entries[j].value);
    }
}

void write_cache_item(Cache* scache, int index){
    if (index >= 0 && index < scache->capacity)
        printf("Item %i\tKey = %s\tValue = %i\n", index, scache->entries[index].key, scache->entries[index].value);
}

void task17(void){
    Cache* cc = create_cache(10);
    add_item_to_cache(cc, "ito", 83);
    add_item_to_cache(cc, "kato", 11);
    add_item_to_cache(cc, "loto", -23);
    add_item_to_cache(cc, "tito", 45);
    add_item_to_cache(cc, "katsu", 732);
    add_item_to_cache(cc, "mokko", 902);
    add_item_to_cache(cc, "intilo", 1);
    write_cache(cc);

    add_item_to_cache(cc, "kioto", 25);
    add_item_to_cache(cc, "nato", -14);
    add_item_to_cache(cc, "kabane", 0);
    add_item_to_cache(cc, "dzidai", -5);
    add_item_to_cache(cc, "ukura", 89);
    add_item_to_cache(cc, "mijure", 19);
    write_cache(cc);
    write_cache_item(cc, search_item(cc, "nato"));
    write_cache_item(cc, search_item(cc, "ukura"));
    free_cache(cc);
}