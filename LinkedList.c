//
// Created by itaym on 11/12/2024.
//

#include "LinkedList.h"






/* Internal node structure */
typedef struct Node_s {
    Element data;
    struct Node_s* next;
} Node;



/* Main list structure */
struct LinkedList_s {

    Node* head;
    Node* tail;
    int size;

    // cache for last access, will help iterate more efficiently
    Node* lastNode;    // last accessed node
    int lastIndex;     // lact accessed node's index

    CopyFunction copyFunc;
    FreeFunction freeFunc;
    PrintFunction printFunc;
    EqualFunction equalFunc;
};


// Node helper functions:

/* Helper function to create a new node */
static Node* createNode(Element data, CopyFunction copyFunc) {
    if (!data || !copyFunc) { // invalid arguments
        return NULL;
    }
    Node* node = (Node*) malloc(sizeof(Node));
    if (!node) { // if allocation didn't work
        return NULL;
    }
    node->data = copyFunc(data);

    if (!node->data) {
        free(node);
        return NULL;
    }

    node->next = NULL;
    return node;


}

// Interface Functions:


LinkedList createLinkedList(CopyFunction copyFunction, FreeFunction freeFunction, PrintFunction printFunction, EqualFunction equalFunction) {
    if (!copyFunction || !freeFunction || !printFunction || !equalFunction) { // one or more functions are null
        return NULL;
    }
    LinkedList list = (LinkedList) malloc(sizeof(struct LinkedList_s));
    if (!list) {
        return NULL;
    }

    // function assignments
    list->copyFunc = copyFunction;
    list->freeFunc = freeFunction;
    list->printFunc = printFunction;
    list->equalFunc = equalFunction;
    list->size = 0;
    list->head = list->tail = NULL;

    // initialize position
    list->lastNode = NULL;
    list->lastIndex = 0;

    return list;
}



status destroyList(LinkedList list) {
    if (!list) return null_pointer;

    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        if (current->data && list->freeFunc) {
            list->freeFunc(current->data);
        }
        free(current);
        current = next;
    }

    free(list);
    return success;
}


status appendNode(LinkedList list, Element element) {

    if (!list || !element) {
        return null_pointer;
    }
    // create and validate a new node - deep copying the element
    Node* newNode = createNode(element, list->copyFunc);
    if (!newNode) {
        return memory_problem;
    }
    // entering the node to the list
    if (!list->head) { // list is empty
        list->head = list->tail = newNode;
    } else { // add the new node at the end of the list
        list->tail->next = newNode;
        list->tail = newNode;
    }



    list->tail->next = NULL;
    list->size++;

    // reset position since list structure changed
    list->lastNode = NULL;
    list->lastIndex = 0;

    return success;
}

status deleteNode(LinkedList list, Element element) {
    if (!list || !element) {
        return null_pointer;
    }

    if (!list->head) {
        return failure;
    }

    Node* current = list->head;
    Node* prev = NULL;

    // find the node to delete
    while (current && !list->equalFunc(current->data, element)) {
        prev = current;
        current = current->next;
    }

    if (!current) {  // element not found
        return failure;
    }

    // update head if deleting first node
    if (current == list->head) {
        list->head = current->next;
    }

    // update tail if deleting last node
    if (current == list->tail) {
        list->tail = prev;
        if (prev) {
            prev->next = NULL;
        }
    }

    // middle node
    if (prev) {
        prev->next = current->next;
    }

    // free the node
    list->freeFunc(current->data);
    free(current);
    list->size--;

    // reset position since list structure changed
    list->lastNode = NULL;
    list->lastIndex = 0;


    return success;
}

status displayList(LinkedList list) {
    if (!list) {
        return null_pointer;
    }

    // traverse the list and print every node
    Node* current = list->head;
    while (current) {
        status result = list->printFunc(current->data);
        if (result != success) {
            return result;
        }
        current = current->next;
    }

    return success;
}



Element getDataByIndex(LinkedList list, int index) {
    if (!list || index < 1 || index > list->size) {
        return NULL;
    }

    Node* current;

    // if we have a position and requested index is after it, we can start from the current position instead
    if (list->lastNode && list->lastIndex <= index) {
        current = list->lastNode;
        int i = list->lastIndex;
        while (i < index) {
            current = current->next;
            i++;
        }
    }

    else {
        current = list->head;
        for (int i = 1; i < index; i++) {
            current = current->next;
        }
    }

    // update cache for next time
    list->lastNode = current;
    list->lastIndex = index;

    return list->copyFunc(current->data);
}




int getLengthList(LinkedList list) {
    if (!list) {
        return 0;
    }
    return list->size;
}



Element searchByKeyInList(LinkedList list, Element element) {
    if (!list || !element || !list->equalFunc) {
        return NULL;
    }

    // linear search through the list
    Node* current = list->head;
    while (current) {
        if (current->data) {
            if (list->equalFunc(current->data, element)) { // if this is the element we're looking for
                return list->copyFunc(current->data);
            }

        }
        current = current->next;
    }
    return NULL; // element not found
}









