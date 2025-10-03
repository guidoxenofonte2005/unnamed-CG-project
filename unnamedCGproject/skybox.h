#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glut.h>

// IDs das texturas do skybox
extern GLuint skyboxTex[6];

void loadSkybox();
void drawSkybox(float size);

#endif