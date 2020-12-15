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
static int getLimitedHash(void* key);

static int (*hashFun)(void*);
static bool (*equalsFun)(void*, void*);

Hashmap* createHashmap(int (*hashfun)(void*), bool (*equalsfun)(void*, void*)) {
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

void addValue(Hashmap* hashmap, void* key, void* value) {
    Entry* entry = (Entry*)malloc(sizeof(entry));
    entry->key = key;
    entry->value = value;

    int hash = getLimitedHash(key);
    if (hashmap->array[hash] == NULL) {
        LinkedList* list = createLinkedList();
        hashmap->array[hash] = list;
    }
    LinkedList* list = (LinkedList*)(hashmap->array[hash]);
    addElement(list, entry);
    hashmap->entries++;
}

void* getValue(Hashmap* hashmap, void* key) {
    int hash = getLimitedHash(key);

    void* arrayBucket = hashmap->array[hash];

    LinkedList* list = (LinkedList*)arrayBucket;

    for (int i = 0; i < list->count; i++) {
        void* element = getElement(list, i);
        Entry* entry = (Entry*)element;

        if (equalsFun(key, entry->key)) {
            return entry->value;
        }
    }
    return NULL;
}

void* removeValue(Hashmap* hashmap, void* key) {
    int hash = getLimitedHash(key);

    void* arrayBucket = hashmap->array[hash];

    LinkedList* list = (LinkedList*)arrayBucket;

    for (int i = 0; i < list->count; i++) {
        void* element = getElement(list, i);
        Entry* entry = (Entry*)element;

        if (equalsFun(key, entry->key)) {
            void* value = entry->value;
            removeElement(list, i);
            return value;
        }
    }

    return NULL;
}

static int getLimitedHash(void* key) {
    int hash = hashFun(key);
    hash = hash % ARRAYSIZE;
    return hash;
}

void freeHashMap(Hashmap* hashmap, void (*freeKeyAndValue)(void* key, void* value)) {
    for (int i = 0; i < ARRAYSIZE; i++) {
        if (hashmap->array[i] != NULL) {
            LinkedList* list = (LinkedList*)(hashmap->array[i]);

            for (int i = 0; i < list->count; i++) {
                void* element = getElement(list, i);
                Entry* entry = (Entry*)element;

                freeKeyAndValue(entry->key, entry->value);
            }

            freeLinkedList(list);
            hashmap->array[i] = NULL;
        }
    }
    free(hashmap);
    hashmap = NULL;
}

// #define HASHMAP_TEST

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

    addValue(hashmap, &key, &value);

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

void testRetrieve() {
    Hashmap* hashmap = createHashmap(testHashFun, testEqualFun);
    int key = 45;
    int value = 56;

    addValue(hashmap, &key, &value);

    void* retrivedValue = getValue(hashmap, &key);
    assert(retrivedValue == &value);
    assert(*((int*)retrivedValue) == value);
}

void testRemove() {
    Hashmap* hashmap = createHashmap(testHashFun, testEqualFun);
    int key = 45;
    int value = 56;

    addValue(hashmap, &key, &value);

    void* removedValue = removeValue(hashmap, &key);
    assert(removedValue == &value);
    assert(*((int*)removedValue) == value);

    void* getval = getValue(hashmap, &key);
    assert(getval == NULL);

    void* getrem = removeValue(hashmap, &key);
    assert(getrem == NULL);
}

void testFreeKeyAndValue(void* key, void* value) {
    free(key);
    free(value);
}

void testFree() {
    Hashmap* hashmap = createHashmap(testHashFun, testEqualFun);
    int* key = (int*)malloc(sizeof(int));
    int* value = (int*)malloc(sizeof(int));

    *key = 45;
    *value = 56;

    addValue(hashmap, key, value);

    freeHashMap(hashmap, testFreeKeyAndValue);
}

int main() {
    testCreate();
    testAdd();
    testRetrieve();
    testRemove();
    testFree();

    return 0;
}

#endif
