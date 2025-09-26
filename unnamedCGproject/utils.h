#include <stdbool.h>

#define PLAYER_FRICTION 0.9f
#define PLAYER_ACCELERATION 50.0f
#define MAX_PLAYER_HORIZONTAL_SPEED 10000.0f
#define MAX_PLAYER_VERTICAL_SPEED 1.0f

#define GRAVITY 0.9f
#define PLAYER_JUMP_FORCE 1.2f

typedef struct {
    bool w;
    bool a;
    bool s;
    bool d;
    bool up;
    bool down;
} PlayerMoveKeys;

typedef enum {
    X_AXIS,
    Y_AXIS,
    Z_AXIS
} MOVEMENT_AXIS;

typedef struct {
    float offsetMinX, offsetMinY, offsetMinZ;
    float offsetMaxX, offsetMaxY, offsetMaxZ;
} ObjectCollisionOffset;

void getPlayerVelocity(float *velocity, PlayerMoveKeys* moveKeys, float phiAngle, float thetaAngle, float deltaTime);
float getDeltaTime();
void getPlayerMovingAngle(float *playerVelocity, float *playerRotation);
void updateFOV(float newFOV, float windowWidth, float windowHeight);
