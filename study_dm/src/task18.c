#include "main.h"

HashTable* create_hash_table(int init_size){
    if (init_size < 1) return NULL;

    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (ht == NULL) return NULL;

    ht->buckets = (Node**)malloc(init_size*sizeof(Node*));
    if (ht->buckets == NULL){
        free(ht);
        msg_error_sellecting_mem("ht->buckets");
        return NULL;
    }
    ht->capacity = init_size;
    ht->size = 0;
    for(int i = 0; i < ht->capacity; i++)
        ht->buckets[i] = NULL;
    return ht;
}

void put_to_ht(HashTable* ht, const char* key, const int value){
    Node* nd = (Node*)malloc(sizeof(Node));
    if (nd == NULL) return;
    nd->key = malloc((strlen(key) + 1)*sizeof(char));
    if(nd->key == NULL){
        free(nd);
        msg_error_sellecting_mem(nd);
        return;
    }

    strcpy(nd->key, key);
    nd->next = ht->buckets[ht->size];
    ht->buckets[ht->size] = nd;
    
}

unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

