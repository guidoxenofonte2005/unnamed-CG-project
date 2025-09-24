#ifndef ILUMINACAO_H
#define ILUMINACAO_H
#include <GL/glut.h>

// Função para configurar iluminação e material
void setupLighting(GLfloat *ambientLight, GLfloat *diffuseLight, GLfloat *specularLight, GLfloat *lightPosition,
                   GLfloat *ambientMaterial, GLfloat *diffuseMaterial, GLfloat *specularMaterial, GLfloat shininess);

#endif
