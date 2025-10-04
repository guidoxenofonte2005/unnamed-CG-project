#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/freeglut.h>

#include "libs/cgltf/cgltf.h"

typedef struct {
    GLuint texBack, texFront, texLeft, texRight;
    GLuint texTop, texBase;
} PlatformTextureTypes;

// Função para carregar uma textura a partir de um arquivo
GLuint loadTexture(const char *filename);

GLuint getTextureFromObject(cgltf_data* modelData);

#endif // TEXTURE_H
