#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED



#endif // PLAYER_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

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
void drawPlayer(Player playerObject);
