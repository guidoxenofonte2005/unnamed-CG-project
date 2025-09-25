#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "utils.h"

void getPlayerVelocity(float *velocity, PlayerMoveKeys* moveKeys, float phiAngle, float thetaAngle, float deltaTime) {
    float dirX = sin(thetaAngle); // depende somente do eixo horizontal
    float dirZ = -cos(thetaAngle); // depende somente do eixo horizontal

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
    velocity[0] *= PLAYER_FRICTION * deltaTime;
    velocity[2] *= PLAYER_FRICTION * deltaTime;

    // Limita a velocidade máxima
    float speed = sqrt(velocity[0]*velocity[0] + velocity[2]*velocity[2]);
    if (speed > MAX_PLAYER_HORIZONTAL_SPEED) {
        velocity[0] = (velocity[0] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
        velocity[2] = (velocity[2] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
    }
}

float getDeltaTime() {
    static clock_t lastTime = 0; // inicializa uma única vez
    clock_t currentTime = clock(); // pega o tempo atual
    float delta = (float)(currentTime - lastTime) / CLOCKS_PER_SEC; // calcula o delta, já dividindo por 1000 para resultado em segundos
    lastTime = currentTime; // transforma o atual em último

    return delta;
}

void getPlayerMovingAngle(float *playerVelocity, float *playerRotation) {
    float speedX = playerVelocity[0];
    float speedZ = playerVelocity[2];

    if (fabs(speedX) > 0.001f || fabs(speedZ) > 0.001f) {
        *playerRotation = atan2f(speedX, speedZ) * (180.0f / M_PI); // retorna a rotação do player
    }
}
