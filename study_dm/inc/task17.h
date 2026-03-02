#ifndef TASK17_H
#define TASK17_H

typedef struct {
    char* key;
    int value;
} CacheEntry;

typedef struct {
    CacheEntry* entries;
    int size;
    int capacity;
    int head;
} Cache;

void task17(void);

#endif