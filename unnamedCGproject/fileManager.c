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

        char *stringParts[MAX_FILE_STRING_SEPARATIONS];
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

            loadPlatformTextures(&sceneObjects[(*qtdObjects)-1], atoi(stringParts[3]));
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
            if ((*qtdObjects) >= max_objects) continue;
            float objectCoords[DIMENSIONS];
            parseFloatValues(stringParts[1], objectCoords);

            // Carrega o objeto (a escala será 1.0 por padrão)
            loadObject(&sceneObjects[(*qtdObjects)], stringParts[2], objectCoords[X_AXIS], objectCoords[Y_AXIS], objectCoords[Z_AXIS]);

            // Lê a escala do arquivo, se existir
            if (partIndex > 4) {
                float scaleValue = atof(stringParts[4]);
                if (scaleValue > 0) { // Garante que a escala não seja zero ou negativa
                    sceneObjects[(*qtdObjects)].scale = scaleValue;
                }
            }

            // Recalcula a caixa de colisão DEPOIS de definir a escala
            getCollisionBoxFromObject(&sceneObjects[(*qtdObjects)]);

            // Define se o tipo
            switch (atoi(stringParts[3])) {
                case 1:
                    sceneObjects[(*qtdObjects)].type = DANGER;
                    break;
                case 3:
                    sceneObjects[(*qtdObjects)].type = FLAG;
                    break;
                case 4:
                    sceneObjects[(*qtdObjects)].type = WIN;
                    break;
                default:
                    printf("TIPO INSERIDO NÃO É SUPORTADO\n");
                    break;
            }

            // Incrementa o contador de objetos no final
            (*qtdObjects)++;
        }
        else {
            printf("NON-SUPPORTED OBJECT DETECTED: %s. Skipping to the next object...\n", stringParts[0]);
        }
    }

    fclose(loadFilePointer);
}

