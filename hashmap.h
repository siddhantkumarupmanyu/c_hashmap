#ifndef HASH_MAP_H
#define HASH_MAP_H
#include <stdbool.h>

#define ARRAYSIZE 120

struct Hashmap {
    void* array[ARRAYSIZE];  // array of pointers
    int entries;
};

struct Entry {
    void* key;
    void* value;
};

typedef struct Hashmap Hashmap;

typedef struct Entry Entry;

Hashmap* createHashmap(int (*hashfun)(void*), bool(equalsfun)(void*, void*));

void addValue(Hashmap* hashmap, void* key, void* value);

void* getValue(Hashmap* hashmap, void* key);

void* removeValue(Hashmap* hashmap, void* key);

void freeHashMap(Hashmap* hashmap, void (*freeKeyAndValue)(void* key, void* value));

#endif