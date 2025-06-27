//
// Created by itaym on 20/12/2024.
//
#include "Jerry.h"
#include "KeyValuePair.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"
#include <math.h>
#define MAX_LINE_LENGTH 301



/**
 * JerryBoreeMain.c
 * Main program for managing the JerryBoree daycare system.
 * Handles user interaction and coordinates between different modules.
 */
typedef struct JerryBoree_t {

    hashTable jerriesByID; // fast Jerry lookup by ID

    MultiValueHashTable jerriesByCharacteristics; // group Jerries by characteristics

    LinkedList jerries; // LinkedList maintaining insertion order

    LinkedList planets; // store known planets

} JerryBoree;




/** helper functions for the hash tables **/


/* Jerry copy, free, print and isEqual functions */


// wrapper for copying Jerry as generic Element
static Element copyJerryShallow(Element jerry) {
    if (!jerry) {
        return NULL;
    }
    return (Jerry*)jerry;
}


// not actually freeing the Jerry object, as we shared pointers
static status freeJerryPtr(Element jerry) {
    return success; // Don't free here - managed separately
}


// wrapper for destroying Jerry as generic Element
status destroyJerryElement(Element elem) {
    if (!elem) return failure;
    return destroyJerry((Jerry*)elem);
}

// wrapper for Jerry ID's comparison
static bool isEqualJerryIDElement(Element elem1, Element elem2) {
    if (!elem1 || !elem2) {
        return false;
    }
    return strcmp((char*)elem1, (char*)elem2) == 0;
}


// wrapper for printing Jerry
static status printJerryElement(Element jerry) {
    if (!jerry) {
        return null_pointer;
    }
    return printJerry((Jerry*)jerry);
}






// transformation function for the id and characteristics
static int transformStringHash(Element string) {
    if (!string) {
        return -1;
    }
    char* str = (char*) string;
    int sum = 0;
    while (*str != '\0') {
        sum += *str;
        str++;
    }
    return sum;
}





// string copy, free, print and isEqual functions
static Element copyString(Element str) {
    if (!str) {
        return NULL;
    }
    char* copy = strdup((char*)str);  // Make a deep copy
    return copy;
}

// helper function to print the Physical characteristic name properly
static status print_pc_name(Element pc_name) {
    if (!pc_name) return failure;
    printf("%s : \n", (char*)pc_name);  // add the " : " after the ID
    return success;
}

// FreeFunction for strings
static status freeString(Element str) {
    if (!str) {
        return null_pointer;
    }
    free(str);  // actually free the copied string
    return success;
}

// PrintFunction for strings
static status printString(Element str) {
    if (!str) return null_pointer;
    printf("%s", (char*)str);
    return success;
}

// EqualFunction for strings
static bool isEqualString(Element str1, Element str2) {
    if (!str1 || !str2) {
        return false;
    }
    return strcmp((char*)str1, (char*)str2) == 0;
}





// planets copy, free, print and isEqual functions
static Element copyPlanet(Element planet) {
    if (!planet) {
        return NULL;
    }
    return planet;
}

// FreeFunction for planets
static status freePlanet(Element planet) {
    if (!planet) {
        return null_pointer;
    }
    return destroyPlanet(planet);  // Don't free here - managed in main program
}

// PrintFunction for planets
static status printPlanetPtr(Element planet) {
    if (!planet) return failure;
    return printPlanet((Planet*)planet);
}

// EqualFunction for planets
static bool isEqualPlanet(Element elem1, Element elem2) {
    if (!elem1 || ! elem2) {
        return false;
    }
    return strcmp(((Planet*)elem1)->name, ((char*)elem2)) == 0;
}

/* clear buffer function to help with user input */

void clearBuffer() {
    int c;
    do { // reset the buffer
        c = getchar();
    } while (c != '\n' && c != EOF);
}







/** JerryBoree functions **/


// function to check if a given natural number is a prime number
bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n == 2 || n == 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) { // if n > 2, it cannot be divided by 2 or 3
        return false;
    }
    // as 5 is the next prime number
    for (int i = 5; i * i <= n; i += 6) { // every prime number larger than 6 can be expressed as (6k +- 1)
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}


// Function to find the closest largest or equal prime number
int find_closest_prime(int number) {
    if (number < 2) {
        return 2;  // the smallest prime number
    }
    int candidate = number;
    while (!is_prime(candidate)) { // if the number isn't a prime number, try the next number
        candidate++;
    }
    return candidate;
}



JerryBoree* initJerryBoree(int tableSize, int multiTableSize) {

    JerryBoree* DayCare = (JerryBoree*)malloc(sizeof(JerryBoree));
    if (!DayCare) {
        return NULL;
    }



    // HashTable creation
    DayCare->jerriesByID = createHashTable(copyString, freeString, printString,
                                                    copyJerryShallow, freeJerryPtr, printJerryElement,
                                           isEqualJerryIDElement, transformStringHash, tableSize);
    if (!DayCare->jerriesByID) {
        free(DayCare);
        return NULL;
    }


    // MultiValueHashTable creation
    DayCare->jerriesByCharacteristics = createMultiValueHashTable(copyString, freeString, print_pc_name,
                                                                copyJerryShallow, freeJerryPtr, printJerryElement, isEqualString,
                                                                isEqualJerryIDElement, transformStringHash, multiTableSize);
    if (!DayCare->jerriesByCharacteristics) {
        destroyHashTable(DayCare->jerriesByID);
        free(DayCare);
        return NULL;
    }


    // Planets LinkedList creation
    DayCare->planets = createLinkedList(copyPlanet, freePlanet, printPlanetPtr, isEqualPlanet);
    if (!DayCare->planets) {
        destroyHashTable(DayCare->jerriesByID);
        destroyMultiValueHashTable(DayCare->jerriesByCharacteristics);
        free(DayCare);
        return NULL;
    }



    // Jerries LinkedList creation
    DayCare->jerries = createLinkedList(copyJerryShallow, destroyJerryElement, printJerryElement,isEqualJerryIDElement);
    if (!DayCare->jerries) {
        destroyHashTable(DayCare->jerriesByID);
        destroyMultiValueHashTable(DayCare->jerriesByCharacteristics);
        destroyList(DayCare->planets);
        free(DayCare);
        return NULL;
    }

    return DayCare;
}



void destroyJerryBoree(JerryBoree** daycare) {
    if (!daycare) {
        return;
    }
    JerryBoree* DayCare = *daycare;


    if (DayCare->jerries) {
        destroyList(DayCare->jerries);
    }

    if (DayCare->planets) {
        destroyList(DayCare->planets);
    }

    if (DayCare->jerriesByCharacteristics) {
        destroyMultiValueHashTable(DayCare->jerriesByCharacteristics);
    }

    if (DayCare->jerriesByID) {
        destroyHashTable(DayCare->jerriesByID);
    }
    free(DayCare);
    *daycare = NULL;
}




/** read from configuration file **/


// counts the number of jerries and the number of unique physical characteristics names in the configuration file
status countFileStats(FILE* fp, int* numJerries, int* numUniqueCharacteristics) {
    if (!fp || !numJerries || !numUniqueCharacteristics) {
        return null_pointer;
    }

    *numJerries = 0;
    char line[MAX_LINE_LENGTH];
    char seen[MAX_LINE_LENGTH][MAX_LINE_LENGTH];  // store unique characteristic names
    int seenCount = 0;  // number of unique characteristics found so far

    rewind(fp); // reset file pointer to start


    // skip until Jerries section
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "Jerries")) break;
    }

    // process each line in Jerries section
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0; // remove newline character
        if (strlen(line) == 0) continue;

        if (line[0] == '\t') {   // if line starts with tab = characteristic line
            char charName[MAX_LINE_LENGTH];
            if (sscanf(line + 1, "%[^:]", charName) == 1) { // extract characteristic's name
                // check if characteristic is new
                bool found = false;
                for (int i = 0; i < seenCount && !found; i++) {
                    found = (strcmp(seen[i], charName) == 0);
                }
                if (!found) strcpy(seen[seenCount++], charName); // if not seen before, add to the seen count
            }
        } else if (strchr(line, ',')) {  // if line contains "," so it's a Jerry line
            (*numJerries)++;
        }
    }

    *numUniqueCharacteristics = seenCount; // store final count
    rewind(fp);
    return success;
}


// add a given jerry to the Jerries LinkedList and the Jerry ID's HashTable
status addJerryToStructs(JerryBoree* daycare, Jerry* new_jerry) {
    if (!daycare) {
        return null_pointer;
    }
    // add Jerry to the structures
    status jerry_insertion = appendNode(daycare->jerries, new_jerry);
    if (jerry_insertion != success) {
        destroyJerry(new_jerry); // clean if append fails
        return jerry_insertion;
    }
    status hashtable_insertion = addToHashTable(daycare->jerriesByID, new_jerry->id, new_jerry);
    if (hashtable_insertion != success) {
        deleteNode(daycare->jerries, new_jerry);
        return jerry_insertion;
    }
    return success;
}


static status loadPlanets(JerryBoree* boree, FILE* fp, int numPlanets) {
    char line[MAX_LINE_LENGTH];

    // skip "Planets" header
    if (!fgets(line, sizeof(line), fp)) return failure;

    // read each planet
    for (int i = 0; i < numPlanets; i++) {
        if (!fgets(line, sizeof(line), fp)) return failure;

        // remove newline
        line[strcspn(line, "\n")] = 0; // remove newline

        char name[MAX_LINE_LENGTH];
        float x, y, z;

        // parse planet line
        if (sscanf(line, "%[^,],%f,%f,%f", name, &x, &y, &z) != 4)
            return failure;

        Planet* planet = createPlanet(name, x, y, z);
        if (!planet) return failure;

        // add to planets list - it will just store the pointer
        if (appendNode(boree->planets, planet) != success) {
            destroyPlanet(planet);
            return failure;
        }

        // not destroying planet here since we're keeping the original instance
    }

    return success;
}
static status loadJerries(JerryBoree* daycare, FILE* fp) {
    char line[MAX_LINE_LENGTH];
    Jerry* currentJerry = NULL;  // keep track of last created Jerry for characteristics

    // skip "Jerries" header
    if (!fgets(line, sizeof(line), fp)) return failure;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        // if line starts with tab, it's a characteristic for current Jerry
        if (line[0] == '\t') {
            if (!currentJerry) return failure;

            char name[MAX_LINE_LENGTH];
            double value;

            if (sscanf(line + 1, "%[^:]:%lf", name, &value) != 2)
                return failure;

            // add characteristic to Jerry
            PhysicalCharacteristic* pc = createPhysicalCharacteristic(name, value);
            if (!pc) return memory_problem;
            status result = addPhysicalCharacteristic(currentJerry, pc);
            if (result != success) {
                destroyPhysicalCharacteristic(pc);
                return failure;
            }

            // add to characteristic lookup
            result = addToMultiValueHashTable(daycare->jerriesByCharacteristics, name, currentJerry);
            if (result != success) {
                return failure;
            }

        } else {
            // this is a new Jerry line
            char id[MAX_LINE_LENGTH], dimension[MAX_LINE_LENGTH], planetName[MAX_LINE_LENGTH];
            int happiness;

            if (sscanf(line, "%[^,],%[^,],%[^,],%d",
                      id, dimension, planetName, &happiness) != 4)
                return failure;

            // find planet from linked list
            Planet* planet = searchByKeyInList(daycare->planets, planetName);
            if (!planet) return failure;
            // create new Jerry
            currentJerry = createJerry(id, happiness, planet, dimension);
            if (!currentJerry) return failure;

            status add_state = addJerryToStructs(daycare, currentJerry);
            if (add_state != success) {
                return add_state;
            }
        }
    }

    return success;
}

status loadConfigurationFile(JerryBoree* boree, const char* filename, int numPlanets) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return failure;

    status result = loadPlanets(boree, fp, numPlanets);
    if (result == success) {
        result = loadJerries(boree, fp);
    }

    fclose(fp);
    return result;
}



/** menu functions **/


void printMenu() {
    printf("Welcome Rick, what are your Jerry's needs today ? \n");
    printf("1 : Take this Jerry away from me \n");
    printf("2 : I think I remember something about my Jerry \n");
    printf("3 : Oh wait. That can't be right \n");
    printf("4 : I guess I will take back my Jerry now \n");
    printf("5 : I can't find my Jerry. Just give me a similar one \n");
    printf("6 : I lost a bet. Give me your saddest Jerry \n");
    printf("7 : Show me what you got \n");
    printf("8 : Let the Jerries play \n");
    printf("9 : I had enough. Close this place \n");

}

void printOption7Menu() {
    printf("What information do you want to know ? \n");
    printf("1 : All Jerries \n");
    printf("2 : All Jerries by physical characteristics \n");
    printf("3 : All known planets \n");
}

void printOption8Menu() {
    printf("What activity do you want the Jerries to partake in ? \n");
    printf("1 : Interact with fake Beth \n");
    printf("2 : Play golf \n");
    printf("3 : Adjust the picture settings on the TV \n");
}



/** Utilities functions **/

// removes Jerry from all structures and frees memory accordingly
status deleteJerryFromStructures(JerryBoree* daycare, Jerry* jerry) {
    if (!daycare || !jerry) return null_pointer;


    // delete all jerries associate
    int numChars = jerry->num_characteristics;
    for (int i = 0; i < numChars; i++) {
        PhysicalCharacteristic* pc = jerry->characteristics[i];
        if (pc) {
            removeFromMultiValueHashTable(daycare->jerriesByCharacteristics,
                                        pc->name, jerry);
        }
    }

    // remove Jerry from id's hash table
    status jerries_id_state = removeFromHashTable(daycare->jerriesByID, jerry->id);
    if (jerries_id_state != success) {
        return jerries_id_state;
    }
    status jerry_delete = deleteNode(daycare->jerries, jerry);
    if (jerry_delete != success) {
        return jerry_delete;
    }

    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return success;
}

// finds Jerry through the Jerry's ID hashtable
Jerry* findJerryByID(JerryBoree* daycare, char* jerry_id) {
    if (!daycare || !jerry_id) return NULL;
    // get Jerry ID
    char id[MAX_LINE_LENGTH];
    printf("What is your Jerry's ID ? \n");
    scanf("%s", id);
    clearBuffer();
    strcpy(jerry_id, id);

    // look up Jerry
    Jerry* jerry = lookupInHashTable(daycare->jerriesByID, id);
    if (!jerry) return NULL;
    return jerry;
}

// calculates the updated happiness level of a given Jerry and keeps it in bound
static int adjustHappiness(int current_happiness, int change) {
    int newHappiness = current_happiness + change;
    if (newHappiness > 100) newHappiness = 100;
    if (newHappiness < 0) newHappiness = 0;
    return newHappiness;
}

status addJerryToDayCare(JerryBoree* daycare) {
    if (!daycare) return null_pointer;

    // get Jerry's ID
    char id[MAX_LINE_LENGTH];
    Jerry* jerry = findJerryByID(daycare, id);
    if (jerry) { // Jerry is already in the daycare!
        printf("Rick did you forgot ? you already left him here ! \n");
        return success;
    }
    // Jerry not in daycare, so we need to insert him
    char planet_name[MAX_LINE_LENGTH];
    printf("What planet is your Jerry from ? \n");
    scanf("%s", planet_name);
    clearBuffer();

    // search for planet in the planets LinkedList
    Planet* planet= searchByKeyInList(daycare->planets, planet_name);
    if (!planet) { // planet not found in planets
        printf("%s is not a known planet ! \n", planet_name);
        return success;
    }

    // gather parameters for Jerry's creation
    char dimension[MAX_LINE_LENGTH];
    int happiness;
    printf("What is your Jerry's dimension ? \n");
    scanf("%s", dimension);
    clearBuffer();


    printf("How happy is your Jerry now ? \n");
    scanf("%d", &happiness);
    clearBuffer();

    Jerry* new_jerry = createJerry(id, happiness, planet, dimension);
    if (!new_jerry) return memory_problem;
    printJerry(new_jerry);

    // add Jerry to the structures
    return addJerryToStructs(daycare, new_jerry);
    }



status addPhysCharToJerry(JerryBoree* daycare) {
    if (!daycare) return null_pointer;

    // get Jerry ID
    char id[MAX_LINE_LENGTH];
    Jerry* jerry = findJerryByID(daycare, id);

    if (!jerry) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return success;
    }

    // get characteristic name
    char pc_name[MAX_LINE_LENGTH];
    printf("What physical characteristic can you add to Jerry - %s ? \n", id);
    scanf("%s", pc_name);  // Add this line to get the characteristic name
    clearBuffer();

    // check if characteristic exists
    status has_pc = hasPhysicalCharacteristic(jerry, pc_name);
    if (has_pc) {
        printf("The information about his %s already available to the daycare ! \n", pc_name);
        return success;
    }

    // get characteristic value
    double value;  // changed to double since physical characteristics are real numbers
    printf("What is the value of his %s ? \n", pc_name);
    scanf("%lf", &value);
    clearBuffer();

    // create and add the characteristic
    PhysicalCharacteristic* pc = createPhysicalCharacteristic(pc_name, value);
    if (!pc) return memory_problem;

    status add_pc_to_jerry = addPhysicalCharacteristic(jerry, pc);
    if (add_pc_to_jerry != success) {
        destroyPhysicalCharacteristic(pc);  // clean up if add fails
        return add_pc_to_jerry;
    }

    status add_to_mvht = addToMultiValueHashTable(daycare->jerriesByCharacteristics, pc_name, jerry);
    if (add_to_mvht != success) {
        return add_to_mvht;
    }

    // display all Jerries with this characteristic
    return displayMultiValueHashElementsByKey(daycare->jerriesByCharacteristics, pc_name);
}



status deletePhysCharFromJerry(JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    char id[MAX_LINE_LENGTH];
    Jerry* jerry = findJerryByID(daycare, id);
    if (!jerry) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return success;
    }
     // get characteristic name
    char pc_name[MAX_LINE_LENGTH];
    printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id);
    scanf("%s", pc_name);  // add this line to get the characteristic name
    clearBuffer();

    status has_pc = hasPhysicalCharacteristic(jerry, pc_name);
    if (!has_pc) { // we can't remove the characteristic if jerry doesn't have it
        printf("The information about his %s not available to the daycare ! \n", pc_name);
        return success;
    }
    status delete_mvht = removeFromMultiValueHashTable(daycare->jerriesByCharacteristics, pc_name, jerry);
    if (delete_mvht != success) {
        return delete_mvht;
    }
    status delete_state = deletePhysicalCharacteristic(jerry, pc_name);
    if (delete_state != success) {
        return delete_state;
    }
    printJerry(jerry);
    return success;
}


status removeJerryFromDaycare(JerryBoree* daycare) {

    if (!daycare) return null_pointer;

    // search for Jerry
    char id[MAX_LINE_LENGTH];
    Jerry* jerry = findJerryByID(daycare, id);
    if (!jerry) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return success;
    }

    status structures_removal = deleteJerryFromStructures(daycare, jerry);
    if (structures_removal != success) {
        return structures_removal;
    }
    return success;
}


status removeSimilarJerry(JerryBoree* daycare) {
    if (!daycare) return null_pointer;


    char pc_name[MAX_LINE_LENGTH];
    printf("What do you remember about your Jerry ? \n");
    scanf("%s", pc_name);
    clearBuffer();


    // the list contains all jerries with the requested physical characteristics
    LinkedList jerries_with_pc = lookupInMultiValueHashTable(daycare->jerriesByCharacteristics, pc_name);


    if (!jerries_with_pc) { // if characteristic not found
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc_name);
        return success;
    }

    double target_value;
    printf("What do you remember about the value of his %s ? \n", pc_name);
    scanf("%lf", &target_value);
    clearBuffer();


    // we need to search in the list for the characteristic
    int list_length = getLengthList(jerries_with_pc);
    double smallest_diff = -1; //
    Jerry* closest_jerry = NULL;
    for (int i = 1; i < list_length + 1; i++) {
        Jerry* curr_jerry = getDataByIndex(jerries_with_pc, i);
        if (!curr_jerry) {
            continue;
        }
        PhysicalCharacteristic* pc = getPhysicalCharacteristic(curr_jerry, pc_name);
        if (!pc) {
            continue;
        }
        double curr_diff = fabs(pc->value - target_value); // absolute value
        if (smallest_diff < 0 || curr_diff < smallest_diff) {
            smallest_diff = curr_diff;
            closest_jerry = curr_jerry;
        }
    }

    if (closest_jerry) {
        printf("Rick this is the most suitable Jerry we found : \n");
        printJerry(closest_jerry);
        status structures_removal = deleteJerryFromStructures(daycare, closest_jerry);
        if (structures_removal != success) {
            return structures_removal;
        }
    }
    return success;
}


status removeSaddestJerry(JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    if (getLengthList(daycare->jerries) == 0) { // no jerries in the daycare
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return success;
    }
    LinkedList all_jerries = daycare->jerries;
    Jerry* saddest_jerry = getDataByIndex(all_jerries, 1);
    if (!saddest_jerry) {
        return failure;
    }


    int saddest_level = saddest_jerry->happiness;
    for (int i =2; i < getLengthList(all_jerries) + 1; i++) {
        Jerry* curr_jerry = getDataByIndex(all_jerries, i);
        if (!curr_jerry) {
            continue;
        }
        if (curr_jerry->happiness < saddest_level) { // update the saddest Jerry
            saddest_jerry = curr_jerry;
            saddest_level = curr_jerry->happiness;
        }
    }
    if (saddest_jerry) {
        printf("Rick this is the most suitable Jerry we found : \n");
        printJerry(saddest_jerry);
    }

    status structures_removal = deleteJerryFromStructures(daycare, saddest_jerry);
    if (structures_removal != success) {
        return structures_removal;
    }
    return success;
}


status printAllJerries(JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    if (getLengthList(daycare->jerries) == 0) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return success;
    }
    return displayList(daycare->jerries);
}

status printJerriesByPhysicalCharacteristic(JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    char pc_name[MAX_LINE_LENGTH];
    printf("What physical characteristics ? \n");
    scanf("%s", pc_name);
    clearBuffer();

    LinkedList jerries_with_pc = lookupInMultiValueHashTable(daycare->jerriesByCharacteristics, pc_name);
    if (!jerries_with_pc || getLengthList(jerries_with_pc) == 0) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc_name);
        return success;
    }
    return displayMultiValueHashElementsByKey(daycare->jerriesByCharacteristics, pc_name);

}

status showInformation(JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    char choice[MAX_LINE_LENGTH];
    printOption7Menu();
    scanf("%s", choice);
    clearBuffer();

    if (strlen(choice) != 1 || choice[0] < '1' || choice[0] > '3') {
        printf("Rick this option is not known to the daycare ! \n");
        return success;  // return to main menu
    }

    switch (choice[0]) {
        case '1': { // all Jerries
            return printAllJerries(daycare);
        }

        case '2': { // all Jerries by physical characteristics
            return printJerriesByPhysicalCharacteristic(daycare);
        }

        case '3': { // all planets
            return displayList(daycare->planets);
        }

        default: {
            return success;
        }
    }
}

status JerriesPlayWithBeth(JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    for (int i = 1; i < getLengthList(daycare->jerries) + 1; i++) {
        Jerry* jerry = getDataByIndex(daycare->jerries, i);
        if (!jerry) continue;
        if (jerry->happiness >= 20) {
            jerry->happiness = adjustHappiness(jerry->happiness, 15); // +15 if happiness >= 20

        }
        else {
            jerry->happiness = adjustHappiness(jerry->happiness, -5);  // -5 if happiness < 20
        }

    }
    return success;
}

status JerriesPlayGolf (JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    for (int i = 1; i < getLengthList(daycare->jerries) + 1; i++) {
        Jerry* jerry = getDataByIndex(daycare->jerries, i);
        if (!jerry) continue;

        if (jerry->happiness >= 50) { // +10 if happiness >= 50
            jerry->happiness = adjustHappiness(jerry->happiness, 10);
        }
        else { // +10 if happiness >= 50
            jerry->happiness = adjustHappiness(jerry->happiness, -10);
        }
    }
    return success;
}

status JerriesAdjustTV(JerryBoree* daycare) {
    if (!daycare) return null_pointer;
    for (int i = 1; i < getLengthList(daycare->jerries) + 1; i++) {
        Jerry* jerry = getDataByIndex(daycare->jerries, i);
        if (!jerry) continue;

        jerry->happiness = adjustHappiness(jerry->happiness, 20);
    }
    return success;

}


status letJerriesPlay(JerryBoree* daycare) {

    if (!daycare) return null_pointer;

    // check if there are any Jerries
    if (getLengthList(daycare->jerries) == 0) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return success;
    }

    // show activity menu and get choice
    char choice[MAX_LINE_LENGTH];
    printOption8Menu();
    scanf("%s", choice);
    clearBuffer();

    if (strlen(choice) != 1 || choice[0] < '1' || choice[0] > '3') {
        printf("Rick this option is not known to the daycare ! \n");
        return success;  // return to main menu
    }


    switch (choice[0]) {
        case '1': { // play with fake beth
            JerriesPlayWithBeth(daycare);
            break;
        }
        case '2': { // play golf
            JerriesPlayGolf(daycare);
            break;
        }
        case '3': { // adjust tv settings
            JerriesAdjustTV(daycare);
            break;
        }

        default: {
            return success;
        }
    }

    // print activity completion and updated Jerry states
    printf("The activity is now over ! \n");
    return displayList(daycare->jerries);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <number_of_planets> <configuration_file>\n", argv[0]);
        return 1;
    }

    int numberOfPlanets = atoi(argv[1]);
    const char* configFile = argv[2];

    FILE* fp = fopen(argv[2], "r");
    if (!fp) return 1;
    int numJerries = 0;
    int numUniquePcs = 0;
    countFileStats(fp, &numJerries, &numUniquePcs);
    fclose(fp);


    // make sure minimum size of 2 for hash tables
    int jerriesTableSize = find_closest_prime(numJerries ? numJerries : 2);
    int characteristicsTableSize = find_closest_prime(numUniquePcs ? numUniquePcs : 2);



    // initialize JerryBoree
    JerryBoree* daycare = initJerryBoree(jerriesTableSize, characteristicsTableSize);
    if (!daycare) {
        printf("A memory problem has been detected in the program \n");
        return 1;
    }


    // load configuration file

    status result = loadConfigurationFile(daycare, configFile, numberOfPlanets);
    if (result != success) {
        printf("A memory problem has been detected in the program\n"
       "Failed to load configuration file '%s'.\n"
       "Please ensure the file exists, is properly formatted,\n"
       "and that the number of planets you provided matches the number listed in the file.\n",
       argv[2]);
        destroyJerryBoree(&daycare);
        return 1;
    }

    // main menu loop

    bool running = true;


    while (running) {
        printMenu();
        char choice[MAX_LINE_LENGTH];
        scanf("%s", choice);
        clearBuffer();

        if (strlen(choice) != 1 || choice[0] < '1' || choice[0] > '9') {
            printf("Rick this option is not known to the daycare ! \n");
            continue;
        }

        switch(choice[0]) {

            case '1': {
                result = addJerryToDayCare(daycare);
                break;
            }

            case '2': {
                result = addPhysCharToJerry(daycare);
                break;
            }

            case '3': {
                result = deletePhysCharFromJerry(daycare);
                break;
            }

            case '4': {
                result = removeJerryFromDaycare(daycare);
                break;
            }

            case '5': {
                result = removeSimilarJerry(daycare);
                break;
            }

            case '6': {
                result = removeSaddestJerry(daycare);
                break;
            }

            case '7': {
                result = showInformation(daycare);
                break;
            }

            case '8': {
                result = letJerriesPlay(daycare);
                break;
            }


            case '9': {
                printf("The daycare is now clean and close ! \n");
                running = false;
                break;
            }

            default: {
                continue;
            }
        }


        // check for memory problems
        if (result != success) {
            printf("A memory problem has been detected in the program \n");
            destroyJerryBoree(&daycare);
            return 1;
        }
    }

    // clean up and exit
    destroyJerryBoree(&daycare);
    return 0;
}

