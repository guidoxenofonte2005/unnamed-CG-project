#include "iluminacao.h"

void setupLighting(GLfloat *luzAmb, GLfloat *luzDif, GLfloat *luzEsp, GLfloat *posLuz,
                   GLfloat *matAmb, GLfloat *matDif, GLfloat *matEsp, GLfloat brilho) {
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

