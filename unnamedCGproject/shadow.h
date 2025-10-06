#ifndef SHADOW_H
#define SHADOW_H

#include <GL/freeglut.h>

// Calcula a matriz de sombra a partir de uma luz e um plano
void makeShadowMatrix(GLfloat plane[4], GLfloat lightPos[4], GLfloat shadowMat[16]);

// Aplica a sombra sobre um objeto desenhado
void drawShadow(Player *player, SceneObject *objectsInCollisionRange, float playerRotation);

#endif
