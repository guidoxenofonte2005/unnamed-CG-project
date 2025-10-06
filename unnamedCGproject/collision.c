#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "collision.h"

bool isObjectColliding(CollisionBox box1, CollisionBox box2) {
    return (box1.minX <= box2.maxX && box1.maxX >= box2.minX) &&
           (box1.minY <= box2.maxY && box1.maxY >= box2.minY) &&
           (box1.minZ <= box2.maxZ && box1.maxZ >= box2.minZ);
}

CollisionSide getCollidingObjectSide(CollisionBox referenceObj, CollisionBox collidingObj) {
    float deltaX = fmin(referenceObj.maxX, collidingObj.maxX) - fmax(referenceObj.minX, collidingObj.minX);
    float deltaY = fmin(referenceObj.maxY, collidingObj.maxY) - fmax(referenceObj.minY, collidingObj.minY);
    float deltaZ = fmin(referenceObj.maxZ, collidingObj.maxZ) - fmax(referenceObj.minZ, collidingObj.minZ);
    float referenceCenterY = (referenceObj.minY + referenceObj.maxY) / 2.0f;
    float collidingObjCenterY = (collidingObj.minY + collidingObj.maxY) / 2.0f;

    if (deltaX < deltaY && deltaX < deltaZ) return SIDE;
    if (deltaY <= deltaX && deltaY <= deltaZ) return (referenceCenterY > collidingObjCenterY) ? TOP : BOTTOM;
    if (deltaZ < deltaY && deltaZ < deltaX) return SIDE;
    return NONE;
}

// usado pra debug de colisão
void drawCollisionBoxWireframe(CollisionBox box) {
    glPushMatrix();
        float centerX = (box.minX + box.maxX) / 2.0f;
        float centerY = (box.minY + box.maxY) / 2.0f;
        float centerZ = (box.minZ + box.maxZ) / 2.0f;
        glTranslatef(centerX, centerY, centerZ);

        // Calcula as dimens�es da caixa
        float sizeX = box.maxX - box.minX;
        float sizeY = box.maxY - box.minY;
        float sizeZ = box.maxZ - box.minZ;

        // Ajusta o wireframe
        glScalef(sizeX, sizeY, sizeZ);

        // Usa glutWireCube (cubo centrado na origem com lado 1)
        glColor3f(1.0f, 0.0f, 0.0f);
        glutWireCube(1.0f);
    glPopMatrix();
}

void getCollisionNormalVec(CollisionSide side, CollisionBox referenceObj, CollisionBox collidingObj, float *returnVec) {
    switch (side) {
    case TOP:
        returnVec[Y_AXIS] = 1.0f;
        break;
    case BOTTOM:
        returnVec[Y_AXIS] = -1.0f;
        break;
    case SIDE:
        // eixo X
        if (referenceObj.maxX <= collidingObj.minX) {
            returnVec[X_AXIS] = -1.0f;
        } else if (referenceObj.minX >= collidingObj.maxX) {
            returnVec[X_AXIS] = 1.0f;
        }
        // eixo Z
        else if (referenceObj.maxZ <= collidingObj.minZ) {
            returnVec[Z_AXIS] = -1.0f;
        } else if (referenceObj.minZ >= collidingObj.maxZ) {
            returnVec[Z_AXIS] = 1.0f;
        }
        break;
    default:
        printf("NO COLLISION DETECTED\n");
        break;
    }
}
