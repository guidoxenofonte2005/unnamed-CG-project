#include <stdio.h>
#include <stdlib.h>

#include <string.h>

FILE *loadFilePointer = NULL;

void loadFile(char *fileLocation) {
    if (strcmp(fileLocation, "")) return;

    loadFilePointer = fopen(fileLocation, "r");
    if (loadFilePointer == NULL) return;


}
