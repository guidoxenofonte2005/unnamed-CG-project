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
    float dirX = sin(thetaAngle);
    float dirZ = -cos(thetaAngle);
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

    // Limita a velocidade máxima horizontal
    float speed = sqrt(velocity[X_AXIS]*velocity[X_AXIS] + velocity[Z_AXIS]*velocity[Z_AXIS]);
    if (speed > MAX_PLAYER_HORIZONTAL_SPEED) {
        velocity[X_AXIS] = (velocity[X_AXIS] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
        velocity[Z_AXIS] = (velocity[Z_AXIS] / speed) * MAX_PLAYER_HORIZONTAL_SPEED;
    }

    // PARTE 2 - VELOCIDADE VERTICAL (Y) - COM VERIFICAÇÃO DE RESPAWN
    if (!(*isOnGround)) {
        // Aplica gravidade apenas se não estiver no chão
        velocity[Y_AXIS] -= GRAVITY * deltaTime * deltaTime;

        // Limita velocidade de queda máxima
        if (velocity[Y_AXIS] < -MAX_PLAYER_VERTICAL_SPEED) {
            velocity[Y_AXIS] = -MAX_PLAYER_VERTICAL_SPEED;
        }
    } else {
        // Se está no chão, zera a velocidade vertical (evita acúmulo)
        if (velocity[Y_AXIS] < 0) {
            velocity[Y_AXIS] = 0;
        }
    }

    // Pulo
    if ((*isOnGround) && moveKeys->jump) {
        velocity[Y_AXIS] = PLAYER_JUMP_FORCE;
        (*isOnGround) = false;
    }
}

float getDeltaTime() {
    static LARGE_INTEGER frequency;
    static LARGE_INTEGER lastTime;
    LARGE_INTEGER currentTime;
    if (frequency.QuadPart == 0) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&lastTime);
        return 0.0f;
    }
    QueryPerformanceCounter(&currentTime);
    double delta = (double)(currentTime.QuadPart - lastTime.QuadPart) / (double)frequency.QuadPart;
    lastTime = currentTime;
    // evita spikes grandes (p.ex. quando o SO suspendeu o processo)
    if (delta < 0.0) delta = 0.0;
    if (delta > 0.25) delta = 0.25;
    return (float)delta;
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
        gluPerspective(newFOV, windowWidth / windowHeight, 0.1, 1000.0);
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

    // Gaurda posição anterior
    object->prevX = object->x;
    object->prevY = object->y;
    object->prevZ = object->z;
    // Atualiza posição
    if (object->anim.animationAxis == 0) {
        object->x += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    } else if (object->anim.animationAxis == 1) {
        object->z += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    } else if (object->anim.animationAxis == 2) {
        object->y += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    }


    // 2. Anima no eixo correto
    if (object->anim.animationAxis == 0) { // Eixo X
        object->x += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    } else if (object->anim.animationAxis == 1) { // Eixo Z
        object->z += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    } else if (object->anim.animationAxis == 2) { // Eixo Y
        object->y += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
    }

    // 3. Atualiza a caixa de colisão do objeto para acompanhar o movimento
    if (object->data != NULL) {
        getCollisionBoxFromObject(object);
    } else {
        // Para plataformas invisíveis (não deve ser o caso dos espinhos)
        if (object->anim.animationAxis == 0) {
             object->collision.minX += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
             object->collision.maxX += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
        } else if (object->anim.animationAxis == 1) {
             object->collision.minZ += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
             object->collision.maxZ += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
        } else if (object->anim.animationAxis == 2) {
             object->collision.minY += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
             object->collision.maxY += object->anim.moveDirection * object->anim.moveSpeed * deltaTime;
        }
    }

    // 4. Verifica os limites e inverte a direção (VERSÃO CORRIGIDA)
    float currentPos;
    if (object->anim.animationAxis == 0) {
        currentPos = object->x;
    } else if (object->anim.animationAxis == 1) {
        currentPos = object->z;
    } else { // Assumindo animationAxis == 2
        currentPos = object->y;
    }

    if (currentPos > object->anim.maxLimit) {
        // Trava a posição no limite para evitar que ultrapasse
        if (object->anim.animationAxis == 0) object->x = object->anim.maxLimit;
        else if (object->anim.animationAxis == 1) object->z = object->anim.maxLimit;
        else if (object->anim.animationAxis == 2) object->y = object->anim.maxLimit; // <<< CORREÇÃO AQUI
        // Inverte a direção
        object->anim.moveDirection = -1.0f;
    } else if (currentPos < object->anim.minLimit) {
        // Trava a posição no limite
        if (object->anim.animationAxis == 0) object->x = object->anim.minLimit;
        else if (object->anim.animationAxis == 1) object->z = object->anim.minLimit;
        else if (object->anim.animationAxis == 2) object->y = object->anim.minLimit; // <<< CORREÇÃO AQUI
        // Inverte a direção
        object->anim.moveDirection = 1.0f;
    }
}

// pega a altura da plataforma mais perto do jogador verticalmente que esteja abaixo dele
float getShadowPlatformHeight(SceneObject *sceneObjects, int objectCount, float objX, float objY, float objZ) {
    float bestY = -15.0f; // limitado ao Y_DEATH_LEVEL
    for (int i = 0; i < objectCount; i++) {
        if (sceneObjects[i].type != PLATFORM) continue;
        CollisionBox c = sceneObjects[i].collision;

        // Verifica se o objeto está horizontalmente acima da plataforma
        if (objX >= c.minX && objX <= c.maxX &&
            objZ >= c.minZ && objZ <= c.maxZ) {

            // Plataforma está abaixo do objeto
            if (c.maxY <= objY && c.maxY > bestY) {
                bestY = c.maxY;
            }
        }
    }
    return bestY; // retorna o topo da plataforma mais próxima abaixo

}
