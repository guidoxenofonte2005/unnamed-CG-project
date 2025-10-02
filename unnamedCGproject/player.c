#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "player.h"
#include "object.h"
#include "utils.h"

// Define a macro de implementação da biblioteca
#define CGLTF_IMPLEMENTATION
#include "libs/cgltf/cgltf.h"

GLuint playerTextureID = 0;

//                                             minX   minY   minZ  maxX, maxY, maxZ
ObjectCollisionOffset playerCollisionOffset = {0.0f, -0.5f, -0.5f, 0.0f, 2.0f, 0.0f};

void movePlayer(float *speed, Player *playerObject) {
    printf("%f\n", speed[Y_AXIS]);
    playerObject->x += speed[X_AXIS];
    playerObject->y += speed[Y_AXIS];
    playerObject->z += speed[Z_AXIS];

    playerObject->collision.minX += speed[X_AXIS];
    playerObject->collision.minY += speed[Y_AXIS];
    playerObject->collision.minZ += speed[Z_AXIS];
    playerObject->collision.maxX += speed[X_AXIS];
    playerObject->collision.maxY += speed[Y_AXIS];
    playerObject->collision.maxZ += speed[Z_AXIS];

    // printf("%f\n", speed[1]);
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
    playerTextureID = getTextureFromObject(playerObj->modelData);
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

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, playerTextureID);

    for (int i = 0; i < playerObj->modelData->meshes_count; ++i) {
        cgltf_mesh* mesh = &playerObj->modelData->meshes[i];

        for (int j = 0; j < mesh->primitives_count; ++j) {
            cgltf_primitive* primitive = &mesh->primitives[j];

            if (primitive->type == cgltf_primitive_type_triangles) {
                cgltf_accessor* positions_accessor = primitive->attributes[0].data;
                cgltf_accessor* normals_accessor = primitive->attributes[1].data;

                // parte que pega a textura do player
                cgltf_accessor* texture_coords_accessor = NULL;
                for (int w = 0; w < primitive->attributes_count; w++) {
                    if (primitive->attributes[w].type == cgltf_attribute_type_texcoord) {
                        texture_coords_accessor = primitive->attributes[w].data;
                        break;
                    }
                }

                cgltf_accessor* indices_accessor = primitive->indices;


                glBegin(GL_TRIANGLES);

                // Itera sobre os índdices para desenhar os triângulos dos modelos
                // a ordem certa de iteração é normal -> textura -> posição
                for (cgltf_size k = 0; k < indices_accessor->count; ++k) {
                    // Lê o índice do vértice atual
                    cgltf_size index = cgltf_accessor_read_index(indices_accessor, k);

                    // Lê a normal do vértice e a define para o cálculo de iluminação
                    float normal[3];
                    cgltf_accessor_read_float(normals_accessor, index, normal, 3);
                    glNormal3f(normal[0], normal[1], normal[2]);

                    // se tiver textura associada, coloca
                    if (texture_coords_accessor) {
                        float texturePosition[2];
                        cgltf_accessor_read_float(texture_coords_accessor, index, texturePosition, 2);
                        glTexCoord2f(texturePosition[0], texturePosition[1]);
                    }

                    // Lê a posição do vértice e desenha com a função OpenGl
                    float position[3];
                    cgltf_accessor_read_float(positions_accessor, index, position, 3);
                    glVertex3f(position[0], position[1], position[2]);
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

                    minX = accessor->min[X_AXIS];
                    minY = accessor->min[Y_AXIS];
                    minZ = accessor->min[Z_AXIS];
                    maxX = accessor->max[X_AXIS];
                    maxY = accessor->max[Y_AXIS];
                    maxZ = accessor->max[Z_AXIS];
                }
            }
        }
    }
    // posição de colisão  min/Max   posição     offset de colisão (torna maior ou menor)
    player->collision.minX = minX + player->x + playerCollisionOffset.offsetMinX;
    player->collision.maxX = maxX + player->x + playerCollisionOffset.offsetMaxX;
    player->collision.minY = minY + player->y + playerCollisionOffset.offsetMinY;
    player->collision.maxY = maxY + player->y + playerCollisionOffset.offsetMaxY;
    player->collision.minZ = minZ + player->z + playerCollisionOffset.offsetMinZ;
    player->collision.maxZ = maxZ + player->z + playerCollisionOffset.offsetMaxZ;

    // printf("PLAYER: %f, %f, %f - %f, %f, %f\n", player->collision.minX, player->collision.minY, player->collision.minZ, player->collision.maxX, player->collision.maxY, player->collision.maxZ);
}

void collideAndSlide(float *speed, Player *player, SceneObject *objectsInRange, int qtdObjInRange, float deltaTime) {
    // Guarda posição anterior
    float oldX = player->x, oldY = player->y, oldZ = player->z;
    // Movimento desejado para este step (sua estrutura atual usa *speed* como deslocamento por step)
    float move[3] = { speed[X_AXIS], speed[Y_AXIS], speed[Z_AXIS] };

    // Offsets constantes da caixa de colisão em relação à posição do jogador
    float topOffset = player->collision.maxY - player->y;
    float bottomOffset = player->collision.minY - player->y;

    // Assume no chão até provar o contrário
    player->isOnGround = false;

    // --- 1) Move X e resolve colisões X ---
    float mv[3] = { move[X_AXIS], 0.0f, 0.0f };
    movePlayer(mv, player);

    for (int i = 0; i < qtdObjInRange; ++i) {
        SceneObject *currentObj = &objectsInRange[i];
        if (isObjectColliding(player->collision, currentObj->collision)) {
            // Reverte X
            player->x = oldX;
            getPlayerCollisionBox(player);
            // Cancela movimento/velocidade X
            speed[X_AXIS] = 0.0f;
            move[X_AXIS] = 0.0f;
            break;
        }
    }

    // --- 2) Move Z e resolve colisões Z ---
    oldZ = player->z;
    mv[0] = 0.0f; mv[1] = 0.0f; mv[2] = move[Z_AXIS];
    movePlayer(mv, player);

    for (int i = 0; i < qtdObjInRange; ++i) {
        SceneObject *currentObj = &objectsInRange[i];
        if (isObjectColliding(player->collision, currentObj->collision)) {
            // Reverte Z
            player->z = oldZ;
            getPlayerCollisionBox(player);
            // Cancela movimento/velocidade Z
            speed[Z_AXIS] = 0.0f;
            move[Z_AXIS] = 0.0f;
            break;
        }
    }

    // --- 3) Move Y e resolve colisões Y (aqui definimos "no chão" corretamente) ---
    oldY = player->y;
    mv[0] = 0.0f; mv[1] = move[Y_AXIS]; mv[2] = 0.0f;
    movePlayer(mv, player);

    for (int i = 0; i < qtdObjInRange; ++i) {
        SceneObject *currentObj = &objectsInRange[i];
        if (isObjectColliding(player->collision, currentObj->collision)) {
            CollisionSide side = getCollidingObjectSide(player->collision, currentObj->collision);

            // Só reconhece "estar em cima" se a colisão vier de cima e o movimento vertical for pra baixo
            if (side == TOP && move[Y_AXIS] <= 0.0f) {
                player->isOnGround = true;
                // reposiciona o player exatamente em cima usando o offset calculado
                const float EPSILON = 0.001f;
                player->y = currentObj->collision.maxY + EPS - topOffset;
                getPlayerCollisionBox(player);

                // anula movimento/velocidade vertical
                speed[Y_AXIS] = 0.0f;
                move[Y_AXIS] = 0.0f;
            }
            else if (side == BOTTOM && move[Y_AXIS] >= 0.0f) {
                // bateu a cabeça — posiciona logo abaixo do teto
                const float EPS = 0.001f;
                player->y = currentObj->collision.minY - EPS - bottomOffset;
                getPlayerCollisionBox(player);

                speed[Y_AXIS] = 0.0f;
                move[Y_AXIS] = 0.0f;
            }
            else {
                // colisão lateral inesperada em Y: apenas reverte Y e zera componente vertical
                player->y = oldY;
                getPlayerCollisionBox(player);
                speed[Y_AXIS] = 0.0f;
                move[Y_AXIS] = 0.0f;
            }
            break;
        }
    }
}
