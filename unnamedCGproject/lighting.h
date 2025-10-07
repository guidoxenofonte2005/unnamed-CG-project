#include <GL/glut.h>

//Função para configurar iluminação e material

// ambientLight     -> Componente ambiente da luz (RGBA)
// diffuseLight     -> Componente difusa da luz (RGBA)
// specularLight    -> Componente especular da luz (RGBA)
// lightPosition    -> Posição/direção da luz (x, y, z, w)
//
// ambientMaterial  -> Como o material reflete luz ambiente (RGBA)
// diffuseMaterial  -> Como o material reflete luz difusa (RGBA)
// specularMaterial -> Como o material reflete luz especular (RGBA)
// shininess        -> Concentração do brilho especular (float)
//                     Valores baixos -> brilho espalhado (fosco)
//                     Valores altos  -> brilho concentrado (polido)
void setupLighting(GLfloat *ambientLight, GLfloat *diffuseLight, GLfloat *specularLight, GLfloat *lightPosition);

void setMaterial(GLfloat *ambientMaterial, GLfloat *diffuseMaterial, GLfloat *specularMaterial, GLfloat shininess);
