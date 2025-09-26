#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <stdbool.h>

typedef struct {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
} CollisionBox;


typedef enum{
    NONE,
    SIDE,
    TOP,
    BOTTOM
} CollisionSide;

#endif // COLLISION_H_INCLUDED

bool isObjectColliding(CollisionBox box1, CollisionBox box2);
CollisionSide getCollidingObjectSide(CollisionBox referenceObj, CollisionBox collidingObj);
void drawCollisionBoxWireframe(CollisionBox box);
