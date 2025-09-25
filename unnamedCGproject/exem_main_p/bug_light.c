// main.c
// ----------------------------------------------------
// Exemplo de cena simples em OpenGL com iluminação básica
// Utiliza a função setupLighting definida em iluminacao.c
// ----------------------------------------------------

#include <GL/glut.h>       // Biblioteca principal do OpenGL + GLUT
#include "iluminacao.h"    // Cabeçalho com a função setupLighting

// ----------------------------------------
// Função de inicialização da cena
// ----------------------------------------
void init() {
    glEnable(GL_DEPTH_TEST);   // Ativa o Z-buffer (profundidade), necessário para 3D
    glShadeModel(GL_SMOOTH);   // Ativa sombreamento suave (Gouraud Shading)
    glEnable(GL_NORMALIZE);    // Normaliza vetores normais (importante para iluminação correta)

    // Define a cor de fundo da janela (cinza claro RGBA)
    glClearColor(0.7, 0.7, 0.7, 1.0);
}

// ----------------------------------------
// Função de desenho (renderização da cena)
// ----------------------------------------
void display() {
    // Limpa o buffer de cor e profundidade antes de desenhar
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reseta a matriz de transformações
    glLoadIdentity();

    // Configura a câmera:
    // gluLookAt( posição da câmera, ponto que está olhando, vetor "up" )
    gluLookAt(3.0, 3.0, 5.0,   // posição da câmera no espaço
              0.0, 0.0, 0.0,   // olha para a origem (centro da cena)
              0.0, 1.0, 0.0);  // vetor "para cima" (eixo Y positivo)

    // ---------------------------
    // Definição da luz
    // ---------------------------
    GLfloat ambientLight[]  = {0.3, 0.3, 0.3, 1.0};  // Componente ambiente (iluminação geral)
    GLfloat diffuseLight[]  = {0.9, 0.9, 0.9, 1.0};  // Componente difusa (espalhamento)
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};  // Componente especular (brilho)
    GLfloat lightPosition[] = {2.0, 2.0, 5.0, 1.0};  // Posição da luz (w=1.0 -> luz pontual)

    // ---------------------------
    // Definição do material do objeto
    // ---------------------------
    GLfloat ambientMaterial[]  = {0.8, 0.8, 0.8, 1.0}; // Reação à luz ambiente
    GLfloat diffuseMaterial[]  = {0.8, 0.8, 0.8, 1.0}; // Reação à luz difusa
    GLfloat specularMaterial[] = {1.0, 1.0, 1.0, 1.0}; // Reação à luz especular
    GLfloat shininess          = 30.0;                 // Brilho (quanto maior, mais concentrado)

    // ---------------------------
    // Chama a função de iluminação
    // ---------------------------
    setupLighting(ambientLight, diffuseLight, specularLight, lightPosition,
                  ambientMaterial, diffuseMaterial, specularMaterial, shininess);

    // ---------------------------
    // Desenha o objeto (esfera)
    // ---------------------------
    glutSolidSphere(1.0, 50, 50);  // raio = 1.0, subdivisões = 50x50

    // Troca os buffers (double buffering):
    //  - BACK BUFFER (onde desenhamos agora)
    //  - FRONT BUFFER (mostrado na tela)
    glutSwapBuffers();
}

// ----------------------------------------
// Função chamada sempre que a janela muda de tamanho
// Responsável por configurar a projeção da câmera
// ----------------------------------------
void reshape(int w, int h) {
    // Define a área de desenho para cobrir toda a janela
    glViewport(0, 0, w, h);

    // Alterna para a matriz de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Define uma projeção em perspectiva
    // (ângulo de visão, aspecto da tela, plano próximo, plano distante)
    gluPerspective(60.0, (float)w / (float)h, 1.0, 20.0);

    // Volta para a matriz de modelagem/visualização
    glMatrixMode(GL_MODELVIEW);
}

// ----------------------------------------
// Programa principal
// ----------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Inicializa o GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Duplo buffer + cores + profundidade
    glutInitWindowSize(600, 600);  // Tamanho inicial da janela
    glutCreateWindow("Exemplo de Iluminacao - Comentado"); // Título da janela

    init();                           // Configurações iniciais
    glutDisplayFunc(display);         // Função de desenho
    glutReshapeFunc(reshape);         // Função de redimensionamento
    glutMainLoop();                   // Loop principal (nunca retorna)

    return 0;
}
