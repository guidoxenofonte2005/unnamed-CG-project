#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "libs/cgltf/cgltf.h"
#include "GL/freeglut.h"

#include "collision.h"


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
    CollisionBox collision;
    AnimationData anim;
    GLuint textureID;
} SceneObject;

// Declarações das funções para manipular a cena
void loadObject(SceneObject* object, const char* filename, float x, float y, float z);
void drawObject(SceneObject* object);
void cleanupObject(SceneObject* object);

void getCollisionBoxFromObject(SceneObject *object);
void loadPlatform(SceneObject *sceneObjects, int *qtdSceneObjects, float x, float y, float z, CollisionBox *platformCollision);

#endif // OBJECT_H_INCLUDED
