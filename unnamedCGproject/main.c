#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <GL/freeglut.h>

#include <math.h>
#include <stdbool.h>

#include "player.h"
#include "utils.h"

#define CGLTF_IMPLEMENTATION
#include "libs/cgltf/cgltf.h"

int verticalMovement;
int horizontalMovement;

int lastMousex, lastMousey;

float thetaAngle = 0.0f;  // ângulo horizontal
float phiAngle = 0.0f;  // ângulo vertical
float camRadius = 25.0f; // distância da câmera ao alvo

bool isCameraActive = false;
int winWidth = 1000, winHeight = 750;

Player player = {0.0f, 0.0f, 0.0f, true, true, IDLE};
PlayerMoveKeys moveKeys = {false, false, false, false};

float playerVelocity[] = {0.0f, 0.0f, 0.0f};

int init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 4.0f/3.0f, 0.1, 100.0); // fov 60°, aspecto 4:3, near=0.1, far=100
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

    glPushMatrix();
        glTranslatef(player.x, player.y, player.z);
        glColor3f(1.0f, 0.0f, 0.0f);
        glutWireCube(1.0f);
    glPopMatrix();

    glutSwapBuffers();
}

void handleKeyboardInput(unsigned char pressedKey, int x, int y) {
    if (pressedKey == 27) { // ESC
        isCameraActive = !isCameraActive;

        if (isCameraActive) {
            glutSetCursor(GLUT_CURSOR_NONE); // esconde cursor
            glutWarpPointer(500 / 2, 500 / 2);
        } else {
            glutSetCursor(GLUT_CURSOR_INHERIT); // mostra cursor
        }
    }
    else {
        if (pressedKey == 119) { // w
            moveKeys.w = true;
        }
        if (pressedKey == 97) { // a
            moveKeys.a = true;
        }
        if (pressedKey == 115) { // s
            moveKeys.s = true;
        }
        if (pressedKey == 100) { // d
            moveKeys.d = true;
        }
        if (pressedKey == 99) { // c - used for debug/testing purposes
            testGLTFLoad();
        }
    }
    // printf("%d", pressedKey);
}

void testGLTFLoad() {
    cgltf_data* data = NULL;
    cgltf_options opts = {0};

    cgltf_result result = cgltf_parse_file(&opts, "3dfiles/scene.gltf", &data);
    if (result == cgltf_result_success) {
        printf("Arquivo carregado!\n");
        cgltf_free(data);
    } else {
        printf("Erro ao carregar o GLTF\n");
    }
}

void keyboardKeyUp(unsigned char key, int x, int y) {
    if (key == 119) { // w
        moveKeys.w = false;
    }
    if (key == 97) { // a
        moveKeys.a = false;
    }
    if (key == 115) { // s
        moveKeys.s = false;
    }
    if (key == 100) { // d
        moveKeys.d = false;
    }
}

void idleUpdates() {
    getPlayerVelocity(playerVelocity, &moveKeys, phiAngle, thetaAngle);
    //printf("%f, %f, %f\n", playerVelocity[0], playerVelocity[1], playerVelocity[2]);
    movePlayer(playerVelocity, &player);
}


void handleMouseMovement(int x, int y) {
    if (!isCameraActive) return;

    int centerX = winWidth / 2;
    int centerY = winHeight / 2;

    int dx = x - centerX;
    int dy = y - centerY;

    float mouseSensitivity = 0.005f; // sensibilidade do mouse

    thetaAngle += dx * mouseSensitivity; // ângulo horizontal
    phiAngle += dy * mouseSensitivity;   // ângulo vertical

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
    glutInitWindowPosition(200, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("HALLO");

    init();

    glutIdleFunc(idleUpdates);
    glutKeyboardFunc(handleKeyboardInput);
    glutKeyboardUpFunc(keyboardKeyUp);
    glutPassiveMotionFunc(handleMouseMovement); // função que detecta movimento do mouse de forma passiva (sem nenhum botão pressionado)

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
