#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "player.h"

void movePlayer(float *speed, Player *playerObject) {
    /**
    Função responsável pelo movimento do player

    **/

    playerObject->x += speed[0];
    playerObject->y += speed[1];
    playerObject->z += speed[2];
}

void loadPlayerEntityData(Player *player) {
    if (!player->entityData) return;

    cgltf_node* node = &player->entityData->nodes[0];
    if (!node->mesh) return;

    cgltf_mesh* mesh = node->mesh;
    cgltf_primitive* prim = &mesh->primitives[0]; // só pega a primeira primitiva por enquanto

    // Atributo de posição
    cgltf_accessor* accessor = prim->attributes[0].data;
    cgltf_buffer_view* view = accessor->buffer_view;
    float* positions = (float*)((char*)view->buffer->data + view->offset + accessor->offset);

    // Índices
    cgltf_accessor* indicesAcc = prim->indices;
    cgltf_buffer_view* indicesView = indicesAcc->buffer_view;
    unsigned short* indices = (unsigned short*)((char*)indicesView->buffer->data + indicesView->offset + indicesAcc->offset);

    // OpenGL buffers
    glGenVertexArrays(1, &player->vao);
    glGenBuffers(1, &player->vbo);
    glGenBuffers(1, &player->ebo);

    glBindVertexArray(player->vao);

    glBindBuffer(GL_ARRAY_BUFFER, player->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 accessor->count * 3 * sizeof(float),
                 positions,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, player->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indicesAcc->count * sizeof(unsigned short),
                 indices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    player->indexCount = indicesAcc->count;
}

void drawPlayer(Player playerObject) {
    glPushMatrix();
        glTranslatef(playerObject.x, playerObject.y, playerObject.z);

        glBindVertexArray(player.vao);
        glDrawElements(GL_TRIANGLES, player.indexCounter, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    glPopMatrix();
}
