#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

#include "libs/cgltf/cgltf.h"

typedef enum {
    IDLE,
    MOVING,
    JUMPING
} PlayerState;

typedef struct {
    float x;
    float y;
    float z;
    bool moveable;
    bool canJump;
    PlayerState state;
} Player;

void movePlayer(float *speed, Player *playerObject);

// Estrutura para armazenar os dados do modelo 3D
typedef struct {
    cgltf_data* data;
} PlayerModel;

// Declara a variável global para o modelo do jogador player_model;
extern PlayerModel player_model;

// Declarações das funções de importação e desenho
void loadPlayerModel(const char* filename);
void drawPlayerModel(Player playerObj, float thetaAngle);
void cleanupPlayerModel();

#endif // PLAYER_H_INCLUDED
