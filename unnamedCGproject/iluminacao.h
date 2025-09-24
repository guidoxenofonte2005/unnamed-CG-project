#ifndef ILUMINACAO_H
#define ILUMINACAO_H
#include <GL/glut.h>

// Função para configurar iluminação e material
void setupLighting(GLfloat *luzAmb, GLfloat *luzDif, GLfloat *luzEsp, GLfloat *posLuz,
                   GLfloat *matAmb, GLfloat *matDif, GLfloat *matEsp, GLfloat brilho);

#endif
