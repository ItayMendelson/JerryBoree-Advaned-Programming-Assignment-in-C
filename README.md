# JerryBoree-Advanced-Programming-Assignment-in-C

## 🏠 JerryBoree - a Rick and Morty Advanced Programming Project

### Structure:
The project implements a Jerry daycare system using several generic data structures:
- **LinkedList**: Generic list implementation supporting basic operations
- **KeyValuePair**: Generic key-value pair storage
- **HashTable**: Generic hash table with chaining for collision resolution
- **MultiValueHashTable**: Generic hash table allowing multiple values per key

### Implementation Details ⚙️

#### LinkedList Design ⚡
- LinkedList uses internal caching to optimize sequential access operations!

#### HashTable Design 🎯
- Built using LinkedList for chaining and KeyValuePair for storage
- Uses modulo-based hashing with ASCII sum for strings
- O(1) average time complexity for operations
- Dynamic hash table sizing using closest prime numbers to the number of Jerries in the configuration file to ensure hashtable's efficiency
- All generic functionality handled through callback functions

#### MultiValueHashTable Design 🌟
- Implemented on top of HashTable
- Each value in HashTable is a LinkedList of values
- Maintains O(1) average lookup while supporting multiple values
- Reuses HashTable code to avoid duplication
- Generic value comparison for searching/deletion
- Size optimized based on prime number closest to the number of unique characteristics to ensure multihashtable's efficiency

#### JerryBoree Implementation 🏠
Our system uses a combination of data structures:
- **jerriesByID** (HashTable): O(1) Jerry lookup
- **jerriesByCharacteristics** (MultiValueHashTable): Physical characteristic grouping
- **jerries** (LinkedList): Insertion order tracking
- **planets** (LinkedList): Planet management

### Safe & Clean 🧹
- Pre-scan of configuration file to determine optimal sizes
- All memory managed carefully with statuses to maintain order
- No memory leaks and clean module separation (tested with Valgrind)
