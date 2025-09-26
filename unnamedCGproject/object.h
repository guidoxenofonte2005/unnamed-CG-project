#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "libs/cgltf/cgltf.h"

#include "collision.h"

// Estrutura para armazenar os dados de um objeto 3D
typedef struct {
    cgltf_data* data;
    float x;
    float y;
    float z;
    CollisionBox collision;
} SceneObject;

// Declarações das funções para manipular a cena
void loadObject(SceneObject* object, const char* filename, float x, float y, float z);
void drawObject(SceneObject* object);
void cleanupObject(SceneObject* object);

void getCollisionBoxFromObject(SceneObject *object);

#endif // OBJECT_H_INCLUDED
