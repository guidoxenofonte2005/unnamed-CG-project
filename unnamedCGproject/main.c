#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <GL/freeglut.h>

#include <math.h>

int verticalMovement;
int horizontalMovement;

int lastMousex, lastMousey;

float thetaAngle = 0.0f;  // ângulo horizontal
float phiAngle = 0.0f;  // ângulo vertical
float camRadius = 2.0f; // distância da câmera ao alvo


int init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 100.0); // fov 60°, aspecto 1:1, near=0.1, far=100
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = camRadius * cosf(phiAngle) * cosf(thetaAngle);
    float camY = camRadius * sinf(phiAngle);
    float camZ = camRadius * cosf(phiAngle) * sinf(thetaAngle);

    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glColor3f(1.0f, 0.0f, 0.0f);
    glutWireCube(1.0f);

    glutSwapBuffers();
}

void handleKeyboardInput() {

}

void handleMouseMovement(int x, int y) {
    int dx = x - lastMousex;
    int dy = y - lastMousey;

    float mouseSensitivity = 0.005f;

    thetaAngle += dx * mouseSensitivity;
    phiAngle -= dy * mouseSensitivity;

    if (phiAngle > 1.55f)  phiAngle = 1.55f;   // ~89°
    if (phiAngle < -1.55f) phiAngle = -1.55f;  // ~-89°

    lastMousex = x;
    lastMousey = y;

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
