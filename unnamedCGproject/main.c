#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <GL/freeglut.h>

int verticalMovement;
int horizontalMovement;

int lastMousex, lastMousey;


int init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.8, 0.5, 0.5,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

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

    float thetaAngle = dx * mouseSensitivity;

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
