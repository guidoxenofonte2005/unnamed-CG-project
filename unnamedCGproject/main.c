#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <GL/freeglut.h>

#include <math.h>
#include <stdbool.h>

#include "player.h"

int verticalMovement;
int horizontalMovement;

int lastMousex, lastMousey;

float thetaAngle = 0.0f;  // ângulo horizontal
float phiAngle = 0.0f;  // ângulo vertical
float camRadius = 5.0f; // distância da câmera ao alvo

bool isCameraActive = false;
int winWidth = 500, winHeight = 500;


int init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 100.0); // fov 60°, aspecto 1:1, near=0.1, far=100
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // cálculo das novas posições da câmera
    float camX = camRadius * cosf(phiAngle) * cosf(thetaAngle);
    float camY = camRadius * sinf(phiAngle);
    float camZ = camRadius * cosf(phiAngle) * sinf(thetaAngle);

    gluLookAt(camX, camY, camZ, // posição da câmera (x,y,z)
              0.0, 0.0, 0.0,    // ponto que a câmera está olhando (0, 0, 0) (mudar para posição do player depois)
              0.0, 1.0, 0.0);   // vetor upwards (0, 1, 0)

    glColor3f(1.0f, 0.0f, 0.0f);
    glutWireCube(1.0f);

    glutSwapBuffers();
}

void handleKeyboardInput(unsigned char pressedKey, int x, int y) {
    if (pressedKey == 27) { // ESC
        isCameraActive = !isCameraActive;
        coisa();

        if (isCameraActive) {
            glutSetCursor(GLUT_CURSOR_NONE); // esconde cursor
            glutWarpPointer(500 / 2, 500 / 2);
        } else {
            glutSetCursor(GLUT_CURSOR_INHERIT); // mostra cursor
        }
    }

}

void handleMouseMovement(int x, int y) {
    if (!isCameraActive) return;

    int centerX = winWidth / 2;
    int centerY = winHeight / 2;

    int dx = x - centerX;
    int dy = y - centerY;

    float mouseSensitivity = 0.005f; // sensibilidade do mouse

    thetaAngle += dx * mouseSensitivity;
    phiAngle += dy * mouseSensitivity;

    if (phiAngle > 1.55f)  phiAngle = 1.55f;  // limite vertical = 89°
    if (phiAngle < -1.55f) phiAngle = -1.55f; // limite vertical = -89°

    glutWarpPointer(centerX, centerY); // não permite que o mouse saia da tela enquanto a detecção de câmera estiver ativa (basicamente só volta pro centro)

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // mudar diretivas de inicialização depois pra algo melhor
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("HALLO");

    init();

    glutKeyboardFunc(handleKeyboardInput);
    glutPassiveMotionFunc(handleMouseMovement); // função que detecta movimento do mouse de forma passiva (sem nenhum botão pressionado)

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
