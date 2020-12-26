all: GenericHashTable.c GenericHashTable.h main.c
	gcc -g -Wall -Wvla GenericHashTable.c main.c -o runfile
all-GDB: GenericHashTable.c GenericHashTable.h main.c
	gcc -g -Wall -Wvla GenericHashTable.c main.c -o runfile