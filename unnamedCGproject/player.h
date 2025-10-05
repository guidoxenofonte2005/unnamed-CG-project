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
    bool isRespawning;
    bool isJumping;
    PlayerState state;
    CollisionBox collision;
    cgltf_data* modelData; // Adiciona o ponteiro para os dados do modelo 3D.
    CollisionBox initialCollision;
    int groundObjectIndex; // guarda o �ndice do objeto que o player t� em cima
} Player;

void movePlayer(float *speed, Player *playerObject);

// Declara��es das fun��es de importa��o e desenho
void loadPlayerModel(Player* playerObj, const char* filename);
void drawPlayerModel(Player* playerObj, float thetaAngle);
void cleanupPlayerModel(Player* playerObj);

void getPlayerCollisionBox(Player *player);
void applyGravity(Player* playerObj, float *speed);
void updatePlayerCollisionBox(Player* player);

void collideAndSlide(float *speed, Player *player, SceneObject *objectsInRange, int qtdObjInRange, float deltaTime);

extern int qtdTextures;

#endif // PLAYER_H_INCLUDED
