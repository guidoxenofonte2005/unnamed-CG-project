#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

void getPlayerVelocity(float *velocity, PlayerMoveKeys* moveKeys, float phiAngle, float thetaAngle) {
    float dirX = cos(phiAngle) * sin(thetaAngle);
    float dirZ = -cos(phiAngle) * cos(thetaAngle);

    float rightX = cos(thetaAngle);
    float rightZ = sin(thetaAngle);

    if (moveKeys->d) {
        velocity[0] += dirX * PLAYER_ACCELERATION;
        velocity[2] += dirZ * PLAYER_ACCELERATION;
    }
    if (moveKeys->a) {
        velocity[0] -= dirX * PLAYER_ACCELERATION;
        velocity[2] -= dirZ * PLAYER_ACCELERATION;
    }
    if (moveKeys->w) {
        velocity[0] -= rightX * PLAYER_ACCELERATION;
        velocity[2] -= rightZ * PLAYER_ACCELERATION;
    }
    if (moveKeys->s) {
        velocity[0] += rightX * PLAYER_ACCELERATION;
        velocity[2] += rightZ * PLAYER_ACCELERATION;
    }

    // Aplica fricção
    velocity[0] *= PLAYER_FRICTION;
    velocity[2] *= PLAYER_FRICTION;

    // Limita a velocidade máxima
    float speed = sqrt(velocity[0]*velocity[0] + velocity[2]*velocity[2]);
    if (speed > MAX_PLAYER_HORIZONTAL_SPEED) {
        velocity[0] = (velocity[0] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
        velocity[2] = (velocity[2] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
    }
}

//void loadScenarioObjectEntityData(ScenarioObject *sceneObj) {
//
//}
//
//void loadCollectibleEntityData(Collectible *collectibleObj) {
//
//}
