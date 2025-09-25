#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED



#endif // COLLISION_H_INCLUDED

typedef struct {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
} CollisionBox;

typedef enum{
    NONE,
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    TOP,
    BOTTOM
} CollisionSide;

CollisionSide getCollidingObjectSide(CollisionBox referenceObj, CollisionBox collidingObj);
