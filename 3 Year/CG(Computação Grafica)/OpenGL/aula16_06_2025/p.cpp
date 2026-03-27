/////////////////////////////////
// BolaRampaERetaPerfeita.cpp
//
/////////////////////////////////

#include <iostream>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Variáveis globais
static float t = 0.0;
static float angle = 0.0;
static int isAnimate = 0;
static int animationPeriod = 16;
static float Xangle = 55.0, Yangle = 0.0, Zangle = 0.0;

// Parâmetros da cena
const float rampLength = 10.0;
const float ballRadius = 1.0;
const float flatLength = 15.0;
const float rampAngle = 55.0; // 55 graus de inclinação
const float transitionPoint = 0.7; // 70% rampa, 30% reta

// Offset base para a posição da bola (usado em ambos os trechos para manter coerência)
const float baseOffsetX = 2.0;
const float baseOffsetY = 2.0;

void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawRampAndFlat()
{
    // Desenha rampa (wireframe)
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(-rampAngle, 0.0, 0.0, 1.0);
    glScalef(rampLength, 0.01, 2.0);
    glutWireCube(1.0);
    glPopMatrix();

    // Calcula a posição da extremidade inferior da rampa
    float endX = (rampLength / 2.0) * cos(rampAngle * M_PI / 180.0);
    float endY = -(rampLength / 2.0) * sin(rampAngle * M_PI / 180.0);

    // Desenha reta nivelada (wireframe)
    glPushMatrix();
    glTranslatef(endX + (flatLength / 2.0), endY, 0.0);
    glScalef(flatLength, 0.01, 2.0);
    glutWireCube(1.0);
    glPopMatrix();
}

void drawBall()
{
    glPushMatrix();

    if(t < transitionPoint) // Bola na rampa
    {
        float rampT = t / transitionPoint;
        float dist = rampT * rampLength;

        // Posição ao longo da rampa (origem no centro da rampa)
        float x = -(rampLength / 2.0) + dist -0.75;
        float y = -ballRadius - 0.75;

        // Aplica offset base fixo para todo o movimento
        glTranslatef(baseOffsetX, baseOffsetY, 0.0);

        // Rotaciona para alinhar com a rampa
        glRotatef(-rampAngle, 0.0, 0.0, 1.0);

        // Move bola ao longo da rampa
        glTranslatef(x, y, 0.0);

        // Calcula o ângulo de rotação da bola (giro)
        angle = dist * 360.0 / (2 * M_PI * ballRadius);
    }
    else // Bola na reta
    {
        // Ponto final da rampa
        float rampEndX = (rampLength / 2.0) * cos(rampAngle * M_PI / 180.0);
        float rampEndY = -(rampLength / 2.0) * sin(rampAngle * M_PI / 180.0);

        // Progresso na reta (0 a 1)
        float flatT = (t - transitionPoint) / (1.0 - transitionPoint);
        float dist = flatT * flatLength;

        // Aplica offset base + posição na reta, mantendo y considerando o raio para apoio
        glTranslatef(baseOffsetX + rampEndX + dist, baseOffsetY + rampEndY - ballRadius, 0.0);

        // Continua incrementando o ângulo de rotação da bola para o movimento na reta
        angle += 360.0 * dist / (2 * M_PI * ballRadius);
    }

    // Desenha a bola
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glutWireSphere(ballRadius, 16, 16);

    glPopMatrix();
}

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Posiciona a câmera
    gluLookAt(0.0, 5.0, 15.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Rotação da cena
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    drawRampAndFlat();
    drawBall();

    glutSwapBuffers();
}

void animate(int value)
{
    if(isAnimate)
    {
        t += 0.005;
        if(t >= 1.0) t = 0.0;
    }
    glutTimerFunc(animationPeriod, animate, 1);
    glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case ' ':
            isAnimate = !isAnimate;
            break;
        case 'x':
            Xangle += 5.0;
            break;
        case 'X':
            Xangle -= 5.0;
            break;
        case 'y':
            Yangle += 5.0;
            break;
        case 'Y':
            Yangle -= 5.0;
            break;
        case 'z':
            Zangle += 5.0;
            break;
        case 'Z':
            Zangle -= 5.0;
            break;
    }
    glutPostRedisplay();
}

void printInteraction()
{
    cout << "Interacao:" << endl;
    cout << "Espaco: Play/Pause" << endl;
    cout << "x/X, y/Y, z/Z: Rotacionar cena" << endl;
    cout << "ESC: Sair" << endl;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bola Descendo Rampa Corretamente");

    setup();
    printInteraction();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutTimerFunc(animationPeriod, animate, 1);

    glutMainLoop();
    return 0;
}
