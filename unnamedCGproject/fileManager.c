#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "fileManager.h"

FILE *loadFilePointer = NULL;

void loadObjectsFromFile(char *fileLocation) {
    if (strcmp(fileLocation, "")) return;

    loadFilePointer = fopen(fileLocation, "r");
    if (loadFilePointer == NULL) return;

    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), loadFilePointer)) {
        const char *delimiters = ";";
        char *token = strtok(line, delimiters);
        printf("%s, %s", token[0], token[1]);
    }

    fclose(loadFilePointer);
}
