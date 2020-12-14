#include "hashmap.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "linkedlist.h"

#define ARRAYSIZE 120

struct Hashmap {
    void* array[ARRAYSIZE];  // array of pointers
    int entries;
};

struct Entry {
    void* key;
    void* value;
};

static void fillUpArrayWithNull(void* array[]);

static int (*hashFun)(void*);
static bool (*equalsFun)(void*, void*);

Hashmap* createHashmap(int (*hashfun)(void*), bool(equalsfun)(void*, void*)) {
    hashFun = hashfun;
    equalsFun = equalsfun;
    Hashmap* hashmap = (Hashmap*)malloc(sizeof(Hashmap));
    hashmap->entries = 0;
    fillUpArrayWithNull(hashmap->array);
    return hashmap;
}

static void fillUpArrayWithNull(void* array[]) {
    for (int i = 0; i < ARRAYSIZE; i++) {
        array[i] = NULL;
    }
}

void addEntry(Hashmap* hashmap, void* key, void* value) {
    Entry* entry = (Entry*)malloc(sizeof(entry));
    entry->key = key;
    entry->value = value;

    int hash = hashFun(key);
    if (hashmap->array[hash] == NULL) {
        LinkedList* list = createLinkedList();
        hashmap->array[hash] = list;
    }
    LinkedList* list = (LinkedList*)(hashmap->array[hash]);
    addElement(list, entry);
}

#define HASHMAP_TEST

#ifdef HASHMAP_TEST

#include <assert.h>

int testHashFun(void* key) {
    return *((int*)(key));
}
bool testEqualFun(void* userKey, void* entryKey) {
    if (*((int*)userKey) == *((int*)entryKey)) {
        return true;
    } else {
        return false;
    }
}

void testCreate() {
    Hashmap* hashmap = createHashmap(testHashFun, testEqualFun);
    assert(hashmap->entries == 0);
    for (int i = 0; i < ARRAYSIZE; i++) {
        assert(hashmap->array[i] == NULL);
    }
}

void testAdd() {
    Hashmap* hashmap = createHashmap(testHashFun, testEqualFun);
    int key = 45;
    int value = 56;

    addEntry(hashmap, &key, &value);

    void* arrayBucket = hashmap->array[45];

    LinkedList* list = (LinkedList*)arrayBucket;

    for (int i = 0; i < list->count; i++) {
        void* element = getElement(list, i);
        Entry* entry = (Entry*)element;

        if (testEqualFun(&key, entry->key)) {
            assert(entry->key == &key);
            assert(*((int*)(entry->key)) == key);
            assert(entry->value == &value);
            assert(*((int*)(entry->value)) == value);
            break;
        }
    }
}

int main() {
    testCreate();
    testAdd();

    return 0;
}

#endif
