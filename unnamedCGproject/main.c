#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <GL/freeglut.h>

#include <math.h>
#include <stdbool.h>

#include "player.h"
#include "object.h"
#include "utils.h"

int verticalMovement;
int horizontalMovement;

int lastMousex, lastMousey;

// ângulo horizontal
float thetaAngle = 0.0f;
// ângulo vertical
float phiAngle = 0.0f;
// distância da câmera
float camRadius = 25.0f;

// ângulo de rotação do player
float playerRotation = 0.0f;

bool isCameraActive = false;
int winWidth = 1000, winHeight = 750;

Player player = {0.0f, 0.0f, 0.0f, true, true, IDLE, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, NULL};
PlayerMoveKeys moveKeys = {false, false, false, false};

float playerVelocity[] = {0.0f, 0.0f, 0.0f};

float deltaTime = 0.0f;

#define MAX_OBJECTS 10 // Define o máximo de objetos na cena
SceneObject sceneObjects[MAX_OBJECTS];
int objectCount = 0;

int init() {
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    // Habilita o teste de profundidade para a remoção de superfícies ocultas, garantindo que objetos mais próximos da câmera sejam desenhados por cima de objetos mais distantes.
    glEnable(GL_DEPTH_TEST);
    // Habilita o sistema de iluminação global do OpenGL, Sem isso, os modelos apareceriam sem sombreamento.
    glEnable(GL_LIGHTING);
    // Habilita o z-buffer
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Configura uma projeção perspectiva, que simula a visão humana (objetos distantes parecem menores).
    gluPerspective(60.0, (float)winWidth / (float)winHeight, 0.1, 100.0); // Ajustado para usar as variáveis de janela

    //1. Define a posição inicial do jogador (à esquerda)
    player.x = -8.0f;
    player.y = 0.0f;
    player.z = 0.0f;

    // Chamada para carregar o modelo 3D uma única vez durante a inicialização.
    loadPlayerModel(&player, "3dfiles/player.glb");

    // Carregando outros objetos da cena e add em um array
    // loadObject(&sceneObjects[objectCount++], "3dfiles/grass1.glb", 0.0f, -1.0f, 0.0f); // Exemplo de um chão
    // loadObject(&sceneObjects[objectCount++], "3dfiles/tree1.glb", 8.0f, 0.0f, 0.0f); // Exemplo de um objeto na posição (9,0,0)
    loadObject(&sceneObjects[objectCount++], "3dfiles/hydrant.glb", 15.0f, 0.0f, -10.0f); // Exemplo de um objeto na posição (15,0,0)
    //loadObject(&sceneObjects[objectCount++], "3dfiles/tree1.glb", -5.0f, 0.0f, 2.0f); // Exemplo de um objeto na posição (25,0,0)

    getCollisionBoxFromObject(&sceneObjects[0]);
    getPlayerCollisionBox(&player);
    return 1;
}

void display() {
    // Limpa o buffer de cor e o de profundidade em cada frame.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = player.x + camRadius * cosf(phiAngle) * cosf(thetaAngle);
    float camY = player.y + camRadius * sinf(phiAngle);
    float camZ = player.z + camRadius * cosf(phiAngle) * sinf(thetaAngle);

    // Define a orientação da câmera
    gluLookAt(camX, camY, camZ,
              player.x, player.y + 4, player.z,
              0.0, 1.0, 0.0);

    // Definindo as propriedades da fonte de luz
    GLfloat ambientLight[]  = {0.2f, 0.2f, 0.2f, 1.0f};  // Luz ambiente fraca
    GLfloat diffuseLight[]  = {0.8f, 0.8f, 0.8f, 1.0f};  // Luz difusa branca
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Brilho especular branco
    GLfloat lightPosition[] = {10.0f, 10.0f, 10.0f, 1.0f}; // Posição da luz

    // Define as propriedades do material (pode ser genérico para todos os objetos)
    GLfloat ambientMaterial[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat diffuseMaterial[]  = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat specularMaterial[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Pouco brilho
    GLfloat shininess = 20;

    // chama a função para aplicar iluminação
    setupLighting(ambientLight, diffuseLight, specularLight, lightPosition, ambientMaterial, diffuseMaterial, specularMaterial, shininess);

    // chama função para desenhar o modelo 3D na tela a cada frane
    drawPlayerModel(&player, playerRotation);

    // Desenha todos os objetos da cena
    for (int i = 0; i < objectCount; ++i) {
        drawObject(&sceneObjects[i]);
    }


    // Troca o buffer de desenho para exibir a nova cena.
    glutSwapBuffers();
}

void handleKeyboardInput(unsigned char pressedKey, int x, int y) {
    if (pressedKey == 27) { // ESC
        isCameraActive = !isCameraActive;

        if (isCameraActive) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(winWidth / 2, winHeight / 2);
        } else {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    }
    if (isCameraActive) {
        if (pressedKey == 119) {
            moveKeys.w = true;
        }
        if (pressedKey == 97) {
            moveKeys.a = true;
        }
        if (pressedKey == 115) {
            moveKeys.s = true;
        }
        if (pressedKey == 100) {
            moveKeys.d = true;
        }
    }
}

void keyboardKeyUp(unsigned char key, int x, int y) {
    if (key == 119) {
        moveKeys.w = false;
    }
    if (key == 97) {
        moveKeys.a = false;
    }
    if (key == 115) {
        moveKeys.s = false;
    }
    if (key == 100) {
        moveKeys.d = false;
    }
}

void idleUpdates() {
    deltaTime = getDeltaTime();

    getPlayerVelocity(playerVelocity, &moveKeys, phiAngle, thetaAngle, deltaTime);
    movePlayer(playerVelocity, &player);

    getPlayerMovingAngle(playerVelocity, &playerRotation);

    if (isObjectColliding(player.collision, sceneObjects[0].collision)) printf("%d", getCollidingObjectSide(player.collision, sceneObjects[0].collision));

    //printf("%d", getCollidingObjectSide(player.collision, sceneObjects[0].collision));
}

void handleMouseMovement(int x, int y) {
    if (!isCameraActive) return;

    int centerX = winWidth / 2;
    int centerY = winHeight / 2;

    int dx = x - centerX;
    int dy = y - centerY;

    float mouseSensitivity = 0.005f;

    thetaAngle += dx * mouseSensitivity;
    phiAngle += dy * mouseSensitivity;

    if (phiAngle > 1.55f)  phiAngle = 1.55f;
    if (phiAngle < -1.55f) phiAngle = -1.55f;

    glutWarpPointer(centerX, centerY);
    glutPostRedisplay();
}

void handleCloseProgram() {
    // A função agora chama a função de limpeza do modelo do jogador é chamada
    // passando a struct 'player' como parâmetro
    cleanupPlayerModel(&player);
    for (int i = 0; i < objectCount; ++i) {
        cleanupObject(&sceneObjects[i]);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("HALLO");

    init();

    glutIdleFunc(idleUpdates);
    glutKeyboardFunc(handleKeyboardInput);
    glutKeyboardUpFunc(keyboardKeyUp);
    glutPassiveMotionFunc(handleMouseMovement);
    glutCloseFunc(handleCloseProgram);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
