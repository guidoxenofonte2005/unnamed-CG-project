#include "skybox.h"
#include "texture.h"

GLuint skyboxTex[6];

//0 = right, 1 = left, 2 = top, 3 = bottom, 4 = front, 5 = back
void loadSkybox() {
    skyboxTex[0] = loadTexture("sky_cenario/pz.png");

    skyboxTex[1] = loadTexture("sky_cenario/nz.png");
    skyboxTex[2] = loadTexture("sky_cenario/py.png");
    skyboxTex[3] = loadTexture("sky_cenario/ny.png");
    skyboxTex[4] = loadTexture("sky_cenario/px.png");
    skyboxTex[5] = loadTexture("sky_cenario/nx.png");
}

void drawSkybox(float size){
    glPushMatrix();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//forçar a textura a substituir a cor/material
    glDisable(GL_LIGHTING); // Desativa a iluminação para o skybox
    glDisable(GL_DEPTH_TEST); // Desativa o teste de profundidade para o skybox
    glDisable(GL_CULL_FACE); // garante que as faces internas apareçam
    glEnable(GL_TEXTURE_2D);

    //RIGHT
    glBindTexture(GL_TEXTURE_2D, skyboxTex[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f( size, -size, -size);
        glTexCoord2f(1,1); glVertex3f( size, -size,  size);
        glTexCoord2f(1,0); glVertex3f( size,  size,  size);
        glTexCoord2f(0,0); glVertex3f( size,  size, -size);
    glEnd();

    // LEFT
    glBindTexture(GL_TEXTURE_2D, skyboxTex[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-size, -size,  size);
        glTexCoord2f(1,1); glVertex3f(-size, -size, -size);
        glTexCoord2f(1,0); glVertex3f(-size,  size, -size);
        glTexCoord2f(0,0); glVertex3f(-size,  size,  size);
    glEnd();

    //TOP
    glBindTexture(GL_TEXTURE_2D, skyboxTex[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-size,  size, -size);
        glTexCoord2f(0,1); glVertex3f( size,  size, -size);
        glTexCoord2f(1,1); glVertex3f( size,  size,  size);
        glTexCoord2f(1,0); glVertex3f(-size,  size,  size);
    glEnd();

    //BOTTOM
    glBindTexture(GL_TEXTURE_2D, skyboxTex[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-size, -size,  size);
        glTexCoord2f(1,0); glVertex3f( size, -size,  size);
        glTexCoord2f(1,1); glVertex3f( size, -size, -size);
        glTexCoord2f(0,1); glVertex3f(-size, -size, -size);
    glEnd();

    //FRONT
    glBindTexture(GL_TEXTURE_2D, skyboxTex[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(1,1); glVertex3f(-size, -size, size);
        glTexCoord2f(0,1); glVertex3f( size, -size, size);
        glTexCoord2f(0,0); glVertex3f( size,  size, size);
        glTexCoord2f(1,0); glVertex3f(-size,  size, size);
    glEnd();

    // BACK
    glBindTexture(GL_TEXTURE_2D, skyboxTex[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(1,1); glVertex3f( size, -size, -size);
        glTexCoord2f(0,1); glVertex3f(-size, -size, -size);
        glTexCoord2f(0,0); glVertex3f(-size,  size, -size);
        glTexCoord2f(1,0); glVertex3f( size,  size, -size);
    glEnd();

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//forçar a textura a substituir a cor/material
    glPopMatrix();
}
