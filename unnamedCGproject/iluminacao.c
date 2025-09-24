#include <GL/freeglut.h>
#include "iluminacao.h"

void setupLighting(GLfloat *ambientLight, GLfloat *diffuseLight, GLfloat *specularLight, GLfloat *lightPosition,
                   GLfloat *ambientMaterial, GLfloat *diffuseMaterial, GLfloat *specularMaterial, GLfloat shininess) {
    // Ativa iluminação e luz 0
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configura a luz
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Configura o material
    glMaterialfv(GL_FRONT, GL_AMBIENT,  ambientMaterial);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

