#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED



#endif // PLAYER_H_INCLUDED

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
    cgltf_data* entityData;
    PlayerState state;

    // OpenGL buffers
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    int indexCounter;
} Player;

void movePlayer(float *speed, Player *playerObject);
void drawPlayer(Player playerObject);
