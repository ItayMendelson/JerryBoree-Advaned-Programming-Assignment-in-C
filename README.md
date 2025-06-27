# 🏠 JerryBoree - A Rick and Morty Advanced Programming Project in C

Welcome to **JerryBoree**, a Jerry daycare simulation based on the *Rick and Morty* universe.\
This project was created as part of the Advanced Programming course and demonstrates the use of **generic data structures**, memory management, and modular design in C.

---

## ⚙️ Setup & Compilation

To build the project, simply run:

```bash
make
```

This will compile all components and generate the `JerryBoree` executable.

### 🏃 Running the Program

```bash
./JerryBoree <number_of_planets> <configuration_file>
```

- `<number_of_planets>`: The number of planets expected in the configuration file.
- `<configuration_file>`: A valid text file that defines the planets and Jerries in the system.

📌 **Note**: Make sure the number of planets you provide matches exactly the number defined in the configuration file, or the program will fail to load.

---

## 📄 Configuration File Format

The configuration file must follow this structure:

```
Planets
<planetName>,<x>,<y>,<z>
...
Jerries
<id>,<dimension>,<planetName>,<happinessLevel>
<TAB><characteristicName>:<value>
<TAB><characteristicName>:<value>
...
```

### ✅ Example:

```
Planets
Earth,123.1,8392,99.2
Pluto,3454.21,124.112,985.445
Jerries
23dF21,C-137,Earth,50
	Height:166.2
	LimbsNumber:4
1q456,C-455,Earth,10
```

-Please note – the beginning of the known stars in the system should be identified by the title Planets.
- After it, a list of the stars will appear according to the quantity given in the run line.
- After the Planets section, the file must include a Jerries section listing all known Jerries in the required format.
- Use a tab character (`\t`) for physical characteristics.
- Planet names in Jerries section must match those in the Planets section.
- No duplicate planets or Jerries.

---

## 🧱 Project Structure

The project is composed of the following components:

```
project/
├── Defs.h                     # Common typedefs and enums
├── Jerry.h / Jerry.c          # Jerry entity
├── LinkedList.h / .c          # Generic LinkedList implementation
├── KeyValuePair.h / .c        # Generic Key-Value structure
├── HashTable.h / .c           # Generic Hash Table
├── MultiValueHashTable.h / .c # Hash Table supporting multiple values per key
├── JerryBoreeMain.c           # Main logic and interaction
├── makefile                   # Build configuration
```
- You will need the path for  configuration_file.txt  -  A valid configuration file. name as you wish :)
---

## 💡 Implementation Highlights

### 🔁 LinkedList

- Generic and flexible.
- Optimized with internal caching for sequential operations.

### 🔐 HashTable

- Built with chaining via LinkedList.
- Hashing by ASCII sum + modulo.
- Dynamic sizing via nearest prime to optimize efficiency.
- Supports generic callbacks for full flexibility.

### 🌈 MultiValueHashTable

- Built atop HashTable.
- Each key maps to a list of values.
- Reuses existing logic with minimal duplication.
- Scales well even with many shared physical traits.

### 🏠 JerryBoree System

- `jerriesByID` – `HashTable` for O(1) Jerry lookup
- `jerriesByCharacteristics` – `MultiValueHashTable` for grouping by traits
- `jerries` – `LinkedList` to maintain insertion order
- `planets` – `LinkedList` for planet info

---

## 🧼 Clean Code & Memory Safety

- Full memory lifecycle management via `status` return codes.
- No memory leaks (validated using **Valgrind**).
- Careful pointer and null checks throughout.
- All structures are modular and independently testable.

---

Made with care by Itay Mendelson\
Ben-Gurion University | Advanced Programming Course

---

## 📌 License

This project is for academic purposes. 

