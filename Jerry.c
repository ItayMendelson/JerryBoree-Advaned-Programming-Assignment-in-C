//
// Created by itaym on 29/11/2024.
//
#include "Jerry.h"

// planet functions
Planet* createPlanet(char* name, double x, double y, double z) {
    if (name == NULL) {
        return NULL;
    }
    Planet* planet = (Planet*)malloc(sizeof(Planet));
    if (planet == NULL) {
        return NULL;
    }
    planet->name = (char*)malloc(strlen(name) + 1);
    if (planet->name == NULL) {
        free(planet);
        planet = NULL;
        return NULL;
    }
    strcpy(planet->name, name);
    planet->x = x;
    planet->y = y;
    planet->z = z;

    return planet;
}


status destroyPlanet(Planet* planet) {
    if (planet == NULL) {
        return failure;
    }
    free(planet->name);
    planet->name = NULL;
    free(planet);
    planet = NULL;
    return success;
}

status printPlanet(Planet* planet) {
    if (planet == NULL) {
        return failure;
    }
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", planet->name, planet->x, planet->y, planet->z);
    return success;
}

// origin functions

Origin* createOrigin(Planet* planet, char* dimension) {
    if (planet == NULL || dimension == NULL) {
        return NULL;
    }
    Origin* origin = (Origin*)malloc(sizeof(Origin));
    if (origin == NULL) {
        return NULL;
    }
    origin->dimension = (char*)malloc(strlen(dimension) + 1);
    if (origin->dimension == NULL) {
        free(origin);
        return NULL;
    }
    strcpy(origin->dimension, dimension);
    origin->planet = planet;
    return origin;
}



status destroyOrigin(Origin* origin) {
    if (origin == NULL) {
        return failure;
    }
    // notice we don't free the planet here, it might be shared by other Jerries
    free(origin->dimension);
    origin->dimension = NULL;
    free(origin);
    origin = NULL;
    return success;
}

// physical characteristics functions

PhysicalCharacteristic* createPhysicalCharacteristic(char* name, double value) {
    if (name == NULL) {
        return NULL;
    }
    // create the array that will store the object
    PhysicalCharacteristic* characteristic = (PhysicalCharacteristic*)malloc(sizeof(PhysicalCharacteristic));
    if (characteristic == NULL) {
        return NULL;
    }

    characteristic->name = (char*)malloc(strlen(name) + 1); // allocate memory for the name of the characteristic
    if (characteristic->name == NULL) {
        free(characteristic);
        characteristic = NULL;
        return NULL;
    }
    strcpy(characteristic->name, name);

    characteristic->value = value;

    return characteristic;
}


// Jerry functions

Jerry* createJerry(char* id, int happiness, Planet* planet, char* dimension) {
    if (id == NULL || planet == NULL || dimension == NULL) {
        return NULL;
    }

    if (happiness < 0 || happiness > 100) {
        return NULL;
    }


    Origin* origin = createOrigin(planet, dimension);
    if (origin == NULL) {
        return NULL;
    }


    Jerry* jerry = (Jerry*)malloc(sizeof(Jerry));
    if (jerry == NULL) {
        destroyOrigin(origin);
        return NULL;
    }
    jerry->id = (char*)malloc(strlen(id) + 1);
    if (jerry->id == NULL) {
        destroyOrigin(origin);
        free(jerry);
        jerry = NULL;
        return NULL;
    }
    strcpy(jerry->id, id);

    jerry->happiness = happiness;
    jerry->origin = origin;
    jerry->characteristics = NULL;
    jerry->num_characteristics = 0;
    return jerry;
}


bool hasPhysicalCharacteristic(Jerry* jerry, char* characteristic_name) {
    if (jerry == NULL || characteristic_name == NULL) {
        return false;
    }
    for (int i = 0; i < jerry->num_characteristics; i++) {
        if (strcmp(jerry->characteristics[i]->name, characteristic_name) == 0) {
            return true;
        }
    }
    return false;
}

status destroyPhysicalCharacteristic(PhysicalCharacteristic* characteristic) {
    if (characteristic == NULL) {
        return failure;
    }
    if (characteristic->name != NULL) {
        free(characteristic->name);
        characteristic->name = NULL;
    }
    free(characteristic);
    characteristic = NULL;
    return success;
}


status addPhysicalCharacteristic(Jerry* jerry, PhysicalCharacteristic* characteristic) {
    if (jerry == NULL || characteristic == NULL || characteristic->name == NULL) {
        return failure;
    }

    if (hasPhysicalCharacteristic(jerry, characteristic->name)) {
        return failure;
    }

    // resize array to hold one more characteristic
    PhysicalCharacteristic** temp =
    (PhysicalCharacteristic**)realloc(jerry->characteristics,
    (jerry->num_characteristics + 1) * sizeof(PhysicalCharacteristic*));
    if (temp == NULL) {
        return memory_problem; // memory allocation failed
    }
    jerry->characteristics = temp; // redirect the characteristics array pointer to the new array
    jerry->characteristics[jerry->num_characteristics] = characteristic; // add the new characteristic
    jerry->num_characteristics++; // update the new characteristics number
    return success;
}


status deletePhysicalCharacteristic(Jerry* jerry, char* characteristic_name) {

    if (jerry == NULL || characteristic_name == NULL) {
        return failure;
    }
    if (jerry->num_characteristics == 0) { // jerry doesn't have any characteristics
        return failure;
    }

    // find the characteristic to delete
    int found_index = -1;
    for (int i = 0; i < jerry->num_characteristics; i++) {
        if (strcmp(jerry->characteristics[i]->name, characteristic_name) == 0) {
            found_index = i; // we found the index of the characteristic
            break;
        }
    }
    if (found_index == -1) { // requested characteristic not found
        return failure;
    }
    destroyPhysicalCharacteristic(jerry->characteristics[found_index]);
    jerry->characteristics[found_index] = NULL;

    for (int i = found_index + 1; i < jerry->num_characteristics; i++) {
        jerry->characteristics[i-1] = jerry->characteristics[i]; // update the pointers
    }
    jerry->num_characteristics--; // decrease the number of characteristics by 1

    // reallocate array to new size or free if empty
    if (jerry->num_characteristics > 0) { // then we need a pointer to the new sized array
        PhysicalCharacteristic** temp = (PhysicalCharacteristic**)realloc(jerry->characteristics, jerry->num_characteristics * sizeof(PhysicalCharacteristic*));
        if (temp == NULL) {
            return memory_problem;
        }
        jerry->characteristics = temp;
    }
    else { // no characteristics left so we should free the array
        free(jerry->characteristics);
        jerry->characteristics = NULL;
    }

    return success;
}


PhysicalCharacteristic* getPhysicalCharacteristic(Jerry* jerry, char* name) {
    if (!jerry || !name) return NULL;

    for (int i = 0; i < jerry->num_characteristics; i++) {
        if (strcmp(jerry->characteristics[i]->name, name) == 0) {
            return (jerry->characteristics[i]);
        }
    }
    return NULL;
}

status printJerry(Jerry* jerry) {
    if (jerry == NULL) {
        return failure;
    }

    printf("Jerry , ID - %s : \n", jerry->id);
    printf("Happiness level : %d \n", jerry->happiness);
    printf("Origin : %s \n", jerry->origin->dimension);
    printf("Planet : %s (%.2f,%.2f,%.2f) \n",
           jerry->origin->planet->name,
           jerry->origin->planet->x,
           jerry->origin->planet->y,
           jerry->origin->planet->z);

    if (jerry->num_characteristics > 0) {
        printf("Jerry's physical Characteristics available : \n");
        printf("\t");
        for (int i = 0; i < jerry->num_characteristics; i++) {
            printf("%s : %.2f", jerry->characteristics[i]->name, jerry->characteristics[i]->value);
            if (i < jerry->num_characteristics - 1) {
                printf(" , ");
            }
        }
        printf(" \n");
    }
    return success;
}


status destroyJerry(Jerry* jerry) {
    if (jerry == NULL) {
        return failure;
    }
    if (jerry->id != NULL) {
        free(jerry->id);
        jerry->id = NULL;
    }

    if (jerry->origin != NULL) {
        destroyOrigin(jerry->origin);
        jerry->origin = NULL;
    }
    if (jerry->characteristics != NULL) {
        for (int i = 0; i < jerry->num_characteristics; i++) {
            if (jerry->characteristics[i] != NULL) {
                destroyPhysicalCharacteristic(jerry->characteristics[i]);
                jerry->characteristics[i] = NULL;
            }
        }
        free(jerry->characteristics); // free the characteristics array itself
        jerry->characteristics = NULL;
    }
    free(jerry);
    jerry = NULL;
    return success;
}

