#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "object.h"

void loadObject(SceneObject* object, const char* filename, float x, float y, float z) {
    cgltf_options options = {0};
    cgltf_result result = cgltf_parse_file(&options, filename, &object->data);

    if (result != cgltf_result_success) {
        printf("Erro ao carregar o arquivo GLTF: %d\n", result);
        return;
    }

    result = cgltf_load_buffers(&options, object->data, filename);
    if (result != cgltf_result_success) {
        printf("Erro ao carregar os buffers do arquivo GLTF: %d\n", result);
        cgltf_free(object->data);
        return;
    }

    object->x = x;
    object->y = y;
    object->z = z;

    getCollisionBoxFromObject(object);
}

void drawObject(SceneObject* object) {
    if (!object->data) {
        return;
    }

    glPushMatrix();
        glTranslatef(object->x, object->y, object->z);

        for (int i = 0; i < object->data->meshes_count; ++i) {
            cgltf_mesh* mesh = &object->data->meshes[i];
            for (int j = 0; j < mesh->primitives_count; ++j) {
                cgltf_primitive* primitive = &mesh->primitives[j];

                if (primitive->type == cgltf_primitive_type_triangles) {
                    cgltf_accessor* positions_accessor = primitive->attributes[0].data;
                    cgltf_accessor* normals_accessor = primitive->attributes[1].data;
                    cgltf_accessor* indices_accessor = primitive->indices;

                    glBegin(GL_TRIANGLES);
                    for (cgltf_size k = 0; k < indices_accessor->count; ++k) {
                        cgltf_size index = cgltf_accessor_read_index(indices_accessor, k);

                        float position[3];
                        cgltf_accessor_read_float(positions_accessor, index, position, 3);
                        glVertex3f(position[0], position[1], position[2]);

                        float normal[3];
                        cgltf_accessor_read_float(normals_accessor, index, normal, 3);
                        glNormal3f(normal[0], normal[1], normal[2]);
                    }
                    glEnd();
                }
            }
        }
    glPopMatrix();
}

void cleanupObject(SceneObject* object) {
    if (object->data) {
        cgltf_free(object->data);
        object->data = NULL;
    }
}

// pega a collisionBox de um determinado objeto
void getCollisionBoxFromObject(SceneObject *object) {
    cgltf_data *data = object->data;

    if (data != NULL) {
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

        object->collision.minX = minX + object->x;
        object->collision.maxX = maxX + object->x;
        object->collision.minY = minY + object->y;
        object->collision.maxY = maxY + object->y;
        object->collision.minZ = minZ + object->z;
        object->collision.maxZ = maxZ + object->z;
    }


    // printf("OBJ: %f, %f, %f - %f, %f, %f\n", object->collision.minX, object->collision.minY, object->collision.minZ, object->collision.maxX, object->collision.maxY, object->collision.maxZ);
}


// platform related thingys
void loadPlatform(SceneObject *sceneObjects, int *qtdSceneObjects, float x, float y, float z, CollisionBox *platformCollision) {
    SceneObject *newPlatform = &sceneObjects[(*qtdSceneObjects)++];

    newPlatform->x = x;
    newPlatform->y = y;
    newPlatform->z = z;

    newPlatform->collision = (*platformCollision);
    newPlatform->data = NULL;
}

GLuint texFront, texBack, texLeft, texRight, texTop, texBase;

void drawPlatform(SceneObject *platform){
    glPushMatrix();//Salva a atual matriz
    glTranslatef(platform->x, platform->y, platform->z); // Move a plataforma para a posição dela na cena

    float minX = platform->collision.minX - platform->x;
    float maxX = platform->collision.maxX - platform->x;
    float minY = platform->collision.minY - platform->y;
    float maxY = platform->collision.maxY - platform->y;
    float minZ = platform->collision.minZ - platform->z;
    float maxZ = platform->collision.maxZ - platform->z;

    glBindTexture(GL_TEXTURE_2D, texFront); // Ativa a textura da frente
    glBegin(GL_QUADS); // Início do desenho de um quadrado
        glTexCoord2f(0,0); glVertex3f(minX, minY, maxZ); // canto inferior esquerdo
        glTexCoord2f(2,0); glVertex3f(maxX, minY, maxZ); // canto inferior direito
        glTexCoord2f(2,2); glVertex3f(maxX, maxY, maxZ); // canto superior direito
        glTexCoord2f(0,2); glVertex3f(minX, maxY, maxZ); // canto superior esquerdo
    glEnd();

    // Face de trás
    glBindTexture(GL_TEXTURE_2D, texBack);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(maxX, minY, minZ);
        glTexCoord2f(2,0); glVertex3f(minX, minY, minZ);
        glTexCoord2f(2,2); glVertex3f(minX, maxY, minZ);
        glTexCoord2f(0,2); glVertex3f(maxX, maxY, minZ);
    glEnd();

    // Face da direita
    glBindTexture(GL_TEXTURE_2D, texRight);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(maxX, minY, maxZ);
        glTexCoord2f(2,0); glVertex3f(maxX, minY, minZ);
        glTexCoord2f(2,2); glVertex3f(maxX, maxY, minZ);
        glTexCoord2f(0,2); glVertex3f(maxX, maxY, maxZ);
    glEnd();

    // Face da esquerda
    glBindTexture(GL_TEXTURE_2D, texLeft);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
        glTexCoord2f(2,0); glVertex3f(minX, minY, maxZ);
        glTexCoord2f(2,2); glVertex3f(minX, maxY, maxZ);
        glTexCoord2f(0,2); glVertex3f(minX, maxY, minZ);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texTop); // substituir por texTop
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0,0); glVertex3f(minX, maxY, minZ);
        glTexCoord2f(2,0); glVertex3f(minX, maxY, maxZ);
        glTexCoord2f(2,2); glVertex3f(maxX, maxY, maxZ);
        glTexCoord2f(0,2); glVertex3f(maxX, maxY, minZ);
    glEnd();


    // Base da plataforma
    // (aqui reaproveitei texBack, mas pode ser outra textura só para a base)
    glBindTexture(GL_TEXTURE_2D, texBase);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
        glTexCoord2f(2,0); glVertex3f(maxX, minY, minZ);
        glTexCoord2f(1,2); glVertex3f(maxX, minY, maxZ);
        glTexCoord2f(0,2); glVertex3f(minX, minY, maxZ);
    glEnd();

    glPopMatrix();//Restaura a matriz salva
}

// load everything up
void loadScenario(SceneObject *sceneObjects, int *qtdSceneObjects) {

}
