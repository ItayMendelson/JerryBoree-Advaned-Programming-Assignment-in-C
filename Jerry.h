//
// Created by itaym on 29/11/2024.
//

#ifndef JERRY_H
#define JERRY_H
#include "Defs.h"



// planet structure

/**
 * Represents a planet in the multiverse.
 * Each planet has a unique name and specific coordinates in space.
 */
typedef struct Planet_t {
    char* name; // Unique name of the planet (dynamically allocated)
    double x;   // X coordinate in space
    double y;   // Y coordinate in space
    double z;   // Z coordinate in space
} Planet;



// origin structure

/**
 * Represents the origin/home universe of a Jerry.
 * Contains information about both the dimension and planet the Jerry is from.
 */
typedef struct Origin_t {
    Planet* planet;  // Pointer to the planet (shared among Jerries from same planet)
    char* dimension; // Name of the dimension (dynamically allocated)
} Origin;



// physical characteristics structure


/**
 * Represents a physical characteristic of a Jerry.
 * Each characteristic has a name and a numeric value.
 */
typedef struct PhysicalCharacteristic_t {
    char* name;   // Name of the characteristic (dynamically allocated)
    double value; // Numeric value of the characteristic
} PhysicalCharacteristic;



// Jerry structure


/**
 * Represents a Jerry from the multiverse.
 * Each Jerry has a unique ID, happiness level, origin information,
 * and a dynamic array of physical characteristics.
 */
typedef struct Jerry_t {
    char* id;       // Unique identifier (dynamically allocated)
    int happiness;  // Happiness level (0-100)
    Origin* origin; // Pointer to Jerry's origin information
    PhysicalCharacteristic** characteristics; // Dynamic array of pointers to physical characteristics
    int num_characteristics;  // Number of characteristics in the array
} Jerry;


// planet functions

/**
 * Creates a new Planet object with the specified name and coordinates.
 * Allocates memory for the Planet structure and its name string.
 * @param name - Name of the planet (will be deep copied).
 * @param x - X coordinate of the planet in space.
 * @param y - Y coordinate of the planet in space.
 * @param z - Z coordinate of the planet in space.
 * @return Planet* - Pointer to the created Planet object, or NULL if memory allocation fails
 * or if name is NULL.
 */
Planet* createPlanet(char* name, double x, double y, double z);



/**
 * Deletes a Planet object and frees all associated memory.
 * Frees both the name string and the Planet structure.
 * @param planet - Pointer to the Planet object to delete.
 * @return status - success if deletion is successful, failure if planet is NULL.
 */
status destroyPlanet(Planet* planet);


/**
 * Prints the information about a Planet object.
 * Format: "Planet : [name] (x.xx,y.yy,z.zz) \n"
 * Numbers are printed with exactly 2 decimal places.
 * @param planet - Pointer to the Planet object to print.
 * @return status - success if printing is successful, failure if planet is NULL.
 */
status printPlanet(Planet* planet);



// origin functions


/**
 * Creates a new Origin object associating a planet with a dimension.
 * Allocates memory for the Origin structure and its dimension string.
 * @param planet - Pointer to the Planet object (will be referenced, not copied).
 * @param dimension - Name of the dimension (will be deep copied).
 * @return Origin* - Pointer to the created Origin object, or NULL if memory allocation fails
 * or if either parameter is NULL.
 */
Origin* createOrigin(Planet* planet, char* dimension);



/**
 * Deletes an Origin object.
 * Frees the memory associated with the Origin's dimension string and the Origin structure itself.
 * @param origin - Pointer to the Origin object to delete.
 * @return status - success if deletion is successful, failure if origin is NULL.
 */
status destroyOrigin(Origin* origin);



// physical characteristics functions



/**
 * Creates a new PhysicalCharacteristic object with the specified name and value.
 * Allocates memory for the PhysicalCharacteristic structure and its name string.
 * @param name - Name of the characteristic (will be deep copied).
 * @param value - Numeric value of the characteristic.
 * @return PhysicalCharacteristic* - Pointer to the created PhysicalCharacteristic object,
 * or NULL if memory allocation fails or if name is NULL.
 */
PhysicalCharacteristic* createPhysicalCharacteristic(char* name, double value);


/**
 * Deallocates all memory associated with a PhysicalCharacteristic object.
 * Frees both the name string and the PhysicalCharacteristic structure itself.
 * @param characteristic - Pointer to the PhysicalCharacteristic object to destroy
 * @return status - success if destruction is successful, failure if characteristic is NULL
 */
status destroyPhysicalCharacteristic(PhysicalCharacteristic* characteristic);



// Jerry functions


/**
 * Creates a new Jerry object. and Allocates memory for the Jerry structure and initializes its fields.
 * @param id - Unique ID for the Jerry.
 * @param happiness - Happiness level of the Jerry (0-100).
 * @param planet - - Pointer to the Planet where Jerry is from.
 * @param dimension - - The dimension Jerry is from.
 * @return Jerry* - Pointer to the created Jerry object, or NULL if memory allocation fails.
 */
Jerry* createJerry(char* id, int happiness, Planet* planet, char* dimension);




/**
 * Deletes a Jerry object and all its associated data except the planet and origin as it may be associated
 * with another Jerry.
 * Frees the memory allocated for the Jerry's ID, Origin, characteristics array and Jerry itself.
 * Handles all dynamic memory within the Jerry structure.
 * @param jerry - Pointer to the Jerry object to delete.
 * @return status - success if deletion is successful, failure if jerry is NULL.
 */
status destroyJerry(Jerry* jerry);



/**
 * Checks if a Jerry has a specific physical characteristic by name.
 * @param jerry - Pointer to the Jerry object.
 * @param characteristic_name - Name of the characteristic to check.
 * @return bool - true if the characteristic exists, false otherwise.
 */
bool hasPhysicalCharacteristic(Jerry* jerry, char* characteristic_name);




/**
 * Adds a physical characteristic to a Jerry.
 * Dynamically resizes the Jerry's characteristics array and performs a deep copy of the provided characteristic.
 * Ensures no duplicate characteristics are added.
 * @param jerry - Pointer to the Jerry object.
 * @param characteristic - Pointer to the PhysicalCharacteristic object to add (deep copy will be made).
 * @return status - success if addition is successful, failure if input is NULL or characteristic already exists,
 * memory_problem if memory allocation fails.
 */
status addPhysicalCharacteristic(Jerry* jerry, PhysicalCharacteristic* characteristic);




/**
 * Removes a physical characteristic from a Jerry by name.
 * Does not free the memory of the characteristic as it may be shared among multiple Jerries.
 * Adjusts the Jerry's characteristics array by resizing it after removal.
 * @param jerry - Pointer to the Jerry object.
 * @param characteristic_name - Name of the characteristic to remove.
 * @return status:
 *  - success: if removal is successful.
 *  - failure: if jerry or characteristic_name is NULL, or if the characteristic does not exist.
 *  - memory_problem: if memory reallocation fails while resizing the array.
 */
status deletePhysicalCharacteristic(Jerry* jerry, char* characteristic_name);



/**
 * Gets a specific physical characteristic by name
 * @param jerry - pointer to Jerry
 * @param name - name of the characteristic to find
 * @return Pointer to the physical characteristic, or NULL if not found
 */
PhysicalCharacteristic* getPhysicalCharacteristic(Jerry* jerry, char* name);


/**
 * Prints all information about a Jerry object in the specified format.
 * Includes ID, happiness level, origin information, and all physical characteristics.
 * Format example:
 * Jerry , ID - [id] :
 * Happiness level : [happiness]
 * Origin : [dimension]
 * Planet : [name] (x.xx,y.yy,z.zz)
 * [If characteristics exist]:
 * Jerry's physical Characteristics available :
 *     [name1] : [value1] , [name2] : [value2] ...
 * @param jerry - Pointer to the Jerry object to print.
 * @return status - success if printing is successful, failure if jerry is NULL.
 */
status printJerry(Jerry* jerry);




#endif //JERRY_H
