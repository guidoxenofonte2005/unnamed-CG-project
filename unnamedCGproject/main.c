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

float fieldOfView = 60.0f;

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

Player player = {0.0f, 0.0f, 0.0f, false, true, IDLE, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, NULL};
PlayerMoveKeys moveKeys = {false, false, false, false, false, false};

float playerVelocity[] = {0.0f, 0.0f, 0.0f};

float deltaTime = 0.0f;

#define MAX_OBJECTS 10 // Define o máximo de objetos na cena
SceneObject sceneObjects[MAX_OBJECTS];
int objectCount = 0;

SceneObject objectsInCollisionRange[MAX_OBJECTS];
int objInColRangeCount = 0;

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
    gluPerspective(fieldOfView, (float)winWidth / (float)winHeight, 0.1, 100.0); // Ajustado para usar as variáveis de janela

    //1. Define a posição inicial do jogador (à esquerda)
    //player.x = 0.0f;
    //player.y = 1.0f;
    //player.z = 0.0f;

    // Chamada para carregar o modelo 3D uma única vez durante a inicialização.
    //loadPlayerModel(&player, "3dfiles/player.glb");

    // Carregando outros objetos da cena e add em um array
    // loadObject(&sceneObjects[objectCount++], "3dfiles/grass1.glb", 0.0f, -1.0f, 0.0f); // Exemplo de um chão
    // Carrega o espinho e CONFIGURA A SUA ANIMAÇÃO
    loadObject(&sceneObjects[objectCount++], "3dfiles/spike.glb", 0.0f, 1.0f, -25.0f);
    sceneObjects[objectCount - 1].anim.isAnimated = true;
    sceneObjects[objectCount - 1].anim.animationAxis = 0; // Animação no Eixo X
    sceneObjects[objectCount - 1].anim.moveSpeed = 5.0f;
    sceneObjects[objectCount - 1].anim.moveDirection = 1.0f;
    sceneObjects[objectCount - 1].anim.minLimit = -10.0f;
    sceneObjects[objectCount - 1].anim.maxLimit = 10.0f;
    //objectCount++; // Incrementa o contador DEPOIS de configurar

    // loadObject(&sceneObjects[objectCount++], "3dfiles/hydrant.glb", 15.0f, 0.0f, -10.0f); // Exemplo de um objeto na posição (15,0,0)

    // depois seria bom colocar todos os loadObjects e o loadPlatforms em uma função só que gere o cenário inteiro de uma vez
    // talvez precisaria fzr um sistema q leia um arquivo pra pegar as informações do cenário
    // fica pra ver depois ent

    //CollisionBox platCol;
    //float platformWidth = 4.0f;
    //float platformHeight = 2.0f;
    //float platformDepth = 5.0f;
    //float centerX = 0.0f;
    //float centerY = -1.0f;
    //float centerZ = 0.0f;

    //platCol.minX = centerX - platformWidth / 2;
    //platCol.maxX = centerX + platformWidth / 2;
    //platCol.minY = centerY - platformHeight / 2;
    //platCol.maxY = centerY + platformHeight / 2;
    //platCol.minZ = centerZ - platformDepth / 2;
    //platCol.maxZ = centerZ + platformDepth / 2;
    // printf("%f, %f, %f - %f, %f, %f\n", platCol.minX, platCol.minY, platCol.minZ, platCol.maxX, platCol.maxY, platCol.maxZ);

    //loadPlatform(sceneObjects, &objectCount, centerX, centerY, centerZ, &platCol);
    // Animação da plataforma
    //loadPlatform(sceneObjects, &objectCount, centerX, centerY, centerZ, &platCol);
    //sceneObjects[objectCount - 1].anim.isAnimated = true;
    //sceneObjects[objectCount - 1].anim.animationAxis = 1; // Animação no Eixo Z
    //sceneObjects[objectCount - 1].anim.moveSpeed = 3.0f;
    //sceneObjects[objectCount - 1].anim.moveDirection = 1.0f;
    //sceneObjects[objectCount - 1].anim.minLimit = -15.0f; // Limite frontal
    //sceneObjects[objectCount - 1].anim.maxLimit = 15.0f;  // Limite traseiro

    loadObjectsFromFile("scenario.txt", sceneObjects, &player, &objectCount);

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
        drawCollisionBoxWireframe(sceneObjects[i].collision);
    }
    drawCollisionBoxWireframe(player.collision);

    glutPostRedisplay();

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
    if (pressedKey == 32) {
        moveKeys.jump = true;
        // printf("%d, %d\n", player.isOnGround, player.canJump);
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
    if (key == 32) {
        moveKeys.jump = false;
    }
}


void simulatePhysics(float deltaTime) {
    // serve para automatizar o movimento de todos os objetos que você marcou como "animados" na sua cena
    for (int i = 0; i < objectCount; i++) {
        animateObject(&sceneObjects[i], deltaTime);
    }

    getPlayerVelocity(playerVelocity, &moveKeys, phiAngle, thetaAngle, deltaTime, &player.isOnGround);
    getObjectsInCollisionRange(player, sceneObjects, MAX_OBJECTS, objectsInCollisionRange, &objInColRangeCount);
    collideAndSlide(playerVelocity, &player, objectsInCollisionRange, objInColRangeCount, deltaTime);
    getPlayerMovingAngle(playerVelocity, &playerRotation);
}

void idleUpdates() {
    static float accumulator = 0.0f;
    float frameTime = getDeltaTime();
    accumulator += frameTime;

    // se muitos frames acumulados, processa vários physics steps
    while (accumulator >= PHYSICS_STEP) {
        simulatePhysics(PHYSICS_STEP);
        accumulator -= PHYSICS_STEP;
    }

    // "redesenha" a tela
    glutPostRedisplay();
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
    // glutPostRedisplay();
}

void handleMouseWheel(int wheel, int direction, int x, int y) {
    if (direction > 0) {
        fieldOfView -= 2.0f;
        fieldOfView = max(fieldOfView, 30);
        updateFOV(fieldOfView, (float)winWidth, (float)winHeight);
    } else {
        fieldOfView += 2.0f;
        fieldOfView = min(fieldOfView, 80);
        updateFOV(fieldOfView, (float)winWidth, (float)winHeight);
    }
}

void handleCloseProgram() {
    // A função agora chama a função de limpeza do modelo do jogador é chamada
    // passando a struct 'player' como parâmetro
    cleanupPlayerModel(&player);
    for (int i = 0; i < objectCount; ++i) {
        cleanupObject(&sceneObjects[i]);
    }
}

void handleWindowResize(int newWidth, int newHeight) {
    // define os tamanhos máximo e mínimo da tela
    if (newWidth < MIN_SCREEN_WIDTH) newWidth = MIN_SCREEN_WIDTH;
    if (newHeight < MIN_SCREEN_HEIGHT) newHeight = MIN_SCREEN_HEIGHT;

    if (newWidth > MAX_SCREEN_WIDTH) newWidth = MAX_SCREEN_WIDTH;
    if (newHeight > MAX_SCREEN_HEIGHT) newHeight = MAX_SCREEN_HEIGHT;

    // atualiza a variável global das dimensões da tela
    winWidth = min(max(newWidth, 1000), min(newWidth, 1920));
    winHeight = min(max(newHeight, 750), min(newHeight, 1080));

    // volta a tela para o padrão mínimo/máximo caso passe do limite, senão continua normal
    glutReshapeWindow(winWidth, winHeight);

    // atualiza o viewport
    glViewport(0, 0, winWidth, winHeight);

    // muda a projeção de perspectiva pra acomodar o novo tamanho da tela
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, (float) winWidth / (float) winHeight, 0.1f, 100.0f);

    // volta para o modelview
    glMatrixMode(GL_MODELVIEW);

    // dá redisplay na tela por segurança
    glutPostRedisplay();
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
    glutMouseWheelFunc(handleMouseWheel);

    glutCloseFunc(handleCloseProgram);

    glutReshapeFunc(handleWindowResize);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
