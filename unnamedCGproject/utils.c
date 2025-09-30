#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <GL/freeglut.h>

#include "utils.h"
#include "player.h"
#include "object.h"

void getPlayerVelocity(float *velocity, PlayerMoveKeys* moveKeys, float phiAngle, float thetaAngle, float deltaTime, bool *isOnGround) {
    // PARTE 1 - VELOCIDADE HORIZONTAL (X e Z)
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

    // Aplica deltaTime
    velocity[X_AXIS] *= deltaTime;
    velocity[Z_AXIS] *= deltaTime;

    // Limita a velocidade máxima
    float speed = sqrt(velocity[X_AXIS]*velocity[X_AXIS] + velocity[Z_AXIS]*velocity[Z_AXIS]);
    if (speed > MAX_PLAYER_HORIZONTAL_SPEED) {
        velocity[X_AXIS] = (velocity[X_AXIS] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
        velocity[Z_AXIS] = (velocity[Z_AXIS] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
    }

    //printf("%f, %f\n", velocity[0], velocity[2]);
    // PARTE 2 - VELOCIDADE VERTICAL (Y)
    if ((*isOnGround) && moveKeys->jump) {
        velocity[Y_AXIS] = PLAYER_JUMP_FORCE;
        (*isOnGround) = false;
    }

    if (!(*isOnGround)) velocity[Y_AXIS] -= GRAVITY * deltaTime; // v = v0 + at
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

void updateFOV(float newFOV, float windowWidth, float windowHeight) {
    glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(newFOV, windowWidth / windowHeight, 0.1, 100.0);
    glPopMatrix();
}

float get3DDistance(const float *obj1, const float *obj2) {
    float deltaX = obj2[0] - obj1[0];
    float deltaY = obj2[1] - obj1[1];
    float deltaZ = obj2[2] - obj1[2];

    return sqrtf(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
}

void getObjectsInCollisionRange(Player player, SceneObject *sceneObjects, int QTD_SCENEOBJECTS, SceneObject *sceneObjectsInRange, int *objCount) {
    int qtd = 0;
    float playerCenterX = (player.collision.minX + player.collision.maxX) / 2.0f;
    float playerCenterY = (player.collision.minY + player.collision.maxY) / 2.0f;
    float playerCenterZ = (player.collision.minZ + player.collision.maxZ) / 2.0f;

    for (int i = 0; i < QTD_SCENEOBJECTS; i++) {
        SceneObject obj = sceneObjects[i];

        float objCenterX = (obj.collision.minX + obj.collision.maxX) / 2.0f;
        float objCenterY = (obj.collision.minY + obj.collision.maxY) / 2.0f;
        float objCenterZ = (obj.collision.minZ + obj.collision.maxZ) / 2.0f;

        float vec1[] = {playerCenterX, playerCenterY, playerCenterZ};
        float vec2[] = {objCenterX, objCenterY, objCenterZ};

        float distance = get3DDistance(vec1, vec2);

        if (distance <= COLLISION_DETECTION_RADIUS) {
            sceneObjectsInRange[qtd++] = obj;
        }
    }

    (*objCount) = qtd;
    return sceneObjectsInRange;
}

void animateObject(SceneObject* object, float deltaTime) {
    // 1. Verifica se o objeto deve ser animado
    if (!object->anim.isAnimated) {
        return;
    }

    // 2. Anima no eixo correto (X ou Z)
    if (object->anim.animationAxis == 0) { // Eixo X
        object->x += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    } else { // Eixo Z
        object->z += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    }

    // 3. Atualiza a caixa de colisão do objeto para acompanhar o movimento
    if (object->data != NULL) {
        // Para objetos com modelo 3D
        getCollisionBoxFromObject(object);
    } else {
        // Para a plataforma invisível (atualização manual)
        if (object->anim.animationAxis == 0) { // Eixo X
             object->collision.minX += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
             object->collision.maxX += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
        } else { // Eixo Z
             object->collision.minZ += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
             object->collision.maxZ += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
        }
    }


    // 4. Verifica os limites e inverte a direção
    float currentPos = (object->anim.animationAxis == 0) ? object->x : object->z;

    if (currentPos > object->anim.maxLimit) {
        if (object->anim.animationAxis == 0) object->x = object->anim.maxLimit; else object->z = object->anim.maxLimit;
        object->anim.moveDirection = -1.0f;
    } else if (currentPos < object->anim.minLimit) {
        if (object->anim.animationAxis == 0) object->x = object->anim.minLimit; else object->z = object->anim.minLimit;
        object->anim.moveDirection = 1.0f;
    }
}
