// Deus nos dê mt paciência nesse projeto

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>
#include "player.h"
#include "object.h"
#include "utils.h"
#include "texture.h"
#include "skybox.h"

#define DEATH_Y_LEVEL -15.0f // Define a altura em que o jogador volta para o ponto inicial
#define MAX_OBJECTS 50 // Define o máximo de objetos na cena

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

Player player = {
    0.0f, 0.0f, 0.0f,     // x, y, z
    false,                 // isOnGround
    true,                  // canJump
    false,                 // isRespawning
    false,                 // isJumping
    IDLE,                  // state
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // collision
    NULL                   // modelData
};

PlayerMoveKeys moveKeys = {false, false, false, false, false, false};

float checkpointX, checkpointY, checkpointZ;
float playerVelocity[] = {0.0f, 0.0f, 0.0f};
float deltaTime = 0.0f;

SceneObject sceneObjects[MAX_OBJECTS];
int objectCount = 0;

// índice de início das plataformas no array sceneObjects
int platformStartIndex = 0;
SceneObject objectsInCollisionRange[MAX_OBJECTS];
int objInColRangeCount = 0;

// Array que guarda as plataformas
PlatformData levelPlatforms[] = {
    // 1. O Chão (Plataforma Inicial)
    // A superfície superior (onde o jogador pisa) está em Y = 1.0.
    { .centerX = 0.0f, .centerY = -1.0f, .centerZ = 0.0f, .width = 30.0f, .height = 4.0f, .depth = 30.0f },

    // 2. A Parede Traseira (no lado Z negativo)
    { .centerX = 0.0f, .centerY = 11.0f, .centerZ = -16.0f, .width = 32.0f, .height = 20.0f, .depth = 2.0f },

    // 3. A Parede Esquerda (no lado X negativo)
    { .centerX = -16.0f, .centerY = 11.0f, .centerZ = 0.0f, .width = 2.0f, .height = 20.0f, .depth = 30.0f },

    // 4. A Parede Direita (no lado X positivo)
    // Igual à parede esquerda, mas no lado oposto.
    { .centerX = 16.0f, .centerY = 11.0f, .centerZ = 0.0f, .width = 2.0f, .height = 20.0f, .depth = 30.0f },

    // 5. O Teto
    { .centerX = 0.0f, .centerY = 22.0f, .centerZ = 0.0f, .width = 32.0f, .height = 2.0f, .depth = 32.0f },

    // 6. Nova Plataforma Pequena no meio do vão
    { .centerX = 0.0f, .centerY = -1.0f, .centerZ = 30.0f, .width = 8.0f, .height = 4.0f, .depth = 8.0f },

    // 7. plataforma (mais à frente e à direita)
    { .centerX = 0.0f, .centerY = -1.0f, .centerZ = 55.0f, .width = 15.0f, .height = 4.0f, .depth = 15.0f },

    // 8. plataforma (mais à frente e à direita)
    { .centerX = 0.0f, .centerY = -1.0f, .centerZ = 85.0f, .width = 15.0f, .height = 4.0f, .depth = 15.0f },

    // 9. plataforma (diagonal da plataforma 8)
    { .centerX = 10.0f, .centerY = 10.0f, .centerZ = 115.0f, .width = 15.0f, .height = 4.0f, .depth = 15.0f },

    // 10. plataforma (diagonal da plataforma 8)
    { .centerX = -10.0f, .centerY = 30.0f, .centerZ = 115.0f, .width = 15.0f, .height = 4.0f, .depth = 15.0f },
};
int numPlatforms = sizeof(levelPlatforms) / sizeof(PlatformData);

GLuint texFront, texBack, texLeft, texRight, texTop, texBase;

int init() {
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//forçar a textura a substituir a cor/material

    // Carrega cada textura da plataforma
    texFront = loadTexture("tex_cenario/girder_wood.png");
    texBack = loadTexture("tex_cenario/wall_ruined_1.png");
    texLeft = loadTexture("tex_cenario/wall_ruined_2.png");
    texRight = loadTexture("tex_cenario/wall_ruined_3.png");
    texTop = loadTexture("tex_cenario/wood_3.png");
    texBase = loadTexture("tex_cenario/metal_floor_1.png");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Configura uma proje��o perspectiva, que simula a vis�o humana (objetos distantes parecem menores).
    gluPerspective(fieldOfView, (float)winWidth / (float)winHeight, 0.1, 1000.0); // Ajustado para usar as vari�veis de janela

    loadSkybox();


    //1. Define a posi��o inicial do jogador (� esquerda)
    //player.x = 0.0f;
    //player.y = 2.0f;
    //player.z = 0.0f;

    checkpointX = player.x;
    checkpointY = 2.0f;
    checkpointZ = player.z;

    // daqui pra baixo tem que substituir pelo load com arquivo
    //loadPlayerModel(&player, "3dfiles/player.glb");

    loadObjectsFromFile("scenario.txt", sceneObjects, &player, &objectCount, MAX_OBJECTS);

    // adiciona ESPINHOS
    for (int i = 0; i < 15; i++) {
        // Garante que não vamos ultrapassar o limite de objetos
        // desnecessário agora que não carrega mais
        // if (objectCount >= MAX_OBJECTS) break;
        // Carrega um espinho na posição correta
        // agora está no fileManager
        // loadObject(&sceneObjects[objectCount+numPlatforms], "3dfiles/spike.glb", spikePositions[i][0], spikeBaseLevelY, spikePositions[i][1]);

        // Gira o espinho para ficar na vertical
        sceneObjects[objectCount-i].rotationAngle = -90.0f;
        sceneObjects[objectCount-i].rotX = 1.0f;

        // O vão 3 começa no índice 8 do array spikePositions (0-indexed)
        if (i <= 6) {
            SceneObject* currentSpike = &sceneObjects[objectCount-i];
            currentSpike->anim.isAnimated = true;
            currentSpike->anim.animationAxis = 2; // Animação no Eixo Y
            currentSpike->anim.moveSpeed = 5.0f;
            currentSpike->anim.moveDirection = 1.0f;
            // Define os limites do movimento vertical
            currentSpike->anim.minLimit = currentSpike->y; // Ponto mais baixo
            currentSpike->anim.maxLimit = currentSpike->y + 7.0f; // Ponto mais alto
        }
        //objectCount++; // Incrementa o contador de objetos
    }

    // agora adiciona plataformas
    //loadLevelPlatforms();

    // Pra animar alguma plataforma, faz o msm que fiz abaixo (verifique o índice em PlatformData levelPlatforms[])
    // Índice da plataforma 9 é (0-indexed, então é 8)
    int ninePlatformIndex = platformStartIndex + 8;
    // Garante que não vamos acessar um índice fora do array
    if (ninePlatformIndex < objectCount) {
        // Ativa a animação
        sceneObjects[ninePlatformIndex].anim.isAnimated = true;
        // Define os parâmetros da animação
        sceneObjects[ninePlatformIndex].anim.animationAxis = 0; // 0 para Eixo X, 1 para Eixo Z
        sceneObjects[ninePlatformIndex].anim.moveSpeed = 8.0f;     // Velocidade do movimento
        sceneObjects[ninePlatformIndex].anim.moveDirection = 1.0f; // Começa se movendo na direção positiva

        // A plataforma começará em x=0.0 e se moverá entre -20.0 e 20.0
        sceneObjects[ninePlatformIndex].anim.minLimit = -20.0f;
    }


    return 1;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = player.x + camRadius * cosf(phiAngle) * cosf(thetaAngle);
    float camY = player.y + camRadius * sinf(phiAngle);
    float camZ = player.z + camRadius * cosf(phiAngle) * sinf(thetaAngle);

    // Define a orienta��o da c�mera
    gluLookAt(camX, camY, camZ,
              player.x, player.y + 4, player.z,
              0.0, 1.0, 0.0);


    glPushMatrix();
    // skybox na posição da câmera (sem se mover com o jogador)
    glTranslatef(player.x, player.y, player.z);
    drawSkybox(50.0f);
    glPopMatrix();


    // Definindo as propriedades da fonte de luz
    GLfloat ambientLight[]  = {0.2f, 0.2f, 0.2f, 1.0f};  // Luz ambiente fraca
    GLfloat diffuseLight[]  = {0.8f, 0.8f, 0.8f, 1.0f};  // Luz difusa branca
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Brilho especular branco
    GLfloat lightPosition[] = {10.0f, 10.0f, 10.0f, 1.0f}; // Posi��o da luz

    // Define as propriedades do material (pode ser gen�rico para todos os objetos)
    GLfloat ambientMaterial[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat diffuseMaterial[]  = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat specularMaterial[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat shininess = 20;

    // chama a fun��o para aplicar ilumina��o
    setupLighting(ambientLight, diffuseLight, specularLight, lightPosition, ambientMaterial, diffuseMaterial, specularMaterial, shininess);

    glBindTexture(GL_TEXTURE_2D, 0); // desliga a textura atual

    // chama fun��o para desenhar o modelo 3D na tela a cada frane
    drawPlayerModel(&player, playerRotation);
    for (int i = 0; i < objectCount; ++i) {
        drawObject(&sceneObjects[i]);
        drawCollisionBoxWireframe(sceneObjects[i].collision);
    }
    drawCollisionBoxWireframe(player.collision);

    glutPostRedisplay();
    glutSwapBuffers();
}

void handleKeyboardInput(unsigned char pressedKey, int x, int y) {
    if (pressedKey == 27) {
        isCameraActive = !isCameraActive;
        if (isCameraActive) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(winWidth / 2, winHeight / 2);
        } else {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    }
    if (pressedKey == 119) moveKeys.w = true;
    if (pressedKey == 97) moveKeys.a = true;
    if (pressedKey == 115) moveKeys.s = true;
    if (pressedKey == 100) moveKeys.d = true;
    if (pressedKey == 32) moveKeys.jump = true;
}

void keyboardKeyUp(unsigned char key, int x, int y) {
    if (key == 119) moveKeys.w = false;
    if (key == 97) moveKeys.a = false;
    if (key == 115) moveKeys.s = false;
    if (key == 100) moveKeys.d = false;
    if (key == 32) moveKeys.jump = false;
}

void simulatePhysics(float deltaTime) {
    // 1. Anima objetos da cena
    for (int i = 0; i < objectCount; i++) {
        animateObject(&sceneObjects[i], deltaTime);
    }

    // 2. Guarda estado anterior e reseta o estado "no chão"
    bool wasOnGround = player.isOnGround;
    player.isOnGround = false; // recalculado na colisão

    // 3. Calcula a velocidade pretendida do jogador (movimento + gravidade)
    getPlayerVelocity(playerVelocity, &moveKeys, phiAngle, thetaAngle, deltaTime, &wasOnGround); // Usamos wasOnGround para o pulo

    // 4. Limita a velocidade de queda para evitar "tunneling"
    float maxFallSpeed = -50.0f;
    if (playerVelocity[1] < maxFallSpeed) {
        playerVelocity[1] = maxFallSpeed;
    }

    // 5. Detecta objetos próximos para otimizar a colisão
    getObjectsInCollisionRange(player, sceneObjects, MAX_OBJECTS, objectsInCollisionRange, &objInColRangeCount);

    // 6. Verifica colisões com objetos perigosos ANTES do movimento
    for (int i = 0; i < objInColRangeCount; i++) {
        if (objectsInCollisionRange[i].type == DANGER &&
            isObjectColliding(player.collision, objectsInCollisionRange[i].collision)) {
            printf("Colidiu com um objeto perigoso! Morreu!\n");
            respawnPlayer();
            return; // Sai da física neste frame
        }
    }

    // 7. Processa movimento e colisões com plataformas/paredes
    collideAndSlide(playerVelocity, &player, objectsInCollisionRange, objInColRangeCount, deltaTime);
    getPlayerMovingAngle(playerVelocity, &playerRotation);

    // 8. Verifica morte por queda
    if (player.y < DEATH_Y_LEVEL) {
        respawnPlayer();
    }
}

void idleUpdates() {
    static float accumulator = 0.0f;
    float frameTime = getDeltaTime();
    accumulator += frameTime;
    while (accumulator >= PHYSICS_STEP) {
        simulatePhysics(PHYSICS_STEP);
        accumulator -= PHYSICS_STEP;
    }
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
    cleanupPlayerModel(&player);
    for (int i = 0; i < objectCount; ++i) {
        cleanupObject(&sceneObjects[i]);
    }
}

void handleWindowResize(int newWidth, int newHeight) {
    // define os tamanhos m�ximo e m�nimo da tela
    if (newWidth < MIN_SCREEN_WIDTH) newWidth = MIN_SCREEN_WIDTH;
    if (newHeight < MIN_SCREEN_HEIGHT) newHeight = MIN_SCREEN_HEIGHT;
    if (newWidth > MAX_SCREEN_WIDTH) newWidth = MAX_SCREEN_WIDTH;
    if (newHeight > MAX_SCREEN_HEIGHT) newHeight = MAX_SCREEN_HEIGHT;

    // atualiza a vari�vel global das dimens�es da tela
    winWidth = min(max(newWidth, 1000), min(newWidth, 1920));
    winHeight = min(max(newHeight, 750), min(newHeight, 1080));

    // volta a tela para o padr�o m�nimo/m�ximo caso passe do limite, sen�o continua normal
    glutReshapeWindow(winWidth, winHeight);

    glViewport(0, 0, winWidth, winHeight);

    // muda a proje��o de perspectiva pra acomodar o novo tamanho da tela
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, (float) winWidth / (float) winHeight, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);

    // d� redisplay na tela por seguran�a
    glutPostRedisplay();
}

// atualizado para o fileManager
// Função para carregar as plataformas
void loadLevelPlatforms() {
    platformStartIndex = objectCount; // guarda o índice inicial das plataformas
    for (int i = 0; i < numPlatforms; i++) {
        CollisionBox platCol;
        PlatformData currentPlatform = levelPlatforms[i];
        platCol.minX = currentPlatform.centerX - currentPlatform.width / 2;
        platCol.maxX = currentPlatform.centerX + currentPlatform.width / 2;
        platCol.minY = currentPlatform.centerY - currentPlatform.height / 2;
        platCol.maxY = currentPlatform.centerY + currentPlatform.height / 2;
        platCol.minZ = currentPlatform.centerZ - currentPlatform.depth / 2;
        platCol.maxZ = currentPlatform.centerZ + currentPlatform.depth / 2;
        loadPlatform(sceneObjects, &objectCount, currentPlatform.centerX, currentPlatform.centerY, currentPlatform.centerZ, &platCol);
    }
}

// Função para respawnar o player caso ele morra
void respawnPlayer() {
    printf("GAME OVER! Voltando para o checkpoint.\n");
    // 1. Reposiciona o player acima da plataforma inicial
    player.x = checkpointX;
    player.y = checkpointY + 0.1f; // Adiciona pequeno offset para garantir que está acima
    player.z = checkpointZ;

    // 2. CRÍTICO: Zera COMPLETAMENTE as velocidades
    playerVelocity[0] = 0.0f;
    player.y = checkpointY + 0.5f; // Offset para garantir que ele caia sobre a plataforma
    playerVelocity[2] = 0.0f;

    // 3. Atualiza a caixa de colisão na nova posição
    updatePlayerCollisionBox(&player);

    // 4. Define flags de estado corretas
    player.isRespawning = false;
    player.isOnGround = false;  // Força a física a detectar o chão novamente
    player.isJumping = true;
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
