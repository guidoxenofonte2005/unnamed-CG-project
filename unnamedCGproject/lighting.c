#include <GL/freeglut.h>
#include "lighting.h"

void setupLighting(GLfloat *ambientLight, GLfloat *diffuseLight, GLfloat *specularLight, GLfloat *lightPosition) {
    // Configura a luz
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambientLight); //Define a luz ambiente
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseLight); //Define a luz difusa
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);//Define a luz especular
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); //Define a posição da luz
}

/*void setMaterial(GLfloat *ambientMaterial, GLfloat *diffuseMaterial, GLfloat *specularMaterial, GLfloat shininess) {
    // Configura como o material vai refletir a luz
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambientMaterial); //reação a luz ambiente
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuseMaterial); // ... a luz difusa
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial); // .. a luz especular
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // intensidade da luz
}
*/

void setMaterial(float *ambientDiffuse, float *specular, float shininess) {
    // Define cor base e brilho
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambientDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
