#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "textura.h"
#include <stdio.h>

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
/*
    GLenum formato = (canais == 4) ? GL_RGBA : GL_RGB;//Serve para identificar se é uma png ou jpg

    glTexImage2D(GL_TEXTURE_2D, // Tipo de textura
             0,                // Nível mipmap, ai colocamos a textura original
             formato,          // Formato interno (como a GPU guarda)
             largura, altura,  // Dimensões da imagem
             0,                // Borda (sempre 0)
             formato,          // Formato dos dados da imagem
             GL_UNSIGNED_BYTE, // Tipo dos dados (1 byte por canal)
             dados);           // Ponteiro para os pixels da imagem
*/

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0,
                 (canais == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, dados);

    //gera versões menores das texturas para os objetos longe da câmera, e deixa a imagem melhor
 //   gluBuild2DMipmaps(GL_TEXTURE_2D, formato, largura, altura, formato, GL_UNSIGNED_BYTE, dados);

    stbi_image_free(dados); //libera a informações dos dados da imagem para o opengl usar

    return texturaID;
}
