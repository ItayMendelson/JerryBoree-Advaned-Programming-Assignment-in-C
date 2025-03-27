//
// Created by itaym on 13/12/2024.
//
#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

struct hashTable_s {

    LinkedList* buckets; // LinkedList for chaining
    int size; // size of the hash table

    // key functions

    CopyFunction copyFuncKey;
    FreeFunction freeFuncKey;
    PrintFunction printFuncKey;
    EqualFunction equalFuncKey;

    // value functions

    CopyFunction copyFuncValue;
    FreeFunction freeFuncValue;
    PrintFunction printFuncValue;

    // additional functions
    TransformIntoNumberFunction transformFuncKey;
};



/* helper functions for LinkedList to handle KeyValuePair elements */

static Element copyKVP(Element kvp) {
    return kvp;  // The KeyValuePair is already copied in createEntry
}

static status freeKVP(Element kvp) {
    if (!kvp) return failure;
    return destroyKeyValuePair((KeyValuePair)kvp);
}

static status printKVP(Element kvp) {
    if (!kvp) return failure;
    KeyValuePair pair = (KeyValuePair)kvp;

    status keyStatus = displayKey(pair);
    if (keyStatus != success) return keyStatus;
    status valueStatus = displayValue(pair);
    if (valueStatus != success) return valueStatus;
    return success;
}


static bool equalKVP(Element kvp1, Element key) {
    if (!kvp1 || !key) return false;
    // kvp1 is KeyValuePair, but key is just the raw key
    KeyValuePair pair = (KeyValuePair)kvp1;
    return isEqualKey(pair, key); // isEqualKey already compares with raw key
}


// wrapper function to create KVPs more easily without a lot of arguments
static KeyValuePair createEntry(hashTable table, Element key, Element value) {
    if (!table || !key || !value) {
        return NULL;
    }
    KeyValuePair kvp = createKeyValuePair(key, value, table->freeFuncKey, table->printFuncKey, table->copyFuncKey,
        table->equalFuncKey, table->freeFuncValue, table->printFuncValue, table->copyFuncValue);
    if (kvp == NULL) {
        return NULL;
    }
    return kvp;
}



/* hash table functions */



// internal function to get the bucket index for a key
static int getBucketIndex(hashTable table, Element key) {
    if (!table || !key) {
        return -1;
    }
    int hashVal = table->transformFuncKey(key);
    if (hashVal < 0) {
        return -1;
    }
    return hashVal % table->size;
}


// helper function to initialize linked lists for chaining in each bucket of the hash table
static LinkedList* initializeBuckets(int size, status* state) {
    LinkedList* buckets = malloc(size * sizeof(LinkedList));
    if (!buckets) {
        *state = memory_problem;
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        // each bucket is a LinkedList for collision handling
        buckets[i] = createLinkedList(copyKVP, freeKVP, printKVP, equalKVP);
        if (!buckets[i]) {
            // cleanup previously allocated buckets
            for (int j = 0; j < i; j++) {
                destroyList(buckets[j]);
            }
            free(buckets);
            *state = memory_problem;
            return NULL;
        }
    }
    *state = success;
    return buckets;
}



hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue
        || !equalKey || !transformIntoNumber || hashNumber <= 0) {
        return NULL;
        }

    hashTable table = malloc(sizeof(struct hashTable_s));
    if (!table) {
        return NULL;
    }

    status bucketState;
    table->buckets = initializeBuckets(hashNumber, &bucketState);
    if (bucketState != success) {
        free(table);
        return NULL;
    }

    // set table size

    table->size = hashNumber;


    // set key properties


    table->copyFuncKey = copyKey;
    table->freeFuncKey = freeKey;
    table->printFuncKey = printKey;
    table->equalFuncKey = equalKey;

    // set value properties
    table->copyFuncValue = copyValue;
    table->freeFuncValue = freeValue;
    table->printFuncValue = printValue;

    // set key hash function
    table->transformFuncKey = transformIntoNumber;

    return table;
}



status destroyHashTable(hashTable table) {
    if (!table) {
        return null_pointer;

    }

    for (int i = 0; i < table->size; i++) {
        if (table->buckets[i]) {
            destroyList(table->buckets[i]);
        }
    }

    free(table->buckets);
    free(table);
    return success;
}


Element lookupInHashTable(hashTable table, Element key) {
    if (!table || !key) {
        return NULL;
    }

    int index = getBucketIndex(table, key);
    if (index < 0) {
        return NULL;
    }

    LinkedList bucket = table->buckets[index];

    KeyValuePair foundPair = searchByKeyInList(bucket, key);
    return foundPair ? getValue(foundPair) : NULL; // as the value is copied in the first place in getValue
}

status addToHashTable(hashTable table, Element key,Element value) {
    if (!table || !key || !value) {
        return null_pointer;
    }

    if (lookupInHashTable(table, key) != NULL) {
        return failure;
    }

    int index = getBucketIndex(table, key); // get the right index in the hash table
    if (index < 0) {
        return failure;
    }
    LinkedList bucket = table->buckets[index];
    KeyValuePair pair = createEntry(table, key, value);
    if (!pair) {
        return memory_problem;
    }

    status state = appendNode(bucket, pair); // add the node to the linked list

    if (state != success) {
        destroyKeyValuePair(pair);
    }
    return state;

}


status removeFromHashTable(hashTable table, Element key) {
    if (!table || !key) {
        return null_pointer;
    }

    int index = getBucketIndex(table, key);
    if (index < 0) {
        return failure;
    }

    LinkedList bucket = table->buckets[index];

    // pass the key directly to deleteNode in the LinkedList

    return deleteNode(bucket, key);
}

status displayHashElements(hashTable table) {
    if (!table) {
        return null_pointer;
    }
    for (int i = 0; i < table->size; i++) {
        if (getLengthList(table->buckets[i]) > 0) {
            status state = displayList(table->buckets[i]);
            if (state != success) {
                return state;
            }
        }

    }
    return success;

}