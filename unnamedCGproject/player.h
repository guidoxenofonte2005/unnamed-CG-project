#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "libs/cgltf/cgltf.h"
#include "collision.h"
#include "object.h"

extern bool gameCompleted;

typedef enum {
    IDLE,
    MOVING,
    JUMPING
} PlayerState;

typedef struct {
    float x;
    float y;
    float z;
    bool isOnGround;
    PlayerState state;
    CollisionBox collision;
    cgltf_data* modelData; // ponteiro para os dados do modelo 3D.
    int groundObjectIndex; // guarda o indice do objeto que o player ta em cima
} Player;

void movePlayer(float *speed, Player *playerObject);

void loadPlayerModel(Player* playerObj, const char* filename);
void drawPlayerModel(Player* playerObj, float thetaAngle);
void cleanupPlayerModel(Player* playerObj);

void getPlayerCollisionBox(Player *player);
void applyGravity(Player* playerObj, float *speed);
void updatePlayerCollisionBox(Player* player);

void collideAndSlide(float *speed, Player *player, SceneObject *objectsInRange, int qtdObjInRange, float deltaTime);

extern float checkpointX, checkpointY, checkpointZ;
extern SceneObject sceneObjects[];

#endif // PLAYER_H_INCLUDED
