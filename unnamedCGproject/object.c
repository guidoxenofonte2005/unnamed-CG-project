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
}

void drawObject(SceneObject* object) {
    if (!object->data) {
        return;
    }

    glPushMatrix();
        glTranslatef(object->x, object->y, object->z);
        glScalef(0.5f, 0.5f, 0.5f); // lembrar de tirar depois, só botei pq o modelo de teste tava enorme

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
