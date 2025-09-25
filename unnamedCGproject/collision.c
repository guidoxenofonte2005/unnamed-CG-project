#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#include "utils.h"
#include "collision.h"

CollisionSide getCollidingObjectSide(CollisionBox referenceObj, CollisionBox collidingObj) {
    if (referenceObj.maxX > collidingObj.minX && referenceObj.minX < collidingObj.maxX) return RIGHT;
    if (referenceObj.minX < collidingObj.maxX && referenceObj.maxX > collidingObj.maxX) return LEFT;
    if (referenceObj.maxY > collidingObj.minY && referenceObj.minY < collidingObj.minY) return TOP;
    if (referenceObj.minY < collidingObj.maxY && referenceObj.maxY > collidingObj.maxY) return BOTTOM;
    if (referenceObj.maxZ > collidingObj.minZ && referenceObj.minZ < collidingObj.minZ) return FRONT;
    if (referenceObj.minZ < collidingObj.maxZ && referenceObj.maxZ > collidingObj.maxZ) return BACK;
    return NONE;
}
