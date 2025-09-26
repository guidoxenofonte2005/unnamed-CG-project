#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "player.h"
#include "utils.h"

// Define a macro de implementação da biblioteca
#define CGLTF_IMPLEMENTATION
#include "libs/cgltf/cgltf.h"

//                                             minX   minY   minZ  maxX, maxY, maxZ
ObjectCollisionOffset playerCollisionOffset = {0.0f, -0.5f, -0.5f, 0.0f, 2.0f, 0.0f};

void movePlayer(float *speed, Player *playerObject) {
    playerObject->x += speed[0];
    playerObject->y += speed[1];
    playerObject->z += speed[2];

    playerObject->collision.minX += speed[0];
    playerObject->collision.minY += speed[1];
    playerObject->collision.minZ += speed[2];
    playerObject->collision.maxX += speed[0];
    playerObject->collision.maxY += speed[1];
    playerObject->collision.maxZ += speed[2];
}

// A função agora recebe um ponteiro para a struct Player.
void loadPlayerModel(Player* playerObj, const char* filename) {
    // Inicializa as opções e tenta carregar o arquicvo GLTF
    cgltf_options options = {0};
    // Tenta carregar o modelo e salva os dados no novo membro 'modelData'.
    cgltf_result result = cgltf_parse_file(&options, filename, &playerObj->modelData);

    if (result != cgltf_result_success) {
        printf("Erro ao carregar o arquivo GLTF: %d\n", result);
        return;
    }

    // Carrega os dados binários do modelo na memória
    result = cgltf_load_buffers(&options, playerObj->modelData, filename);
    if (result != cgltf_result_success) {
        printf("Erro ao carregar os buffers do arquivo GLTF: %d\n", result);
        cgltf_free(playerObj->modelData);
    }

    getPlayerCollisionBox(playerObj);
}

//  A função agora recebe um ponteiro para a struct Player.
void drawPlayerModel(Player* playerObj, float thetaAngle) {
    if (!playerObj->modelData) {
        return;
    }

    // A lógica de desenho do modelo 3D
    // Itera sobre todas as malhas e primitivas do modelo 3D

    glPushMatrix(); // Salva o estado atual da matriz para evitar que o player afete a cena toda
    glTranslatef(playerObj->x, playerObj->y, playerObj->z);
    glRotatef(thetaAngle, 0.0f, 1.0f, 0.0f);
    for (int i = 0; i < playerObj->modelData->meshes_count; ++i) {
        cgltf_mesh* mesh = &playerObj->modelData->meshes[i];

        for (int j = 0; j < mesh->primitives_count; ++j) {
            cgltf_primitive* primitive = &mesh->primitives[j];

            if (primitive->type == cgltf_primitive_type_triangles) {
                cgltf_accessor* positions_accessor = primitive->attributes[0].data;
                cgltf_accessor* normals_accessor = primitive->attributes[1].data;
                cgltf_accessor* indices_accessor = primitive->indices;


                glBegin(GL_TRIANGLES);

                // Itera sobre os índdices para desenhar os triângulos dos modelos
                for (cgltf_size k = 0; k < indices_accessor->count; ++k) {
                    // Lê o índice do vértice atual
                    cgltf_size index = cgltf_accessor_read_index(indices_accessor, k);

                    // Lê a posição do vértice e desenha com a função OpenGl
                    float position[3];
                    cgltf_accessor_read_float(positions_accessor, index, position, 3);
                    glVertex3f(position[0], position[1], position[2]);

                    // Lê a normal do vértice e a define para o cálculo de iluminação
                    float normal[3];
                    cgltf_accessor_read_float(normals_accessor, index, normal, 3);
                    glNormal3f(normal[0], normal[1], normal[2]);
                }
                glEnd();
            }
        }
    }
    glPopMatrix(); // Restaura o estado da matriz
}

void cleanupPlayerModel(Player* playerObj) {
    // Libera a memória alocada para os dados do modelo 3d
    if (playerObj->modelData) {
        cgltf_free(playerObj->modelData);
        playerObj->modelData = NULL;
    }
}

// pega a collisionBox do player
void getPlayerCollisionBox(Player *player) {
    cgltf_data *data = player->modelData;

    float minX = 0.0f, minY = 0.0f, minZ = 0.0f, maxX = 0.0f, maxY = 0.0f, maxZ = 0.0f;

    for (size_t meshIndex = 0; meshIndex < data->meshes_count; ++meshIndex) {
        cgltf_mesh* mesh = &data->meshes[meshIndex];
        for (size_t primIndex = 0; primIndex < mesh->primitives_count; ++primIndex) {
            cgltf_primitive* primitive = &mesh->primitives[primIndex];
            for (size_t attrIndex = 0; attrIndex < primitive->attributes_count; ++attrIndex) {
                cgltf_attribute* attr = &primitive->attributes[attrIndex];
                if (attr->type == cgltf_attribute_type_position) {
                    cgltf_accessor* accessor = attr->data;

                    minX = accessor->min[0];
                    minY = accessor->min[1];
                    minZ = accessor->min[2];
                    maxX = accessor->max[0];
                    maxY = accessor->max[1];
                    maxZ = accessor->max[2];
                }
            }
        }
    }

    // os números float são um ajuste manual da colisão do player
    player->collision.minX = minX + player->x + playerCollisionOffset.offsetMinX;
    player->collision.maxX = maxX + player->x + playerCollisionOffset.offsetMaxX;
    player->collision.minY = minY + player->y + playerCollisionOffset.offsetMinY;
    player->collision.maxY = maxY + player->y + playerCollisionOffset.offsetMaxY;
    player->collision.minZ = minZ + player->z + playerCollisionOffset.offsetMinZ;
    player->collision.maxZ = maxZ + player->z + playerCollisionOffset.offsetMaxZ;

    printf("PLAYER: %f, %f, %f - %f, %f, %f\n", player->collision.minX, player->collision.minY, player->collision.minZ, player->collision.maxX, player->collision.maxY, player->collision.maxZ);
}
