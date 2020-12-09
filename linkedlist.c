#include "linkedlist.h"

#include <stdlib.h>

struct LinkedList {
    int count;
    Node* head;
    Node* last;
};

struct Node {
    void* data;
    Node* next;
};

static void adjustListHeadAndLastPointers(LinkedList* list, Node* node);
static void freeNode(Node* node);

LinkedList* createLinkedList() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->count = 0;
    list->head = NULL;
    list->last = NULL;
    return list;
}

void addElement(LinkedList* list, void* element) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = element;

    if (list->last) {
        list->last->next = node;
    }

    node->next = NULL;

    list->count++;

    adjustListHeadAndLastPointers(list, node);
}

static void adjustListHeadAndLastPointers(LinkedList* list, Node* node) {
    if ((!(list->head)) || (!node)) {
        list->head = node;
    }
    list->last = node;
}

void* getElement(LinkedList* list, int index) {
    Node* currentNode = list->head;
    int i = 0;
    while (currentNode) {
        if (i == index) {
            return currentNode->data;
        }
        i++;
        currentNode = currentNode->next;
    }
    return NULL;
}

void freeLinkedList(LinkedList* list) {
    Node* currentNode = list->head;
    Node* nextNode;
    while (currentNode) {
        nextNode = currentNode->next;

        freeNode(currentNode);

        currentNode = nextNode;
    }
    list->head = NULL;
    list->last = NULL;
    list->count = 0;
    free(list);
    list = NULL;
}

static void freeNode(Node* node) {
    free(node->data);
    node->next = NULL;
    free(node);
    node = NULL;
}

void* removeElement(LinkedList* list, int index) {
    Node* currentNode = list->head;
    Node* prev;
    int i = 0;
    while (currentNode) {
        if (i == index) {
            if (i == 0) {
                list->head = currentNode->next;
            } else if (i == (list->count - 1)) {
                list->last = prev;
                prev->next = NULL;
            } else {
                prev->next = currentNode->next;
            }

            void* data = currentNode->data;

            currentNode->next = NULL;
            free(currentNode);

            list->count--;
            return data;
        }
        prev = currentNode;
        i++;
        currentNode = currentNode->next;
    }
    return NULL;
}

void addElementAt(LinkedList* list, void* element, int index) {
    Node* currentNode = list->head;
    Node* prev;
    int i = 0;

    if (index == list->count) {
        addElement(list, element);
        return;
    }

    while (currentNode) {
        if (i == index) {
            Node* node = (Node*)malloc(sizeof(Node));
            node->data = element;

            if (i == 0) {  // head case
                list->head = node;
            } else {
                prev->next = node;
            }
            node->next = currentNode;

            list->count++;
            return;
        }
        prev = currentNode;
        i++;
        currentNode = currentNode->next;
    }
}

// #define LINKEDLIST_TEST

#ifdef LINKEDLIST_TEST

#include <assert.h>

void testCreate() {
    LinkedList* list = createLinkedList();
    assert(list->count == 0);
}

void testAdd() {
    LinkedList* list = createLinkedList();
    int* a = (int*)malloc(sizeof(int));
    *a = 42;
    int* b = (int*)malloc(sizeof(int));
    *b = 56;
    addElement(list, (void*)a);
    addElement(list, (void*)b);
    assert(list->count == 2);
}

void testGetElement_Exist() {
    LinkedList* list = createLinkedList();
    int* a = (int*)malloc(sizeof(int));
    *a = 42;
    int* b = (int*)malloc(sizeof(int));
    *b = 56;
    addElement(list, (void*)a);
    addElement(list, (void*)b);
    assert(list->count == 2);

    int* gotA = (int*)getElement(list, 0);
    assert(*gotA == *a);
    assert(gotA == a);

    int* gotB = (int*)getElement(list, 1);
    assert(*gotB == *b);
    assert(gotB == b);
}

void testGetElement_Not_Exist() {
    LinkedList* list = createLinkedList();
    int* a = (int*)malloc(sizeof(int));
    *a = 42;

    addElement(list, (void*)a);
    assert(list->count == 1);

    int* gotA = (int*)getElement(list, 0);
    assert(*gotA == *a);
    assert(gotA == a);

    int* gotB = (int*)getElement(list, 1);
    assert(gotB == NULL);
}

void testFreeList() {
    LinkedList* list = createLinkedList();

    int* a = (int*)malloc(sizeof(int));
    *a = 42;
    int* b = (int*)malloc(sizeof(int));
    *b = 56;
    addElement(list, (void*)a);
    addElement(list, (void*)b);
    assert(list->count == 2);

    freeLinkedList(list);
    // assert(); but what
}

void testRemoveElement() {
    LinkedList* list = createLinkedList();
    int* a = (int*)malloc(sizeof(int));
    *a = 42;
    int* b = (int*)malloc(sizeof(int));
    *b = 56;
    int* c = (int*)malloc(sizeof(int));
    *b = 66;
    addElement(list, (void*)a);
    addElement(list, (void*)b);
    addElement(list, (void*)c);

    assert(list->count == 3);

    int* gotA = (int*)getElement(list, 0);
    assert(*gotA == *a);
    assert(gotA == a);

    int* removedElement = removeElement(list, 1);
    assert(*removedElement == *b);
    assert(removedElement == b);

    int* gotC = (int*)getElement(list, 1);
    assert(*gotC == *c);
    assert(gotC == c);

    assert(list->count == 2);
}

void testRemoveElement_First() {
    LinkedList* list = createLinkedList();
    int* a = (int*)malloc(sizeof(int));
    *a = 42;
    int* b = (int*)malloc(sizeof(int));
    *b = 56;
    addElement(list, (void*)a);
    addElement(list, (void*)b);

    assert(list->count == 2);

    int* removedElement = removeElement(list, 0);
    assert(*removedElement == *a);
    assert(removedElement == a);

    int* gotB = (int*)getElement(list, 0);
    assert(*gotB == *b);
    assert(gotB == b);

    assert(list->count == 1);

    assert(list->head->data == b);
}

void testRemoveElement_Last() {
    LinkedList* list = createLinkedList();
    int* a = (int*)malloc(sizeof(int));
    *a = 42;
    int* b = (int*)malloc(sizeof(int));
    *b = 56;
    addElement(list, (void*)a);
    addElement(list, (void*)b);

    assert(list->count == 2);

    int* removedElement = removeElement(list, 1);
    assert(*removedElement == *b);
    assert(removedElement == b);

    assert(list->count == 1);

    assert(list->last->data == a);
}

void testInsertElementAt() {
    LinkedList* list = createLinkedList();
    int* a = (int*)malloc(sizeof(int));
    *a = 42;
    int* b = (int*)malloc(sizeof(int));
    *b = 56;
    addElement(list, (void*)a);
    addElement(list, (void*)b);

    assert(list->count == 2);

    int c = 46;

    addElementAt(list, &c, 1);

    assert(getElement(list, 1) == (void*)&c);
    assert(*((int*)getElement(list, 1)) == c);

    assert(list->count == 3);
}

void testInsertElementAt_Head() {
    LinkedList* list = createLinkedList();
    int a = 42;
    int b = 56;
    addElement(list, &a);
    addElement(list, &b);

    assert(list->count == 2);

    int c = 46;

    addElementAt(list, &c, 0);

    assert(getElement(list, 0) == &c);
    assert(*((int*)getElement(list, 0)) == c);

    assert(list->count == 3);
}

void testInsertElementAt_Last() {
    LinkedList* list = createLinkedList();
    int a = 42;
    int b = 56;
    addElement(list, &a);
    addElement(list, &b);

    assert(list->count == 2);

    int c = 46;

    addElementAt(list, &c, 2);

    assert(getElement(list, 2) == (void*)&c);
    assert(*((int*)getElement(list, 2)) == c);

    assert(list->count == 3);
}

int main() {
    testCreate();
    testAdd();
    testGetElement_Exist();
    testGetElement_Not_Exist();
    testFreeList();
    testRemoveElement();
    testRemoveElement_First();
    testRemoveElement_Last();
    testInsertElementAt();
    testInsertElementAt_Head();
    testInsertElementAt_Last();

    return 0;
}

#endif