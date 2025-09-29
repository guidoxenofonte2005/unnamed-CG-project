#include <stdio.h>

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image/stb_image.h"

GLuint loadTexture(const char *filename){
    int largura, altura , canais;
    // largura, altura -> dimensões da imagem
    // canais -> quantos canais de cor (3=RGB, 4=RGBA)

    unsigned char *dados = stbi_load(filename, &largura, &altura, &canais, 0);

    if(!dados){
        printf("Erro para carregar textura %s\n",filename);
        return 0;
    }//facilita pra encontrar o problema caso aconteça

    GLuint texturaID;
    glGenTextures(1, &texturaID); //cria IDs de textura
    glBindTexture(GL_TEXTURE_2D, texturaID);// Ativa essa textura como atual

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //comportamento da textura na horizontal, vai se repetindo quando esta fora do intervalo 0 a 1
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //comportamento da textura na vertical...

    //Filtro de minificação e magnificação, serve para ajustar quando a imagem/textura for maior/menor que a textura original
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //usa interpolação bilineal para reduzir a imagem
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //... para aumentar

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0,
                 (canais == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, dados);

    stbi_image_free(dados); //libera a informações dos dados da imagem para o opengl usar

    return texturaID;
}
