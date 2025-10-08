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

float fieldOfView = 60.0f;
int lastMousex, lastMousey;
// Estado do jogo
bool gameCompleted = false;

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

Player player = {0.0f, 0.0f, 0.0f, // x, y, z
                 false, // isOnGround
                 IDLE, // state
                 {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // collisionBox
                 NULL, // modelData
                 0}; // groundObjectIndex

//                           W      A      S      D     JUMP
PlayerMoveKeys moveKeys = {false, false, false, false, false};

float checkpointX, checkpointY, checkpointZ;
float playerVelocity[] = {0.0f, 0.0f, 0.0f};
float deltaTime = 0.0f;

SceneObject sceneObjects[MAX_OBJECTS];
int objectCount = 0;

// índice de início das plataformas no array sceneObjects
SceneObject objectsInCollisionRange[MAX_OBJECTS];
int objInColRangeCount = 0;

//GLuint texFront, texBack, texLeft, texRight, texTop, texBase;

int init() {
    glClearColor(0.32f, 0.23f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//forçar a textura a substituir a cor/material

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Configura uma proje��o perspectiva, que simula a vis�o humana (objetos distantes parecem menores).
    gluPerspective(fieldOfView, (float)winWidth / (float)winHeight, 0.1, 1000.0); // Ajustado para usar as vari�veis de janela

    loadSkybox();

    checkpointX = player.x;
    checkpointY = 2.0f;
    checkpointZ = player.z;

    loadObjectsFromFile("scenario.txt", sceneObjects, &player, &objectCount, MAX_OBJECTS);

    // adiciona ESPINHOS
    for (int i = 0; i < 15; i++) {

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
    }

    // ADICIONA ANIMAÇÕES
    for (int i = 0; i < objectCount; i++) {
        SceneObject* currentObject = &sceneObjects[i];

        // Animação para a Plataforma 9 (que se move para os lados)
        // Posição inicial dela: X=10, Y=10, Z=115
        if (currentObject->x == 10.0f && currentObject->y == 10.0f && currentObject->z == 115.0f) {
            currentObject->anim.isAnimated = true;
            currentObject->anim.animationAxis = 0; // 0 para Eixo X
            currentObject->anim.moveSpeed = 8.0f;
            currentObject->anim.moveDirection = 1.0f;
            currentObject->anim.minLimit = -10.0f; // Moverá de -10 a 10 no eixo X
            currentObject->anim.maxLimit = 10.0f;
        }

        // Animação para a Plataforma 11 (que sobe e desce)
        // Posição inicial dela: X=-10, Y=50, Z=140
        if (currentObject->x == -10.0f && currentObject->y == 50.0f && currentObject->z == 140.0f) {
            currentObject->anim.isAnimated = true;
            currentObject->anim.animationAxis = 2; // 2 para Eixo Y
            currentObject->anim.moveSpeed = 4.0f;
            currentObject->anim.moveDirection = 1.0f;
            currentObject->anim.minLimit = 40.0f; // Ponto mais baixo
            currentObject->anim.maxLimit = 60.0f; // Ponto mais alto
        }

        // Animação para a Plataforma 12 (que se move para os lados)
        // Posição inicial dela: X=-10, Y=50, Z=165
        if (currentObject->x == -10.0f && currentObject->y == 50.0f && currentObject->z == 165.0f) {
            currentObject->anim.isAnimated = true;
            currentObject->anim.animationAxis = 0; // 0 para Eixo X
            currentObject->anim.moveSpeed = 5.0f;
            currentObject->anim.moveDirection = 1.0f;
            currentObject->anim.minLimit = -20.0f; // Moverá de -10 a 10 no eixo X
            currentObject->anim.maxLimit = 20.0f;
        }

        // Animação para a Plataforma 13 (que se move para os lados)
        // Posição inicial dela: X=-10, Y=50, Z=190
        if (currentObject->x == -10.0f && currentObject->y == 50.0f && currentObject->z == 190.0f) {
            currentObject->anim.isAnimated = true;
            currentObject->anim.animationAxis = 0; // 0 para Eixo X
            currentObject->anim.moveSpeed = 8.0f;
            currentObject->anim.moveDirection = 1.0f;
            currentObject->anim.minLimit = -10.0f; // Moverá de -10 a 10 no eixo X
            currentObject->anim.maxLimit = 10.0f;
        }

        // Animação para a Plataforma 14 (que se move para os lados)
        // Posição inicial dela: X=-10, Y=50, Z=215
        if (currentObject->x == -10.0f && currentObject->y == 50.0f && currentObject->z == 215.0f) {
            currentObject->anim.isAnimated = true;
            currentObject->anim.animationAxis = 0; // 0 para Eixo X
            currentObject->anim.moveSpeed = 6.0f;
            currentObject->anim.moveDirection = 1.0f;
            currentObject->anim.minLimit = -15.0f; // Moverá de -10 a 10 no eixo X
            currentObject->anim.maxLimit = 15.0f;
        }

        // Animação para a Plataforma 15 (que se move para os lados)
        // Posição inicial dela: X=-10, Y=50, Z=240
        if (currentObject->x == -10.0f && currentObject->y == 50.0f && currentObject->z == 240.0f) {
            currentObject->anim.isAnimated = true;
            currentObject->anim.animationAxis = 0; // 0 para Eixo X
            currentObject->anim.moveSpeed = 7.0f;
            currentObject->anim.moveDirection = 1.0f;
            currentObject->anim.minLimit = -10.0f; // Moverá de -10 a 10 no eixo X
            currentObject->anim.maxLimit = 10.0f;
        }
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

    // Define a orientacao da camera
    gluLookAt(camX, camY, camZ,
              player.x, player.y + 4, player.z,
              0.0, 1.0, 0.0);

    glEnable(GL_LIGHTING); // Habilita o sistema de iluminação
    glEnable(GL_LIGHT0); // Ativa a luz 0

    glPushMatrix();
        // Salva os atributos de enable e do buffer de profundidade
        glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_LIGHTING); // Skybox não precisa de luz
        glDisable(GL_CULL_FACE); // Garante que o interior da caixa seja desenhado
        glDepthMask(GL_FALSE); // Desliga a profundidade

        // Translada a skybox para a posição da câmera para criar a ilusão de um fundo infinito
        glTranslatef(camX, camY, camZ);
        drawSkybox(500.0f);

        glPopAttrib(); // Restaura os atributos
    glPopMatrix();

    // Definindo as propriedades da fonte de luz
    GLfloat ambientLight[] = {0.1f, 0.1f, 0.2f, 1.0f}; // ambiente azulado fraco
    GLfloat diffuseLight[] = {0.4f, 0.4f, 0.8f, 1.0f}; // luz difusa azul-claro
    GLfloat specularLight[] = {0.6f, 0.6f, 1.0f, 1.0f}; // brilho frio
    GLfloat lightPosition[] = {0.0f, 400.0f, 200.0f, 1.0f}; // vindo de cima, tipo lua

    // DEBUG - CHECAR POSIÇÃO DA LUZ
    //glPushMatrix();
    //glTranslatef(0.0f, 300.0f, 200.0f);
    //glutSolidSphere(10, 5, 5);
    //glPopMatrix();

    // chama a funcao para aplicar iluminacao
    setupLighting(ambientLight, diffuseLight, specularLight, lightPosition);

    glBindTexture(GL_TEXTURE_2D, 0); // desliga a textura atual

    // Define as propriedades do material do player
    float ambientDiffusePlayer[] = {0.6f, 0.6f, 0.6f, 1.0f};
    float specularPlayer[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float shininessPlayer = 20.0f;

    // chama função para desenhar o modelo 3D na tela a cada frame
    setMaterial(ambientDiffusePlayer, specularPlayer, shininessPlayer);
    drawPlayerModel(&player, playerRotation);

    for (int i = 0; i < objectCount; ++i) {
        if (sceneObjects[i].type == PLATFORM) {
            float ambientDiffusePlatform[] = {0.4f, 0.4f, 0.4f, 1.0f};
            float specularPlatform[] = {0.1f, 0.1f, 0.1f, 1.0f};
            float shininessPlatform = 10.0f;

            setMaterial(ambientDiffusePlatform, specularPlatform, shininessPlatform);
            drawPlatform(&sceneObjects[i]);
        }
        else {
            float ambientDiffuseObject[] = {0.3f, 0.3f, 0.65f, 0.2f}; // normalmente usa o diffuse neste parâmetro
            float specularObject[] = {0.4f, 0.4f, 0.4f, 1.0f};
            float shininessObject = 30.0f;

            setMaterial(ambientDiffuseObject, specularObject, shininessObject);
            drawObject(&sceneObjects[i]);
        }
        //drawCollisionBoxWireframe(sceneObjects[i].collision);
    }
    //drawCollisionBoxWireframe(player.collision);

    drawShadow(&player, objectsInCollisionRange, &playerRotation);

    if (gameCompleted) {
        // Salva o estado atual das matrizes
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        // Configura projeção ortográfica 2D
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, winWidth, 0, winHeight);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Desativa recursos que interferem no texto 2D
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);

        // Cor dourada
        glColor3f(1.0f, 0.8f, 0.0f);

        // Mensagem principal - fonte ainda maior
        const char* message1 = "GG! Voce passou em CG!";
        float textWidth1 = 0;
        for (int i = 0; message1[i] != '\0'; i++) {
            textWidth1 += glutBitmapWidth(GLUT_BITMAP_9_BY_15, message1[i]);
        }
        float x1 = (winWidth - textWidth1) / 2;
        float y1 = winHeight * 0.8f; // 60% da altura da tela (mais para cima)

        glRasterPos2f(x1, y1);
        for (int i = 0; message1[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, message1[i]);
        }

        // Mensagem secundária - fonte maior também
        const char* message2 = "Pressione ESC para sair";
        float textWidth2 = 0;
        for (int i = 0; message2[i] != '\0'; i++) {
            textWidth2 += glutBitmapWidth(GLUT_BITMAP_8_BY_13, message2[i]);
        }
        float x2 = (winWidth - textWidth2) / 2;
        float y2 = winHeight * 0.75f; // 40% da altura da tela

        glRasterPos2f(x2, y2);
        for (int i = 0; message2[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, message2[i]);
        }

        // Restaura estado original
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);

        // Restaura as matrizes
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    glutSwapBuffers();
}

void handleKeyboardInput(unsigned char pressedKey, int x, int y) {
    if (gameCompleted && pressedKey == 27) {
        printf("Jogo concluído! Saindo...\n");
        exit(0);
    }

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
    updatePlayerCollisionBox(&player);

    // 6. Verifica colisões com objetos perigosos ANTES do movimento
    for (int i = 0; i < objInColRangeCount; i++) {
        if (objectsInCollisionRange[i].type == DANGER && isObjectColliding(player.collision, objectsInCollisionRange[i].collision)) {
            printf("Colidiu com um objeto perigoso! Morreu!\n");
            respawnPlayer();
            return; // Sai da física neste frame
        }

        else if (objectsInCollisionRange[i].type == FLAG && !objectsInCollisionRange[i].checkpointActivated) {
            if (isObjectColliding(player.collision, objectsInCollisionRange[i].collision)) {
                checkpointX = objectsInCollisionRange[i].x - 2.0f;
                checkpointY = objectsInCollisionRange[i].y + 1.0f;
                checkpointZ = objectsInCollisionRange[i].z - 2.0f;
                printf("Checkpoint atualizado em (%.1f, %.1f, %.1f)\n", checkpointX, checkpointY, checkpointZ);

                // Marca o objeto original como ativado
                for (int j = 0; j < objectCount; j++) {
                    if (sceneObjects[j].x == objectsInCollisionRange[i].x &&
                        sceneObjects[j].y == objectsInCollisionRange[i].y &&
                        sceneObjects[j].z == objectsInCollisionRange[i].z &&
                        sceneObjects[j].type == FLAG) {
                        sceneObjects[j].checkpointActivated = true;
                        break;
                    }
                }
            }
        }

        else if (objectsInCollisionRange[i].type == WIN) {
            if (isObjectColliding(player.collision, objectsInCollisionRange[i].collision)) {
                gameCompleted = true;
                objectsInCollisionRange[i].type = DEFAULT;
            }
        }
    }
    // 7. Processa movimento e colisões com plataformas/paredes
    collideAndSlide(playerVelocity, &player, objectsInCollisionRange, objInColRangeCount, deltaTime);
    getPlayerMovingAngle(playerVelocity, &playerRotation);

    // 7.1 acessa o objeto original que o player tá em cima
    if (player.isOnGround && player.groundObjectIndex >= 0 && player.groundObjectIndex < objInColRangeCount) {
        SceneObject* groundObj = &objectsInCollisionRange[player.groundObjectIndex];

        if (groundObj->anim.isAnimated) {
            // Delta real de movimento da plataforma
            float platform_dx = groundObj->x - groundObj->prevX;
            float platform_dy = groundObj->y - groundObj->prevY;
            float platform_dz = groundObj->z - groundObj->prevZ;

            float movement[3] = {platform_dx, platform_dy, platform_dz};
            movePlayer(movement, &player);
        }
    }

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
    player.isOnGround = false;  // Força a física a detectar o chão novamente
}


// Desenha o texto de que ganhemo o jogo
void drawText(float x, float y, const char *text, void (*font)) {
    // Salva estado
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Configura 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.8f, 0.0f); // Dourado

    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(font, text[i]);
    }

    // Restaura estado
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
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
