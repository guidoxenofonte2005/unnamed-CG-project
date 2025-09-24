#include <GL/freeglut.h>
#include "iluminacao.h"

void setupLighting(GLfloat *ambientLight, GLfloat *diffuseLight, GLfloat *specularLight, GLfloat *lightPosition,
                   GLfloat *ambientMaterial, GLfloat *diffuseMaterial, GLfloat *specularMaterial, GLfloat shininess) {
    // Ativa iluminação e luz 0
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configura a luz
    glLightfv(GL_LIGHT0, GL_AMBIENT,  luzAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luzDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEsp);
    glLightfv(GL_LIGHT0, GL_POSITION, posLuz);

    // Configura o material
    glMaterialfv(GL_FRONT, GL_AMBIENT,  matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  matDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matEsp);
    glMaterialf(GL_FRONT, GL_SHININESS, brilho);
}

