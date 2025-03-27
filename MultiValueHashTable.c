//
// Created by itaym on 17/12/2024.
//
#include "MultiValueHashTable.h"
#include "HashTable.h"

struct MultiValueHashTable_s {
    hashTable table;  // hashtable to store keys and their associated LinkedLists


    PrintFunction printKey;

    // to handle the values inside the linked lists
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalValue;
};


// helper functions for HashTable to manage LinkedList elements
static Element copyList(Element list) {
    return (LinkedList)list;
}

static status freeList(Element list) {
    if (!list) return null_pointer;
    // we own the list so we can destroy it
    destroyList((LinkedList)list);
    return success;
}


static status printList(Element list) {
    if (!list) return null_pointer;
    return displayList((LinkedList)list);
}



MultiValueHashTable createMultiValueHashTable(
    CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
    CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
    EqualFunction equalKey, EqualFunction equalValue,
    TransformIntoNumberFunction transformIntoNumber, int hashNumber) {

    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue ||
        !printValue || !equalKey || !equalValue || !transformIntoNumber || hashNumber <= 0) {
        return NULL;
    }

    MultiValueHashTable mvht = malloc(sizeof(struct MultiValueHashTable_s));
    if (!mvht) return NULL;

    mvht->table = createHashTable(copyKey, freeKey, printKey,
                                 copyList, freeList, printList,
                                 equalKey, transformIntoNumber, hashNumber);
    if (!mvht->table) {
        free(mvht);
        return NULL;
    }

    mvht->printKey = printKey;
    mvht->copyValue = copyValue;
    mvht->freeValue = freeValue;
    mvht->printValue = printValue;
    mvht->equalValue = equalValue;

    return mvht;
}

status destroyMultiValueHashTable(MultiValueHashTable mvht) {
    if (!mvht) return null_pointer;
    status result = destroyHashTable(mvht->table);
    if (result != success) return result;
    free(mvht);
    return result;
}

status addToMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    if (!mvht || !key || !value) return null_pointer;

    // check if it already exists
    LinkedList valueList = lookupInHashTable(mvht->table, key);

    if (!valueList) {
        // create new list if key doesn't exist
        valueList = createLinkedList(mvht->copyValue, mvht->freeValue,
                                   mvht->printValue, mvht->equalValue);
        if (!valueList) return memory_problem;

        // add value to new list
        status appendStatus = appendNode(valueList, value);
        if (appendStatus != success) {
            destroyList(valueList);  // clean up the list if append fails
            return memory_problem;
        }

        // and add list to hash table
        status result = addToHashTable(mvht->table, key, valueList);
        if (result != success) {
            destroyList(valueList);
            return result;
        }
    } else {
        // add to existing list
        return appendNode(valueList, value);
    }

    return success;
}

LinkedList lookupInMultiValueHashTable(MultiValueHashTable mvht, Element key) {
    if (!mvht || !key) return NULL;
    return lookupInHashTable(mvht->table, key);
}

status removeFromMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    if (!mvht || !key || !value) return null_pointer;

    LinkedList valueList = lookupInMultiValueHashTable(mvht, key);
    if (!valueList) {
        return failure;
    }

    status removeStatus = deleteNode(valueList, value);
    if (removeStatus != success) {
        return removeStatus;
    }

    // if the list is empty, remove the key from the hash table
    if (getLengthList(valueList) == 0) {
        return removeFromHashTable(mvht->table, key);
    }

    return success;
}

status displayMultiValueHashElementsByKey(MultiValueHashTable mvht, Element key) {
    if (!mvht || !key) return null_pointer;

    LinkedList valueList = lookupInMultiValueHashTable(mvht, key);
    if (!valueList) { // haven't found the key
        return failure;
    }

    mvht->printKey(key);
    return printList(valueList);
}