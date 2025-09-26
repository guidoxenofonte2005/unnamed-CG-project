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
    if (referenceObj.maxX > collidingObj.minX && referenceObj.minX < collidingObj.maxX) return RIGHT;
    if (referenceObj.minX < collidingObj.maxX && referenceObj.maxX > collidingObj.maxX) return LEFT;
    if (referenceObj.maxY > collidingObj.minY && referenceObj.minY < collidingObj.minY) return TOP;
    if (referenceObj.minY < collidingObj.maxY && referenceObj.maxY > collidingObj.maxY) return BOTTOM;
    if (referenceObj.maxZ > collidingObj.minZ && referenceObj.minZ < collidingObj.minZ) return FRONT;
    if (referenceObj.minZ < collidingObj.maxZ && referenceObj.maxZ > collidingObj.maxZ) return BACK;
    return NONE;
}
