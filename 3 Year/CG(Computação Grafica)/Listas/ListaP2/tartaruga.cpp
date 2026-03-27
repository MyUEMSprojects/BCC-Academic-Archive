#include <iostream>
#include <fstream>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

/*

Nome: Felipe Echeverria Vilhalva
RGM: 45611
PT2 - CG

*/

float angle = 0.0f; // angulo para o "passo" das patas
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate the view/world.

// variável para a animação circular da tartaruga
float turtleCircleAngle = 0.0f; // angulo para a posição da tartaruga na trilha circular
const float circleRadius = 5.0f; // Raio da trilha circular

void drawEllipsoid(float rx, float ry, float rz) {
    glPushMatrix();
    glScalef(rx, ry, rz); // Escala para transformar a esfera em elipsoide
    glutSolidSphere(1.0, 30, 30); // Desenha uma esfera unitária
    glPopMatrix();
}

// Função para desenhar um cilindro
void drawCylinder(float baseRadius, float topRadius, float height) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluCylinder(quadric, baseRadius, topRadius, height, 30, 30);
    gluDeleteQuadric(quadric);
}

void drawTurtle() {
    glPushMatrix();

    // Casco (um elipsoide)
    glColor3f(0.3f, 0.6f, 0.2f); // Verde escuro para o casco
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f); // Levantar o casco um pouco
    drawEllipsoid(1.5f, 0.7f, 2.0f);
    glPopMatrix();

    // Cabeça (elipsoide)
    glColor3f(0.4f, 0.7f, 0.3f); // Verde claro para a cabeça e pescoço
    glPushMatrix();
    glTranslatef(2.3f, 0.3f, 0.0f); // Posição relativa ao casco
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f); // Pequena inclinação da cabeça
    drawEllipsoid(0.4f, 0.3f, 0.3f);
    glPopMatrix();

    // Pescoço (cilindros conectados)
    glColor3f(0.4f, 0.7f, 0.3f);
    glPushMatrix();
    glTranslatef(1.3f, 0.3f, 0.0f); // Posição inicial do pescoço
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotaciona para ficar horizontal
    drawCylinder(0.2f, 0.2f, 0.8f); // baseRadius, topRadius, height
    glPopMatrix();

    // Patas (cilindros)
    glColor3f(0.4f, 0.7f, 0.3f);

    // Amplitude de rotação para o passo
    float legAngle = 15.0f * sin(angle * 3.0f);

    // Pata dianteira direita
    glPushMatrix();
    glTranslatef(1.0f, -0.2f, 0.8f); // Posição no corpo
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotaciona para apontar para baixo inicialmente
    glRotatef(legAngle, 0.0f, 1.0f, 0.0f); // Rotação ao redor do eixo Y (balança para frente/tras)
    drawCylinder(0.25f, 0.2f, 0.5f);
    glPopMatrix();

    // Pata dianteira esquerda
    glPushMatrix();
    glTranslatef(1.0f, -0.2f, -0.8f); // Posicao no corpo
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotaciona para apontar para baixo inicialmente
    glRotatef(-legAngle, 0.0f, 1.0f, 0.0f); // Rotacao oposta ao redor do eixo Y
    drawCylinder(0.25f, 0.2f, 0.5f);
    glPopMatrix();

    // Pata traseira direita
    glPushMatrix();
    glTranslatef(-1.0f, -0.2f, 0.8f); // Posicao no corpo
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotaciona para apontar para baixo inicialmente
    glRotatef(-legAngle, 0.0f, 1.0f, 0.0f); // Rotacao oposta ao redor do eixo Y
    drawCylinder(0.25f, 0.2f, 0.5f);
    glPopMatrix();

    // Pata traseira esquerda
    glPushMatrix();
    glTranslatef(-1.0f, -0.2f, -0.8f); // Posicao no corpo
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotaciona para apontar para baixo inicialmente
    glRotatef(legAngle, 0.0f, 1.0f, 0.0f); // Rotacao igual a dianteira direita
    drawCylinder(0.25f, 0.2f, 0.5f);
    glPopMatrix();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 3.0, 10.0, // Posição da camera
              0.0, 0.0, 0.0,   // Ponto para onde a camera esta olhando (centro da cena)
              0.0, 1.0, 0.0);   // Vetor "up" da camera

    // Aplicar as rotações do mundo/cena controladas pelo teclado
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    // Desenha a trilha circular
    glColor3f(0.5f, 0.3f, 0.0f); // Cor marrom para a trilha
    glPushMatrix();
    glTranslatef(0.0f, -0.48f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutWireTorus(0.1, circleRadius, 20, 40);
    glPopMatrix();

    glPushMatrix();

    // Translada a tartaruga ao longo da trilha circular no sentido horário
    float x = circleRadius * cos(turtleCircleAngle);
    float z = circleRadius * sin(turtleCircleAngle);
    glTranslatef(x, 0.0f, z); // alinhar corretamente com a trilha

    // Rotaciona a tartaruga para ficar alinhada com a direção de movimento (sentido horário)
    glRotatef(-turtleCircleAngle * 180.0f / M_PI - 90.0f, 0.0f, 1.0f, 0.0f);

    drawTurtle();
    glPopMatrix();

    glutSwapBuffers();
}

void animate(int value) {
    angle += 0.05f; // Animação do passo da pata
    turtleCircleAngle += 0.01f; // Animação da trilha circular (velocidade)

    if (turtleCircleAngle > 2 * M_PI) {
        turtleCircleAngle -= 2 * M_PI;
    }

    glutPostRedisplay();
    glutTimerFunc(16, animate, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
   switch(key) {
      case 27: // ESC key
         exit(0);
         break;
      case 'x':
         Xangle += 5.0;
         if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
         if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
         if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
         if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
         if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
         if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tartaruga na Trilha Circular");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyInput);
    glutTimerFunc(0, animate, 0);

    glutMainLoop();
    return 0;
}