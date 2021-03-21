#include "../utils/headers.h"

int main() {
    HashMap *hashMap = NULL;
    initHashMap(&hashMap);

    freeHashMap(&hashMap);
}