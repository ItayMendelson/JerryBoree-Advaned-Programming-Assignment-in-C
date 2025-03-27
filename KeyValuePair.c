//
// Created by itaym on 13/12/2024.
//
#include "KeyValuePair.h"

struct KeyValuePair_s {
    Element key;
    Element value;
    FreeFunction freeFuncKey;
    PrintFunction printFuncKey;
    EqualFunction equalFuncKey;
    CopyFunction copyFuncKey;
    FreeFunction freeFuncVal;
    PrintFunction printFuncVal;
    CopyFunction copyFuncVal;


};

// functions

KeyValuePair createKeyValuePair(Element key, Element value,
                                FreeFunction freeKey, PrintFunction printKey, CopyFunction copyKey, EqualFunction equalKey,
                                FreeFunction freeValue, PrintFunction printValue, CopyFunction copyValue) {
    if (!key || !value || !freeKey || !printKey || !copyKey || !equalKey || !freeValue || !printValue
        || !copyValue) {
        return NULL;
    }


    KeyValuePair pair = (KeyValuePair)malloc(sizeof(struct KeyValuePair_s));
    if (!pair) {
        return NULL;
    }



    // copy the key
    pair->key = copyKey(key);
    if (!pair->key) {
        free(pair);
        return NULL;
    }

    // copy the value
    pair->value = copyValue(value);
    if (!pair->value) {
        freeKey(pair->key);
        free(pair);
        return NULL;
    }

    // store the functions we will need after creation
    pair->freeFuncKey = freeKey;
    pair->printFuncKey = printKey;
    pair->equalFuncKey = equalKey;
    pair->copyFuncKey = copyKey;
    pair->freeFuncVal = freeValue;
    pair->printFuncVal = printValue;
    pair->copyFuncVal = copyValue;




    return pair;
}

status destroyKeyValuePair(KeyValuePair pair) {
    if (!pair) {
        return null_pointer;
    }
    if (pair->key) {
        pair->freeFuncKey(pair->key);
    }
    if (pair->value) {
        pair->freeFuncVal(pair->value);
    }
    free(pair);
    pair = NULL;
    return success;
}

status displayValue(KeyValuePair pair) {
    if (!pair) {
        return null_pointer;
    }

    if (pair->value) {
        return pair->printFuncVal(pair->value);
    }
    return failure;
}

status displayKey(KeyValuePair pair) {
    if (!pair) {
        return null_pointer;
    }
    if (pair->key) {
        return pair->printFuncKey(pair->key); // using the received key print function
    }
    return failure;
}

Element getValue(KeyValuePair pair) {
    if (!pair) {
        return NULL;
    }
    if (pair->value) {
        return pair->copyFuncVal(pair->value);  // using the received value copy function
    }
    return NULL;
}

Element getKey(KeyValuePair pair) {
    if (!pair) {
        return NULL;
    }
    if (pair->key) {
        return pair->copyFuncKey(pair->key); // using the received key copy function
    }
    return NULL;
}

bool isEqualKey(KeyValuePair pair, Element key) {
    if (!pair || !key) {
        return false;
    }
    return pair->equalFuncKey(pair->key, key); // using the received key equal function
}