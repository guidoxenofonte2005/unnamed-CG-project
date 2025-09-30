#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>

#include "object.h"
#include "player.h"
#include "fileManager.h"

FILE *loadFilePointer = NULL;

void loadObjectsFromFile(char *fileLocation, SceneObject *sceneObjects, Player *player, int *qtdObjects) {
    if (strcmp(fileLocation, "") == 0) {
        printf("NO FILE STRING RECEIVED");
        return;
    }

    loadFilePointer = fopen(fileLocation, "r");
    if (loadFilePointer == NULL) return;

    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), loadFilePointer)) {
        const char *delimiters = ";";
        char *token = strtok(line, delimiters);
        if (strcmp(token, "PLAYER") == 0) {
            float posX, posY, posZ;
            while (token != NULL) {
                token = strtok(NULL, delimiters);

                // player position
                char *posToken = strtok(token, ",");
                if (posToken != NULL) {
                    int posTracker = 0;
                    while (posToken != NULL) {
                        switch (posTracker) {
                        case 0:
                            break;
                        case 1:
                            break;
                        case 2:
                            break;
                        default:
                            break;
                        }
                    }
                }

            }


            // additional info
            player->isOnGround = false;
            player->canJump = false;
            player->state = IDLE;
            player->collision =
        }
        else if (strcmp(token, "OBJECT") == 0) {

        }
        else if (strcmp(token, "PLATFORM") == 0) {

        }
        else {
            printf("Unable to extract info from object %s\n", token);
            continue;
        }
    }

    fclose(loadFilePointer);
}
