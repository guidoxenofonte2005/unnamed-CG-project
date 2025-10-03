#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "object.h"
#include "player.h"
#include "fileManager.h"

FILE *loadFilePointer = NULL;

void parseFloatValues(const char *floatString, float *values) {
    char *tempStr = strdup(floatString);
    char *token = strtok(tempStr, ",");

    int i = 0;
    while (token != NULL && i < DIMENSIONS) {
        values[i++] = atof(token);
        token = strtok(NULL, ",");
    }

    free(tempStr);
}

void loadObjectsFromFile(char *fileLocation, SceneObject *sceneObjects, Player *player, int *qtdObjects, int max_objects) {
    if (strcmp(fileLocation, "") == 0) {
        printf("NO FILE STRING RECEIVED");
        return;
    }

    loadFilePointer = fopen(fileLocation, "r");
    if (loadFilePointer == NULL) return;

    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), loadFilePointer)) {
        line[strcspn(line, "\n")] = 0; // remove o \n do fim da linha

        char *stringParts[MAX_FILE_STRING_SEPARATIONS]; // define o m�ximo de partes que a string pode ter (nesse caso, 4 partes)
        int partIndex = 0;

        char *token = strtok(line, ";");
        while (token != NULL && partIndex < MAX_FILE_STRING_SEPARATIONS) {
            stringParts[partIndex++] = token;
            token = strtok(NULL, ";");
        }

        if (strcmp(stringParts[0], "PLATFORM") == 0) {
            float centerCoords[DIMENSIONS], platformSize[DIMENSIONS];
            parseFloatValues(stringParts[1], centerCoords);
            parseFloatValues(stringParts[2], platformSize);

            CollisionBox platformCollision = getPlatformCollisionBox(centerCoords[X_AXIS], centerCoords[Y_AXIS], centerCoords[Z_AXIS],
                                                                     platformSize[X_AXIS], platformSize[Y_AXIS], platformSize[Z_AXIS]);
            loadPlatform(sceneObjects, qtdObjects, centerCoords[X_AXIS], centerCoords[Y_AXIS], centerCoords[Z_AXIS], &platformCollision);
        }
        else if (strcmp(stringParts[0], "PLAYER") == 0) {
            float playerCoords[DIMENSIONS];
            parseFloatValues(stringParts[1], playerCoords);

            player->x = playerCoords[X_AXIS];
            player->y = playerCoords[Y_AXIS];
            player->z = playerCoords[Z_AXIS];

            loadPlayerModel(player, stringParts[2]);
        }
        else if (strcmp(stringParts[0], "OBJECT") == 0) {
            if ((*qtdObjects) > max_objects) continue;
            float objectCoords[DIMENSIONS];
            parseFloatValues(stringParts[1], objectCoords);

            loadObject(&sceneObjects[(*qtdObjects)], stringParts[2], objectCoords[X_AXIS], objectCoords[Y_AXIS], objectCoords[Z_AXIS]);

            if (atoi(stringParts[3]) == 1) {
                sceneObjects[(*qtdObjects)].type = DANGER;
            }
            (*qtdObjects)++;
        }
        else {
            printf("NON-SUPPORTED OBJECT DETECTED: %s. Skipping to the next object...\n", stringParts[0]);
        }
    }

    fclose(loadFilePointer);
}
