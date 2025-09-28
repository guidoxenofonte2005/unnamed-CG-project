#include <GL/freeglut.h>
#include "lighting.h"

void setupLighting(GLfloat *ambientLight, GLfloat *diffuseLight, GLfloat *specularLight, GLfloat *lightPosition,
                   GLfloat *ambientMaterial, GLfloat *diffuseMaterial, GLfloat *specularMaterial, GLfloat shininess) {
    // Ativa iluminação e luz 0
    glEnable(GL_LIGHTING); // Habilita o sistema de iluminação
    glEnable(GL_LIGHT0); // Ativa a luz 0

    // Configura a luz
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambientLight); //Define a luz ambiente
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseLight); //Define a luz difusa
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);//Define a luz especular
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); //Define a posição da luz

    // Configura como o material vai refletir a luz
    glMaterialfv(GL_FRONT, GL_AMBIENT,  ambientMaterial); //reação a luz ambiente
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  diffuseMaterial); // ... a luz difusa
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial); // .. a luz especular
    glMaterialf(GL_FRONT, GL_SHININESS, shininess); // intensidade da luz
}

