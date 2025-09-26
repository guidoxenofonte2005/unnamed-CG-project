#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <GL/freeglut.h>

#include "utils.h"

void getPlayerVelocity(float *velocity, PlayerMoveKeys* moveKeys, float phiAngle, float thetaAngle, float deltaTime) {
    float dirX = sin(thetaAngle); // depende somente do eixo horizontal
    float dirZ = -cos(thetaAngle); // depende somente do eixo horizontal

    float rightX = cos(thetaAngle);
    float rightZ = sin(thetaAngle);

    if (moveKeys->d || moveKeys->a || moveKeys->w || moveKeys->s) {
        if (moveKeys->d) {
            velocity[X_AXIS] += dirX * PLAYER_ACCELERATION;
            velocity[Z_AXIS] += dirZ * PLAYER_ACCELERATION;
        }
        if (moveKeys->a) {
            velocity[X_AXIS] -= dirX * PLAYER_ACCELERATION;
            velocity[Z_AXIS] -= dirZ * PLAYER_ACCELERATION;
        }
        if (moveKeys->w) {
            velocity[X_AXIS] -= rightX * PLAYER_ACCELERATION;
            velocity[Z_AXIS] -= rightZ * PLAYER_ACCELERATION;
        }
        if (moveKeys->s) {
            velocity[X_AXIS] += rightX * PLAYER_ACCELERATION;
            velocity[Z_AXIS] += rightZ * PLAYER_ACCELERATION;
        }
    } else {
        velocity[X_AXIS] *= PLAYER_FRICTION;
        if (fabs(velocity[X_AXIS]) < 0.01f) velocity[X_AXIS] = 0;

        velocity[Z_AXIS] *= PLAYER_FRICTION;
        if (fabs(velocity[Z_AXIS]) < 0.01f) velocity[Z_AXIS] = 0;
    }

    // Aplica fricção
    // velocity[0] *= PLAYER_FRICTION * deltaTime;
    // velocity[2] *= PLAYER_FRICTION * deltaTime;
    velocity[X_AXIS] *= deltaTime;
    velocity[Z_AXIS] *= deltaTime;

    // Limita a velocidade máxima
    float speed = sqrt(velocity[X_AXIS]*velocity[X_AXIS] + velocity[Z_AXIS]*velocity[Z_AXIS]);
    if (speed > MAX_PLAYER_HORIZONTAL_SPEED) {
        velocity[X_AXIS] = (velocity[X_AXIS] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
        velocity[Z_AXIS] = (velocity[Z_AXIS] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
    }

    //printf("%f, %f\n", velocity[0], velocity[2]);
}

float getDeltaTime() {
    static clock_t lastTime = 0; // inicializa uma única vez
    clock_t currentTime = clock(); // pega o tempo atual
    float delta = (float)(currentTime - lastTime) / CLOCKS_PER_SEC; // calcula o delta, já dividindo por 1000 para resultado em segundos
    lastTime = currentTime; // transforma o atual em último

    return delta;
}

void getPlayerMovingAngle(float *playerVelocity, float *playerRotation) {
    float speedX = playerVelocity[X_AXIS];
    float speedZ = playerVelocity[Z_AXIS];

    if (fabs(speedX) > 0.001f || fabs(speedZ) > 0.001f) {
        *playerRotation = atan2f(speedX, speedZ) * (180.0f / M_PI); // retorna a rotação do player
    }
}

void handlePlayerJump(float *velocity, PlayerMoveKeys* moveKeys, float deltaTime) { // depois pode trocar o movekeys por um booleano pra simplificar
    if (moveKeys->up) {
        velocity[Y_AXIS] += PLAYER_JUMP_FORCE;
    }
    else if (moveKeys->down) {
        velocity[Y_AXIS] -= PLAYER_JUMP_FORCE;
    } else {
        velocity[Y_AXIS] *= 0.9f;
        if (fabs(velocity[Y_AXIS]) < 0.01f) velocity[Y_AXIS] = 0;
    }

    velocity[Y_AXIS] *= deltaTime;

    if (velocity[Y_AXIS] > 0) {
        if (velocity[Y_AXIS] > MAX_PLAYER_VERTICAL_SPEED) {
            velocity[Y_AXIS] = MAX_PLAYER_VERTICAL_SPEED;
        }
    }
    if (velocity[Y_AXIS] < 0) {
        if (velocity[Y_AXIS] < -MAX_PLAYER_VERTICAL_SPEED) {
            velocity[Y_AXIS] = -MAX_PLAYER_VERTICAL_SPEED;
        }
    }
}

void updateFOV(float newFOV, float windowWidth, float windowHeight) {
    glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(newFOV, windowWidth / windowHeight, 0.1, 100.0);
    glPopMatrix();
}
