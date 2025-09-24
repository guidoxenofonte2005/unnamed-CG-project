#include <stdbool.h>

#define PLAYER_FRICTION 0.9f
#define PLAYER_ACCELERATION 2.5f
#define MAX_PLAYER_HORIZONTAL_SPEED 100.0f

#define GRAVITY 0.9f
#define PLAYER_JUMP_FORCE 1.2f

typedef struct {
    bool w;
    bool a;
    bool s;
    bool d;
} PlayerMoveKeys;

void getPlayerVelocity(float *velocity, PlayerMoveKeys* moveKeys, float phiAngle, float thetaAngle, float deltaTime);
float getDeltaTime();
