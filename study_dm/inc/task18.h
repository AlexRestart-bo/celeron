#ifndef TASK18_H
#define TASK18_H

typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node** buckets;
    int size;
    int capacity;
} HashTable;

void task18(void);

#endif