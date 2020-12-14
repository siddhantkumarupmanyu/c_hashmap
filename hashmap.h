#ifndef HASH_MAP_H
#define HASH_MAP_H
#include <stdbool.h>

struct Hashmap;

struct Entry;

typedef struct Hashmap Hashmap;

typedef struct Entry Entry;

Hashmap* createHashmap(int (*hashfun)(void*), bool(equalsfun)(void*, void*));

#endif