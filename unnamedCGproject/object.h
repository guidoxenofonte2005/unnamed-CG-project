#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "libs/cgltf/cgltf.h"
#include "GL/freeglut.h"
#include "collision.h"
#include "texture.h"

// Enum para os tipos de objetos
typedef enum {
    DEFAULT, // Um objeto genérico
    PLATFORM,
    DANGER, // Um objeto perigoso
    FLAG // Um objeto para mercar chek
} ObjectType;

typedef struct {
    bool isAnimated; // deve ser animado ou não
    int animationAxis; // 0 para eixo X, 1 para eixo Z
    float moveSpeed;
    float moveDirection;
    float minLimit;
    float maxLimit;
} AnimationData;

// Estrutura para armazenar os dados de um objeto 3D
typedef struct {
    cgltf_data* data;
    float x;
    float y;
    float z;
    float prevX, prevY, prevZ;
    float scale;
    float rotationAngle; // O ângulo da rotação em graus
    float rotX, rotY, rotZ; // O vetor do eixo de rotação (ex: 1,0,0 para o eixo X)
    CollisionBox collision;
    AnimationData anim;
    ObjectType type;
    GLuint textureID;
    PlatformTextureTypes platformTextures;
} SceneObject;

// Declarações das funções para manipular a cena
void loadObject(SceneObject* object, const char* filename, float x, float y, float z);
void drawObject(SceneObject* object);
void cleanupObject(SceneObject* object);
void getCollisionBoxFromObject(SceneObject *object);

void loadPlatform(SceneObject *sceneObjects, int *qtdSceneObjects, float x, float y, float z, CollisionBox *platformCollision);
CollisionBox getPlatformCollisionBox(float centerX, float centerY, float centerZ, float width, float height, float depth);
void drawPlatform(SceneObject *platform);

void loadPlatformTextures(SceneObject *platform, int textureType);

#endif // OBJECT_H_INCLUDED
