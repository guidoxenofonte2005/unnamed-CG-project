#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <GL/freeglut.h>

#include <math.h>
#include <stdbool.h>

#include "player.h"
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

Player player = {0.0f, 0.0f, 0.0f, true, true, IDLE};
PlayerMoveKeys moveKeys = {false, false, false, false};

float playerVelocity[] = {0.0f, 0.0f, 0.0f};

float deltaTime = 0.0f;

int init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade para a remoção de superfícies ocultas
    glEnable(GL_LIGHTING); // Habilita o sistema de iluminação global do OpenGL.

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, 4.0f/3.0f, 0.1, 100.0); // Configura uma projeção perspectiva, que simula a visão humana (objetos distantes parecem menores).

    // Chamada para carregar o modelo 3D uma única vez durante a inicialização.
    loadPlayerModel("3dfiles/player.glb");
    return 1;
}

void display() {
    // Limpa o buffer de cor e o de profundidade em cada frame.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = camRadius * cosf(phiAngle) * cosf(thetaAngle);
    float camY = camRadius * sinf(phiAngle);
    float camZ = camRadius * cosf(phiAngle) * sinf(thetaAngle);

    // Define a orientação da câmera
    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // glPushMatrix();
    //     glTranslatef(player.x, player.y, player.z);
    //     glColor3f(1.0f, 0.0f, 0.0f);
    //     glutWireCube(1.0f);
    // glPopMatrix();

    // chama função para desenhar o modelo 3D na tela a cada frane
    drawPlayerModel(player, playerRotation);
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
    // A função agora chama a função de limpeza do modelo
    cleanupPlayerModel();
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
