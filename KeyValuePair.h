//
// Created by itaym on 13/12/2024.
//

#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H
#include "Defs.h"


/**
 * Welcome to the Key-Value Pair module!
 * This module provides a generic Key-Value Pair Abstract Data Type (ADT) that can store
 * and manage pairs of elements (key and value) of any type, and provides operations to
 * create, destroy, access, and compare key-value pairs.
 * To use this ADT, users must provide callback functions for:
 * - Copying elements (key and value)
 * - Freeing elements (key and value)
 * - Printing elements (key and value)
 * - Comparing keys for equality (keys only)
 */



typedef struct KeyValuePair_s* KeyValuePair;





// Function declarations


/**
 * Creates a new key-value pair
 * @param key The key element
 * @param value The value element
 * @param freeKey Function to free the key (must be non-NULL)
 * @param printKey Function to print the key (must be non-NULL)
 * @param copyKey Function to copy the key (must be non-NULL)
 * @param equalKey Function to compare keys for equality (must be non-NULL)
 * @param freeValue Function to free the value (must be non-NULL)
 * @param printValue Function to print the value (must be non-NULL)
 * @param copyValue Function to copy the value (must be non-NULL)
 * @return Handle to the new key-value pair, or NULL if creation failed
 */
KeyValuePair createKeyValuePair(Element key, Element value,
                                FreeFunction freeKey, PrintFunction printKey, CopyFunction copyKey, EqualFunction equalKey,
                                FreeFunction freeValue, PrintFunction printValue, CopyFunction copyValue);





/**
 * Destroys a key-value pair and frees all associated resources
 * @param pair The key-value pair to destroy
 * @return Operation status indicating success or null pointer if received NULL in parameters
 */
status destroyKeyValuePair(KeyValuePair pair);





/**
 * Prints the value of the key-value pair using the provided value print function
 * @param pair The key-value pair
 * @return Operation status indicating success, failure or null pointer if received NULL in parameters
 */
status displayValue(KeyValuePair pair);






/**
 * Prints the key of the key-value pair using the provided key print function
 * @param pair The key-value pair
 * @return Operation status indicating success, failure or null pointer if received NULL in parameters
 */
status displayKey(KeyValuePair pair);





/**
 * Retrieves the value from the key-value pair
 * @param pair The key-value pair
 * @return Copy of the value element, based on the copy function supplied to the pair or NULL if pair is NULL or value is missing
 */
Element getValue(KeyValuePair pair);




/**
 * Retrieves the key from the key-value pair
 * @param pair The key-value pair
 * @return Copy of the key element, based on the copy function supplied to the pair or NULL if pair is NULL or key is missing
 */
Element getKey(KeyValuePair pair);



/**
 * Checks if a given key is equal to the key in the key-value pair
 * @param pair The key-value pair
 * @param key The key to compare
 * @return True if keys are equal, false otherwise (including NULL cases)
 */
bool isEqualKey(KeyValuePair pair, Element key);





#endif //KEYVALUEPAIR_H
