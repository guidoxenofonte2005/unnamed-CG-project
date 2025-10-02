#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

#include "libs/cgltf/cgltf.h"
#include "collision.h"
#include "object.h"

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
    bool canJump;
    PlayerState state;
    CollisionBox collision;
    cgltf_data* modelData; // Adiciona o ponteiro para os dados do modelo 3D.
} Player;

void movePlayer(float *speed, Player *playerObject);

// Declarações das funções de importação e desenho
void loadPlayerModel(Player* playerObj, const char* filename);
void drawPlayerModel(Player* playerObj, float thetaAngle);
void cleanupPlayerModel(Player* playerObj);

//void getPlayerCollisionBox(Player *player);
void applyGravity(Player* playerObj, float *speed);

void collideAndSlide(float *velocity, Player *player, SceneObject *objectsInRange, int qtdObjInRange, float deltaTime);

extern int qtdTextures;

#endif // PLAYER_H_INCLUDED
