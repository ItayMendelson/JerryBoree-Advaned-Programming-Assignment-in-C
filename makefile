JerryBoree: JerryBoreeMain.o MultiValueHashTable.o HashTable.o KeyValuePair.o LinkedList.o Jerry.o
	gcc JerryBoreeMain.o MultiValueHashTable.o HashTable.o KeyValuePair.o LinkedList.o Jerry.o -o JerryBoree

JerryBoreeMain.o: JerryBoreeMain.c Jerry.h Defs.h KeyValuePair.h \
 LinkedList.h HashTable.h MultiValueHashTable.h
	gcc -c JerryBoreeMain.c
HashTable.o: HashTable.c HashTable.h Defs.h LinkedList.h KeyValuePair.h
	gcc -c HashTable.c
Jerry.o: Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c
KeyValuePair.o: KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c
LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c
MultiValueHashTable.o: MultiValueHashTable.c MultiValueHashTable.h \
 LinkedList.h Defs.h HashTable.h KeyValuePair.h
	gcc -c MultiValueHashTable.c
clean:
	rm *.o JerryBoree
