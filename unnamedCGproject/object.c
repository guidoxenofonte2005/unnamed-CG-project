#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "object.h"
#include "utils.h"

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

    object->textureID = getTextureFromObject(object->data);
}

void drawObject(SceneObject* object) {
    if (!object->data) {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, object->textureID);

    glPushMatrix();
        glTranslatef(object->x, object->y, object->z);

        for (int i = 0; i < object->data->meshes_count; ++i) {
            cgltf_mesh* mesh = &object->data->meshes[i];
            for (int j = 0; j < mesh->primitives_count; ++j) {
                cgltf_primitive* primitive = &mesh->primitives[j];

                if (primitive->type == cgltf_primitive_type_triangles) {
                    cgltf_accessor* positions_accessor = NULL;
                    cgltf_accessor* normals_accessor = NULL;

                    // Loop para encontrar os atributos de posi��o e normal pelo tipo
                    for (int attr_idx = 0; attr_idx < primitive->attributes_count; ++attr_idx) {
                        cgltf_attribute* attr = &primitive->attributes[attr_idx];
                        if (attr->type == cgltf_attribute_type_position) {
                            positions_accessor = attr->data;
                        } else if (attr->type == cgltf_attribute_type_normal) {
                            normals_accessor = attr->data;
                        }
                    }

                    // Se n�o encontrar os dados de v�rtice, pula para o pr�ximo
                    if (!positions_accessor || !normals_accessor || !primitive->indices) {
                        continue;
                    }
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
                    for (cgltf_size k = 0; k < indices_accessor->count; ++k) {
                        cgltf_size index = cgltf_accessor_read_index(indices_accessor, k);

                        float normal[3];
                        cgltf_accessor_read_float(normals_accessor, index, normal, 3);
                        glNormal3f(normal[X_AXIS], normal[Y_AXIS], normal[Z_AXIS]);

                        if (texture_coords_accessor) {
                            float texturePosition[2];
                            cgltf_accessor_read_float(texture_coords_accessor, index, texturePosition, 2);
                            glTexCoord2f(texturePosition[0], texturePosition[1]);
                        }

                        float position[3];
                        cgltf_accessor_read_float(positions_accessor, index, position, 3);
                        glVertex3f(position[X_AXIS], position[Y_AXIS], position[Z_AXIS]);
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

CollisionBox getPlatformCollisionBox(float centerX, float centerY, float centerZ, float width, float height, float depth) {
    CollisionBox platformCollision;

    platformCollision.minX = centerX - width / 2;
    platformCollision.maxX = centerX + width / 2;
    platformCollision.minY = centerY - height / 2;
    platformCollision.maxY = centerY + height / 2;
    platformCollision.minZ = centerZ - depth / 2;
    platformCollision.maxZ = centerZ + depth / 2;

    return platformCollision;
}
