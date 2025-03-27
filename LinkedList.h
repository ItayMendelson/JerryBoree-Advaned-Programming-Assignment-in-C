//
// Created by itaym on 11/12/2024.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Defs.h"


/**
* Welcome to the Linked List module!
* This module provides a generic list Abstract Data Type (ADT) that can store
* and manage elements of any type, and provides standard list operations like append, delete, search and display.
* through a clean interface.
*  *To use this ADT, users must provide callback functions for:
* - Copying elements
* - Freeing elements
* - Printing elements
* - Comparing elements for equality
*/




typedef struct LinkedList_s* LinkedList;




// Functions:


/**
 * Creates a new empty list
 * @param copyFunction Function to copy elements (must be non-NULL)
 * @param freeFunction Function to free elements (must be non-NULL)
 * @param printFunction Function to print elements (must be non-NULL)
 * @param equalFunction Function to compare elements (must be non-NULL)
 * @return Handle to the new list, or NULL if creation failed
 */
LinkedList createLinkedList(CopyFunction copyFunction, FreeFunction freeFunction, PrintFunction printFunction, EqualFunction equalFunction);





/**
 * Destroys a list and frees all associated resources
 * @param list The list to destroy
 * @return Operation status indicating success or null pointer if received NULL in parameters
 */
status destroyList(LinkedList list);






/**
 * Adds an element to the end of the list
 * @param list The list to add to
 * @param element The element to add
 * @return Operation status indicating success, failure or memory problem if memory allocation fails
 */
status appendNode(LinkedList list, Element element);






/**
 * Removes the occurrence of an element from the list
 * @param list The list to remove from
 * @param element The element to remove
 * @return Operation status indicating success or failure
 */
status deleteNode(LinkedList list, Element element);






/**
 * Prints all elements in the list using the provided print function
 * @param list The list to display
* @return Operation status indicating success, failure or null pointer if received NULL in parameters
 */
status displayList(LinkedList list);





/**
 * Retrieves an element at a specific position
 * @param list The list to access
 * @param index The position (1-based indexing - starting to count from 1)
 * @return The element at the specified position, or NULL if invalid index
 */
Element getDataByIndex(LinkedList list, int index);







/**
 * Returns the current number of elements in the list
 * @param list The list to check
 * @return Number of elements, 0 if list is NULL
 */
int getLengthList(LinkedList list);








/**
 * Searches for an element in the list
 * @param list The list to search in
 * @param element Key element to search for
 * @return First matching element found, or NULL if not found
 */
Element searchByKeyInList(LinkedList list, Element element);


#endif //LINKEDLIST_H
