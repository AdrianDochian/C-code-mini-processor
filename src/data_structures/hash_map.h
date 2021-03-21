#ifndef C_CODE_MINI_PROCESSOR_HASH_MAP_H
#define C_CODE_MINI_PROCESSOR_HASH_MAP_H

#include "../utils/headers.h"

#define HASH_MAP_SIZE 20

// STRUCT DEFINITIONS
typedef struct HashMapEntry {
    char *key;
    char *value;
    struct HashMapEntry *nextEntry;
} HashMapEntry;

typedef struct List {
    HashMapEntry *element;
} List;

typedef struct HashMap {
    int size;
    List *items;
} HashMap;

// FUNCTION DEFINITIONS
void initHashMap(HashMap **hashMap);

void freeHashMap(HashMap **hashMap);

void addToHashMap(HashMap *hashMap, char *key, char *value);

char *getValueFromHashMap(HashMap *hashMap, char *key);

char *getValueFromList(List *items, char *key);

void addToList(List *items, char *key, char *value);

void insertLast(HashMapEntry **head, char *key, char *value);

void freeList(HashMapEntry **head);

HashMapEntry *getHashMapEntry(char *key, char *value);

void printHashMap(HashMap *hashMap);

void printList(int index, HashMapEntry *head);

int hash(char *key);

#endif //C_CODE_MINI_PROCESSOR_HASH_MAP_H
