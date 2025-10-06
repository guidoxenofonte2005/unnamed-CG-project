#include <stdbool.h>

#define PLAYER_FRICTION 0.9f
#define PLAYER_ACCELERATION 15.0f
#define MAX_PLAYER_HORIZONTAL_SPEED 100.0f
#define MAX_PLAYER_VERTICAL_SPEED 1.6f

#define GRAVITY 200.0f
#define PLAYER_JUMP_FORCE 1.5f

#define COLLISION_DETECTION_RADIUS 30.0f

#define MIN_SCREEN_WIDTH 1000
#define MAX_SCREEN_WIDTH 1920
#define MIN_SCREEN_HEIGHT 750
#define MAX_SCREEN_HEIGHT 1080

#define PHYSICS_STEP (1.0f / 75.0f) // EXTREMAMENTE IMPORTANTE PRA F�SICA DO JOGO, N�O TIRA POR NADA NO MUNDO

#include "object.h"
#include "player.h"

typedef struct {
    bool w;
    bool a;
    bool s;
    bool d;
    bool jump;
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

void getPlayerVelocity(float *velocity, PlayerMoveKeys* moveKeys, float phiAngle, float thetaAngle, float deltaTime, bool *isOnGround);
float getDeltaTime();
void getPlayerMovingAngle(float *playerVelocity, float *playerRotation);
void updateFOV(float newFOV, float windowWidth, float windowHeight);
void getObjectsInCollisionRange(Player player, SceneObject *sceneObjects, int QTD_SCENEOBJECTS, SceneObject *sceneObjectsInRange, int *objCount);

// Anima um objeto com base nos seus dados de anima��o
void animateObject(SceneObject* object, float deltaTime);
void respawnPlayer();

float getShadowPlatformHeight(SceneObject *sceneObjects, int objectCount, float objX, float objY, float objZ);
