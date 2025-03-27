//
// Created by itaym on 14/12/2024.
//
#ifndef MULTIVALUEHASHTABLE_H
#define MULTIVALUEHASHTABLE_H
#include "LinkedList.h"



/**
 * Welcome to the Multi-Value Hash Table module!
 * This module provides a generic Multi-Value Hash Table Abstract Data Type (ADT) that can store
 * and manage key-value pairs where each key can be associated with multiple values. It provides
 * operations to create, destroy, add, remove, lookup, and display elements in the multi-value hash table.
 * To use this ADT, users must provide callback functions for:
 * - Copying elements (key and value)
 * - Freeing elements (key and value)
 * - Printing elements (key and value)
 * - Comparing keys for equality
 * - Comparing values for equality
 * - Transforming keys into integer hash values
 */


typedef struct MultiValueHashTable_s* MultiValueHashTable;

// Function declarations

/**
 * Creates a new multi-value hash table
 * @param copyKey Function to copy keys (must be non-NULL)
 * @param freeKey Function to free keys (must be non-NULL)
 * @param printKey Function to print keys (must be non-NULL)
 * @param copyValue Function to copy values (must be non-NULL)
 * @param freeValue Function to free values (must be non-NULL)
 * @param printValue Function to print values (must be non-NULL)
 * @param equalKey Function to compare keys for equality (must be non-NULL)
 * @param equalValue Function to compare values for equality (must be non-NULL)
 * @param transformIntoNumber Function to transform keys into integer hash values (must be non-NULL)
 * @param hashNumber The size of the underlying hash table (must be positive)
 * @return pointer to the new multi-value hash table, or NULL if creation failed
 */
MultiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
    CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
    EqualFunction equalKey, EqualFunction equalValue, TransformIntoNumberFunction transformIntoNumber,
    int hashNumber);


/**
 * Destroys a multi-value hash table and frees all associated resources
 * @param mvht The multi-value hash table to destroy
 * @return Operation status indicating success or null pointer if received NULL in parameters
 */
status destroyMultiValueHashTable(MultiValueHashTable mvht);



/**
 * Adds a key-value pair to the multi-value hash table
 * @param mvht The multi-value hash table
 * @param key The key
 * @param value The value to associate with the key
 * @return Operation status indicating success, null pointer if received NULL in parameters, or memory allocation failure
 */
status addToMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value);





/**
 * Retrieves the linked list of values associated with a key from the multi-value hash table
 * @param mvht The multi-value hash table
 * @param key The key to look up
 * @return The linked list of values associated with the key, or NULL if key not found or invalid argument
 */
LinkedList lookupInMultiValueHashTable(MultiValueHashTable mvht, Element key);





/**
 * Removes a specific value associated with a key from the multi-value hash table
 * @param mvht The multi-value hash table
 * @param key The key
 * @param value The value to remove
 * @return Operation status indicating success, failure (if key or value not found), or null pointer if received NULL in parameters
 */
status removeFromMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value);


/**
 * Displays all values associated with a key in the multi-value hash table
 * @param mvht The multi-value hash table
 * @param key The key whose values to display
 * @return Operation status indicating success, failure (if key not found), or null pointer if received NULL in parameters
 */
status displayMultiValueHashElementsByKey(MultiValueHashTable mvht, Element key);


#endif //MULTIVALUEHASHTABLE_H
