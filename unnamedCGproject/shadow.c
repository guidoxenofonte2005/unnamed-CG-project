#include "player.h"
#include "object.h"
#include "shadow.h"

#include <GL/freeglut.h>

// ------------------------------------------------------------
// Função: makeShadowMatrix
// Entrada: plano (A,B,C,D) e posição da luz (x,y,z,w)
// Saída: matriz de sombra 4x4 (1D array com 16 elementos)
// ------------------------------------------------------------
void makeShadowMatrix(GLfloat plane[4], GLfloat lightPos[4], GLfloat shadowMat[16]) {
    GLfloat dot = plane[0] * lightPos[0] +
                  plane[1] * lightPos[1] +
                  plane[2] * lightPos[2] +
                  plane[3] * lightPos[3]; // pega o produto escalar das 4 componentes

    shadowMat[0]  = dot - lightPos[0] * plane[0]; // X TRANSFORM - 1st index
    shadowMat[4]  = 0.0f - lightPos[0] * plane[1]; // Y TRANSFORM - 1st index
    shadowMat[8]  = 0.0f - lightPos[0] * plane[2]; // Z TRANSFORM - 1st index
    shadowMat[12] = 0.0f - lightPos[0] * plane[3]; // HOMOGENOUS TRANSFORM - 1st index

    shadowMat[1]  = 0.0f - lightPos[1] * plane[0]; // X TRANSFORM - 2nd index
    shadowMat[5]  = dot - lightPos[1] * plane[1]; // Y TRANSFORM - 2nd index
    shadowMat[9]  = 0.0f - lightPos[1] * plane[2]; // Z TRANSFORM - 2nd index
    shadowMat[13] = 0.0f - lightPos[1] * plane[3]; // HOMOGENOUS TRANSFORM - 2nd index

    shadowMat[2]  = 0.0f - lightPos[2] * plane[0]; // X TRANSFORM - 3rd index
    shadowMat[6]  = 0.0f - lightPos[2] * plane[1]; // Y TRANSFORM - 3rd index
    shadowMat[10] = dot - lightPos[2] * plane[2]; // Z TRANSFORM - 3rd index
    shadowMat[14] = 0.0f - lightPos[2] * plane[3]; // HOMOGENOUS TRANSFORM - 3rd index

    shadowMat[3]  = 0.0f - lightPos[3] * plane[0]; // X TRANSFORM - 4th index
    shadowMat[7]  = 0.0f - lightPos[3] * plane[1]; // Y TRANSFORM - 4th index
    shadowMat[11] = 0.0f - lightPos[3] * plane[2]; // Z TRANSFORM - 4th index
    shadowMat[15] = dot - lightPos[3] * plane[3]; // HOMOGENOUS TRANSFORM - 4th index
}

// ------------------------------------------------------------
// Função: drawShadow
// Desenha a sombra de um objeto em relação a um plano
// Entrada: plano, luz e função de desenho do objeto
// ------------------------------------------------------------
void drawShadow(Player *player, SceneObject *objectsInCollisionRange, float playerRotation) {
    if (player->isOnGround && player->groundObjectIndex >= 0) {
        SceneObject *groundObj = &objectsInCollisionRange[player->groundObjectIndex];

        // Define o plano (y = constante da parte superior da plataforma)
        GLfloat plane[4] = {0.0f, 1.0f, 0.0f, -(groundObj->collision.maxY + 0.1f)};
        GLfloat shadowMat[16];
        GLfloat lightPos[] = {200.0f, 500.0f, 200.0f, 1.0f};

        // Cria a matriz de sombra (usa a função do shadow.c)
        makeShadowMatrix(plane, lightPos, shadowMat);

        // Desenha a sombra diretamente
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPushMatrix();
            glMultMatrixf(shadowMat);
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);

            // Desenha o modelo do player projetado
            glTranslatef(player->x, player->y, player->z);
            glRotatef(playerRotation, 0.0f, 1.0f, 0.0f);
            glScalef(1.0f, 1.0f, 1.0f);

            // Usa o mesmo desenho do modelo do player
            if (player->modelData) {
                for (int i = 0; i < player->modelData->meshes_count; ++i) {
                    cgltf_mesh* mesh = &player->modelData->meshes[i];
                    for (int j = 0; j < mesh->primitives_count; ++j) {
                        cgltf_primitive* primitive = &mesh->primitives[j];
                        if (primitive->type == cgltf_primitive_type_triangles) {
                            cgltf_accessor* positions_accessor = primitive->attributes[0].data;
                            cgltf_accessor* indices_accessor = primitive->indices;

                            glBegin(GL_TRIANGLES);
                            for (cgltf_size k = 0; k < indices_accessor->count; ++k) {
                                cgltf_size index = cgltf_accessor_read_index(indices_accessor, k);
                                float pos[3];
                                cgltf_accessor_read_float(positions_accessor, index, pos, 3);
                                glVertex3f(pos[0], pos[1], pos[2]);
                            }
                            glEnd();
                        }
                    }
                }
            }

        glPopMatrix();
        glPopAttrib();
    }
}
