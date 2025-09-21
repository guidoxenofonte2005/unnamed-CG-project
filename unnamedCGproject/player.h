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
    int x;
    int y;
    bool moveable = false;
    bool canJump = true;
    PlayerState state = IDLE;
} Player;

void coisa();
