#include <stdbool.h>

#define PLAYER_FRICTION 0.9f
#define PLAYER_ACCELERATION 100.0f
#define MAX_PLAYER_HORIZONTAL_SPEED 10000.0f

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
void getPlayerMovingAngle(float *playerVelocity, float *playerRotation);
