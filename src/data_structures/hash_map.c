#include "../utils/headers.h"

void initHashMap(HashMap **hashMap) {
    *hashMap = malloc(sizeof(HashMap));
    (*hashMap)->size = HASH_MAP_SIZE;
    (*hashMap)->items = (List *) malloc((*hashMap)->size * sizeof(List));

    for (int i = 0; i < (*hashMap)->size; i++) {
        (*hashMap)->items[i].element = NULL;
    }
}

void freeHashMap(HashMap **hashMap) {
    for (int i = 0; i < (*hashMap)->size; i++) {
        freeList(&((*hashMap)->items[i].element));
    }

    free((*hashMap)->items);
    free(*hashMap);
}

void removeFromList(List *items, char *key) {
    HashMapEntry *temp = items->element;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            free(temp->value);
            temp->value = NULL;
            return;
        }
        temp = temp->nextEntry;
    }
}

void removeFromHashMap(HashMap *hashMap, char *key) {
    removeFromList(&(hashMap->items[hash(key)]), key);
}

void addToHashMap(HashMap *hashMap, char *key, char *value) {
    addToList(&(hashMap->items[hash(key)]), key, value);
}

char *getValueFromHashMap(HashMap *hashMap, char *key) {
    return getValueFromList(&(hashMap->items[hash(key)]), key);
}

char *getValueFromList(List *items, char *key) {
    HashMapEntry *temp = items->element;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            return temp->value;
        }
        temp = temp->nextEntry;
    }
    return NULL;
}

void addToList(List *items, char *key, char *value) {
    if (items->element == NULL) {
        items->element = getHashMapEntry(key, value);
    } else {
        insertLast(&(items->element), key, value);
    }
}

void insertLast(HashMapEntry **head, char *key, char *value) {
    HashMapEntry *temp = *head;

    // particular case of one element in a list that may be updated
    if (temp->nextEntry == NULL && strcmp(temp->key, key) == 0) {
        // update the value
        free(temp->value);
        temp->value = (char *) malloc((strlen(value) + 1) * sizeof(char));
        strcpy(temp->value, value);
        return;
    }

    while (temp->nextEntry != NULL) {
        // if already in hashmap
        if (strcmp(temp->key, key) == 0) {
            // update the value
            free(temp->value);
            temp->value = (char *) malloc((strlen(value) + 1) * sizeof(char));
            strcpy(temp->value, value);
            return;
        }

        // go find the last node or the existing key to replace the value
        temp = temp->nextEntry;
    }
    // the key was not found in the list => add it
    temp->nextEntry = getHashMapEntry(key, value);
}

void freeList(HashMapEntry **head) {
    HashMapEntry *temp;
    while ((*head) != NULL) {
        temp = (*head);
        (*head) = (*head)->nextEntry;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}

HashMapEntry *getHashMapEntry(char *key, char *value) {
    HashMapEntry *result = malloc(sizeof(HashMapEntry));

    result->key = (char *) malloc((strlen(key) + 1) * sizeof(char *));
    strcpy(result->key, key);

    result->value = (char *) malloc((strlen(value) + 1) * sizeof(char *));
    strcpy(result->value, value);

    result->nextEntry = NULL;

    return result;
}

void printHashMap(HashMap *hashMap) {
    for (int i = 0; i < hashMap->size; i++) {
        printList(i, hashMap->items[i].element);
    }
}

void printList(int index, HashMapEntry *head) {
    while (head != NULL) {
        printf("%d) {[%s], [%s]}\n", index, head->key, head->value);
        head = head->nextEntry;
    }
}

int hash(char *key) {
    int index = 0;

    for (int i = 0; i < strlen(key); i++) {
        index += key[i];
    }

    return index % HASH_MAP_SIZE;
}
