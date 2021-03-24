#include "../utils/headers.h"

void freeMemoryAndExit(HashMap **hashMap) {
    freeHashMap(hashMap);
    exit(EXIT_FAILURE);
}

void parseSymbolMapping(char *symbolMapping, HashMap *hashMap) {
    char *symbol = calloc(SMALL_BUFFERS_SIZE, sizeof(char));
    char *mapping = calloc(SMALL_BUFFERS_SIZE, sizeof(char));

    char *pointerToEqualChar = strstr(symbolMapping, "=");

    if (pointerToEqualChar == NULL) {
        strcpy(symbol, symbolMapping);
    } else {
        strncpy(symbol, symbolMapping, pointerToEqualChar - symbolMapping);
        strncpy(mapping, pointerToEqualChar, symbolMapping + strlen(symbolMapping) - ++pointerToEqualChar);
    }

    addToHashMap(hashMap, symbol, mapping);

    free(symbol);
    free(mapping);
}

void parseInput(int argc, char **argv, HashMap *hashMap, FILE **input, FILE **output) {
    int currentIsSymbol = 0;
    int currentIsDirectory = 0;
    int currentIsOutputFile = 0;

    for (int i = 1; i < argc; i++) {
        if (currentIsOutputFile) {
            *output = fopen(argv[i], "w");
            currentIsOutputFile = 0;
            continue;
        }

        if (currentIsDirectory) {
            // TODO
            currentIsDirectory = 0;
            continue;
        }

        if (currentIsSymbol) {
            parseSymbolMapping(argv[i], hashMap);
            currentIsSymbol = 0;
            continue;
        }

        if (strstr(argv[i], "-D") != NULL) {
            if (argv[i][2] == '\0') {
                currentIsSymbol = 1;
                continue;
            }
            parseSymbolMapping(&argv[i][2], hashMap);
            continue;
        }

        if (strstr(argv[i], "-I") != NULL) {
            if (argv[i][2] == '\0') {
                currentIsDirectory = 1;
                continue;
            }
            //TODO
            continue;
        }

        if (strstr(argv[i], "-o") != NULL) {
            if (argv[i][2] == '\0') {
                currentIsOutputFile = 1;
                continue;
            }
            *output = fopen(&argv[i][2], "w");
            continue;
        }

        if (*input == NULL) {
            *input = fopen(argv[i], "r");
            if (*input == NULL) {
                freeMemoryAndExit(&hashMap);
            }
        } else {
            freeMemoryAndExit(&hashMap);
        }
    }
}

void replaceLine(char **line, char *toBeReplaced, char *valueForReplace) {
    int newSize = strlen(*line) - strlen(toBeReplaced) + strlen(valueForReplace) + 1;
    char *pointerToStart = strstr(*line, toBeReplaced);

    char *tempBuffer = calloc(newSize, sizeof(char));

    strncpy(tempBuffer, *line, pointerToStart - *line);
    strcat(tempBuffer, valueForReplace);
    strcat(tempBuffer, pointerToStart + strlen(toBeReplaced));

    free(*line);
    *line = malloc(newSize);
    strcpy(*line, tempBuffer);
    free(tempBuffer);
}

void parseAndAddToHashMap(char *buffer, HashMap *hashMap) {
    char *delimiters = "\n ";
    char key[SMALL_BUFFERS_SIZE];
    char value[SMALL_BUFFERS_SIZE];

    char *token;
    token = strtok(buffer, delimiters);
    int tokenNumber = 0;
    while (token != NULL) {
        if (strcmp(token, "#define") != 0) {
            if (tokenNumber++ == 0) {
                strcpy(key, token);
            } else {
                strcpy(value, token);
            }
        }

        token = strtok(NULL, delimiters);
    }

    addToHashMap(hashMap, key, value);
}

void undefineKey(HashMap *hashMap, char *buffer) {
    char *token;
    token = strtok(buffer, " \n");
    token = strtok(NULL, " \n");
    removeFromHashMap(hashMap, token);
}

int main(int argc, char **argv) {
    HashMap *hashMap = NULL;
    char buffer[BUFFER_SIZE];
    char *delimiters = "\t []{}<>=+-*/%!&|^.,:;()\\";
    FILE *input = NULL;
    FILE *output = NULL;

    initHashMap(&hashMap);

    parseInput(argc, argv, hashMap, &input, &output);
    input = input == NULL ? stdin : input;
    output = output == NULL ? stdout : output;

    int doneWithDefines = 0;
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        if (strstr(buffer, "#define") != NULL) {
            parseAndAddToHashMap(buffer, hashMap);
            continue;
        }

        if (doneWithDefines == 0 && strcmp(buffer, "\n") == 0) {
            continue;
        }

        if (strstr(buffer, "#undef") != NULL) {
            undefineKey(hashMap, buffer);
            continue;
        }

        doneWithDefines = 1;
        char *lineToWrite = calloc(strlen(buffer) + 1, sizeof(char));
        strcpy(lineToWrite, buffer);

        char *token;
        token = strtok(buffer, delimiters);
        while (token != NULL) {
            char *possibleValue = getValueFromHashMap(hashMap, token);

            if (possibleValue != NULL) {
                replaceLine(&lineToWrite, token, possibleValue);
            }
            token = strtok(NULL, delimiters);
        }

        fprintf(output, "%s", lineToWrite);
        free(lineToWrite);
    }

    freeHashMap(&hashMap);
    fclose(input);
    fclose(output);
}