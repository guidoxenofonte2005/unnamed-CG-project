#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "player.h"

void movePlayer(float *speed, Player *playerObject) {
    /**
    Função responsável pelo movimento do player

    **/

    playerObject->x += speed[0];
    playerObject->y += speed[1];
    playerObject->z += speed[2];
}

void drawPlayer(Player playerObject) {

}
