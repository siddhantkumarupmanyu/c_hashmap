#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct LinkedList;

struct Node;

typedef struct LinkedList LinkedList;

typedef struct Node Node;

LinkedList* createLinkedList();

void addElement(LinkedList* list, void* element);

void* getElement(LinkedList* list, int index);

void freeLinkedList(LinkedList* list);

void* removeElement(LinkedList* list, int index);

#endif