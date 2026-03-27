// Felipe Echeverria Vilhalva RGM: 45611
// o trabalho foi feito inteiramente reutilizando o código animateMan1.cpp(sem auxilio do chatGPT e afins)

/*
4.59. Enhance animateMan*.cpp:

(a) The character’s body parts, except for the head, are currently all cubes.
Make them more realistically rounded using cylinders.

(b) Add movement to the character’s feet, which are currently fixed with
respect to his lower legs. Give him movable hands as well.

(c) As remarked earlier, all the character’s movements are currently parallel
to a single plane. Enhance to true 3D
*/

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Felipe: OBS -> comentarios novos(em Portugues) com novas funcionalidades vão começar com o meu nome "Felipe"

// Felipe: estrutura para rotação no plano 3D
struct Vector3f {
   float x, y, z;
};

// Globals.
static float highlightColor[3] = {0.0, 0.0, 0.0}; // Emphasize color.
static float lowlightColor[3] = {0.7, 0.7, 0.7}; // De-emphasize color.
static float partSelectColor[3] = {1.0, 0.0, 0.0}; // Selection indicate color.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int animateMode = 0; // In animation mode?
static int animationPeriod = 1000; // Time interval between frames.
static ofstream outFile; // File to write configurations data.

static GLUquadric* q = NULL; // Felipe: objeto para desenhar cilindros
// aumenta o numero de partes para 13, pois antes era 9, agora são + dois pés e + 2 mãos
const int numParts = 13;

// Camera class.
class Camera
{
public:
   Camera();
   void incrementViewDirection();
   void decrementViewDirection();
   void incrementZoomDistance() { zoomDistance += 1.0; }
   void decrementZoomDistance() { zoomDistance -= 1.0; }

   float getViewDirection() const { return viewDirection; }
   float getZoomDistance() const { return zoomDistance; }

private:
   float viewDirection;
   float zoomDistance;
};

// Global camera.
Camera camera;

// Camera constructor.
Camera::Camera()
{
   viewDirection = 0.0;
   zoomDistance = 30.0;
}

// Function to increment camera viewing angle.
void Camera::incrementViewDirection()
{
   viewDirection += 5.0;
   if (viewDirection > 360.0) viewDirection -= 360.0;
}

// Function to decrement camera viewing angle.
void Camera::decrementViewDirection()
{
   viewDirection -= 5.0;
   if (viewDirection < 0.0) viewDirection += 360.0;
}

// Man class.
class Man
{
public:
   Man();
   void incrementSelectedPart();
   void incrementSelectedAxis();

   void incrementPartAngle();
   void decrementPartAngle();

   void incrementUpMove() { upMove += 0.1; }
   void decrementUpMove() { upMove -= 0.1; }
   void setUpMove(float move) { upMove = move; }

   void incrementForwardMove() { forwardMove += 0.1; }
   void decrementForwardMove() { forwardMove -= 0.1; }
   void setForwardMove(float move) { forwardMove = move; }

   void setHighlight(int inputHighlight) { highlight = inputHighlight; }

   void draw();
   void outputData();
   void writeData();

private:
   // Man configuration values.
   // Felipe: Mudei a estrutura para lidar com a rotação 3D(x,y,z) de cada parte do corpo
   Vector3f partAngles[numParts];

   float upMove, forwardMove; // Up and forward translation components - both lie
                                // on the plane parallel to which parts rotate -
                                // therefore all translations and part rotations
                                // are along one fixed plane.

   int selectedPart; // Selected part number - this part can be interactively rotated
                       // in the develop mode.

   // Felipe: adicionei para lidar com os eixos da rotação atuais (0=X, 1=Y, 2=Z)
   int selectedAxis;

   int highlight; // If man is currently selected.
};

// Global vector of man configurations.
vector<Man> manVector;

// Global iterators to traverse manVector.
vector<Man>::iterator manVectorIterator;
vector<Man>::iterator manVectorAnimationIterator;

// Man constructor.
Man::Man()
{
   // Felipe: Adicionei para o padrão 3D
   for (int i=0; i<numParts; i++) {
       partAngles[i].x = 0.0;
       partAngles[i].y = 0.0;
       partAngles[i].z = 0.0;
   }

   upMove = 0.0;
   forwardMove = 0.0;
   selectedPart = 0;
   selectedAxis = 0; // Felipe: eixo X de rotação padrão
   highlight = 1;
}

// Function to incremented selected part..
void Man::incrementSelectedPart()
{
   if (selectedPart < numParts - 1) selectedPart++; // Felipe: antes era 8, agora é numero de partes - 1
   else selectedPart = 0;
}

// Felipe: função para alternar os eixos de rotação
void Man::incrementSelectedAxis()
{
   // (selectedAxis + 1)  move para o proximo eixo da sequencia, e %3(pega o resto da divisão por 3) para ser entre 0-2
    selectedAxis = (selectedAxis + 1) % 3;
}

// Function to increment angle of selected part.
void Man::incrementPartAngle()
{
    // Felipe: agora incrementa o angulo da parte do corpo selecionada no plano 3D
    switch(selectedAxis) {
        case 0: // X
            partAngles[selectedPart].x += 5.0;
            if (partAngles[selectedPart].x > 360.0) partAngles[selectedPart].x -= 360.0;
            break;
        case 1: // Y
            partAngles[selectedPart].y += 5.0;
            if (partAngles[selectedPart].y > 360.0) partAngles[selectedPart].y -= 360.0;
            break;
        case 2: // Z
            partAngles[selectedPart].z += 5.0;
            if (partAngles[selectedPart].z > 360.0) partAngles[selectedPart].z -= 360.0;
            break;
    }
}

// Function to decrement angle of selected part.
void Man::decrementPartAngle()
{
    // Felipe: agora decrementa o angulo da parte do corpo selecionada no plano 3D
    switch(selectedAxis) {
        case 0: // eixo X
            partAngles[selectedPart].x -= 5.0;
            if (partAngles[selectedPart].x < 0.0) partAngles[selectedPart].x += 360.0;
            break;
        case 1: // eixo Y
            partAngles[selectedPart].y -= 5.0;
            if (partAngles[selectedPart].y < 0.0) partAngles[selectedPart].y += 360.0;
            break;
        case 2: // eixo Z
            partAngles[selectedPart].z -= 5.0;
            if (partAngles[selectedPart].z < 0.0) partAngles[selectedPart].z += 360.0;
            break;
    }
}

// Felipe: funcao auxiliar pra desenhar o dedo na mão
void drawFinger(float length, float radius)
{
   glPushMatrix();
   // rotaciona o dedo pra ficar corretamente alinhado ao eixo Y
   glRotatef(-90.0, 1.0, 0.0, 0.0);

   // desenha o dedo
   gluCylinder(q, radius, radius, length, 10, 5);
   glPopMatrix();
}

// Function to draw man.
void Man::draw()
{
   if (highlight||animateMode) glColor3fv(highlightColor);
   else glColor3fv(lowlightColor);

   glPushMatrix();

   // Up and forward translations.
   glTranslatef(0.0, upMove, forwardMove);

   // Torso begin
   if (highlight && !animateMode) if (selectedPart == 0)
      glColor3fv(partSelectColor);

   // Felipe: aplicando a rotação 3D
   glRotatef(partAngles[0].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[0].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[0].x, 1.0, 0.0, 0.0);

   // Felipe: Substitui o cubo por um cilindro para o torso
   glPushMatrix();
   glTranslatef(0.0, -8.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 2.0, 2.0, 16.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Torso end.

   // Head begin.
   glPushMatrix();
   glTranslatef(0.0, 11.5, 0.0);
   glPushMatrix();
   glScalef(2.0, 3.0, 2.0);
   glutWireSphere(1.0, 10, 8);
   glPopMatrix();
   glPopMatrix();
   // Head end.

   // Left arm begin.
   glPushMatrix();

   // Left upper arm begin
   if (highlight && !animateMode) if (selectedPart == 1) glColor3fv(partSelectColor);
   glTranslatef(3.0, 8.0, 0.0);
   glRotatef(180.0, 1.0, 0.0, 0.0); // Felipe: pose padrão
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[1].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[1].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[1].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);
   glPushMatrix();

   // Felipe: mudei para um cilindro
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left upper arm end.

   // Left lower arm begin
   if (highlight && !animateMode) if (selectedPart == 2) glColor3fv(partSelectColor);
   glTranslatef(0.0, 4.0, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[2].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[2].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[2].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);

   // Felipe: mudei para um cilindro
   glPushMatrix();
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left lower arm end.

   // Felipe: adicionando uma mão móvel
   if (highlight && !animateMode) if (selectedPart == 9) glColor3fv(partSelectColor);
   glTranslatef(0.0, 4.5, 0.0);

   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[9].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[9].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[9].x, 1.0, 0.0, 0.0);

   glPushMatrix();
   glutWireSphere(1.2, 8, 6);

   // Felipe: desenhando os dedos da mão esquerda

   glPushMatrix();
   glTranslatef(0.8, 0.0, -0.5); // Felipe: Posição do polegar
   glRotatef(45.0, 0.0, 1.0, 0.0); // Felipe: Rotação do polegar
   drawFinger(1.5, 0.3); // Felipe: Comprimento e raio do dedo
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.5, 0.0, -1.0);
   drawFinger(2.0, 0.3);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 0.0, -1.1);
   drawFinger(2.2, 0.3);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.5, 0.0, -1.0);
   drawFinger(2.0, 0.3);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.9, 0.0, -0.8);
   drawFinger(1.7, 0.3);
   glPopMatrix();

   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left hand end.

   glPopMatrix();
   // Left arm end.

   // Right arm begin.
   glPushMatrix();

   // Right upper arm begin
   if (highlight && !animateMode) if (selectedPart == 3) glColor3fv(partSelectColor);
   glTranslatef(-3.0, 8.0, 0.0);
   glRotatef(180.0, 1.0, 0.0, 0.0); // Initial pose

   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[3].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[3].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[3].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);

   // Felipe: mudei para um cilindro
   glPushMatrix();
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right upper arm end.

   // Right lower arm begin
   if (highlight && !animateMode) if (selectedPart == 4) glColor3fv(partSelectColor);
   glTranslatef(0.0, 4.0, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[4].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[4].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[4].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, 4.0, 0.0);

   // Felipe: mudei para um cilindro
   glPushMatrix();
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right lower arm end.

   // Felipe: adicionando uma mão móvel
   // ideia: adicionar dedos
   if (highlight && !animateMode) if (selectedPart == 10) glColor3fv(partSelectColor);
   glTranslatef(0.0, 4.5, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[10].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[10].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[10].x, 1.0, 0.0, 0.0);

   glPushMatrix();
   glutWireSphere(1.2, 8, 6);

   // Felipe: desenhando os dedos

   glPushMatrix();
   glTranslatef(-0.8, 0.0, -0.5);
   glRotatef(-45.0, 0.0, 1.0, 0.0);
   drawFinger(1.5, 0.3);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.5, 0.0, -1.0);
   drawFinger(2.0, 0.3);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 0.0, -1.1);
   drawFinger(2.2, 0.3);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.5, 0.0, -1.0);
   drawFinger(2.0, 0.3);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.9, 0.0, -0.8);
   drawFinger(1.7, 0.3);
   glPopMatrix();

   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right hand end.

   glPopMatrix();
   // Right arm end.

   // Left leg begin.
   glPushMatrix();

   // Left upper leg begin
   if (highlight && !animateMode) if (selectedPart == 5) glColor3fv(partSelectColor);
   glTranslatef(1.5, -8.0, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[5].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[5].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[5].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);

   // Felipe: mudei para um cilindro
   glPushMatrix();
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left upper leg end.

   // Left lower leg begin
   if (highlight && !animateMode) if (selectedPart == 6) glColor3fv(partSelectColor);
   glTranslatef(0.0, -4.0, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[6].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[6].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[6].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);

   // Felipe: mudei para um cilindro
   glPushMatrix();
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left lower leg end.

   // Felipe: Adicionado pé móvel
   if (highlight && !animateMode) if (selectedPart == 11) glColor3fv(partSelectColor);
   glTranslatef(0.0, -4.5, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[11].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[11].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[11].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -0.5, 0.5);

   // Felipe: Pé
   glPushMatrix();
   glScalef(2.0, 1.0, 3.0);
   glutWireCube(1.0);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Left foot end.

   glPopMatrix();
   // Left leg end.

   // Right leg begin.
   glPushMatrix();

   // Right upper leg begin
   if (highlight && !animateMode) if (selectedPart == 7) glColor3fv(partSelectColor);
   glTranslatef(-1.5, -8.0, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[7].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[7].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[7].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);

   // Felipe: mudei para um cilindro
   glPushMatrix();
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right upper leg end.

   // Right lower leg begin
   if (highlight && !animateMode) if (selectedPart == 8) glColor3fv(partSelectColor);
   glTranslatef(0.0, -4.0, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[8].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[8].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[8].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -4.0, 0.0);

   // Felipe: mudei para um cilindro
   glPushMatrix();
   glTranslatef(0.0, -4.0, 0.0);
   glRotatef(90.0, -1.0, 0.0, 0.0);
   gluCylinder(q, 1.0, 1.0, 8.0, 15, 5);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right lower leg end.

   // Felipe: Adicionado pé móvel
   if (highlight && !animateMode) if (selectedPart == 12) glColor3fv(partSelectColor);
   glTranslatef(0.0, -4.5, 0.0);
   // Felipe: aplicando rotação 3D
   glRotatef(partAngles[12].z, 0.0, 0.0, 1.0);
   glRotatef(partAngles[12].y, 0.0, 1.0, 0.0);
   glRotatef(partAngles[12].x, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -0.5, 0.5);

   // Felipe: Pé
   glPushMatrix();
   glScalef(2.0, 1.0, 3.0);
   glutWireCube(1.0);
   glPopMatrix();

   if (highlight && !animateMode) glColor3fv(highlightColor);
   // Right foot end.

   glPopMatrix();
   // Right leg end.

   glPopMatrix();
}

// Function to output configurations data to file.
void Man::outputData()
{
   int i;
   // Felipe: escreve os 3 angulos para cada parte do corpo
   for (i = 0; i < numParts; i++) {
       outFile << partAngles[i].x << " " << partAngles[i].y << " " << partAngles[i].z << " ";
   }
   outFile << upMove << " " << forwardMove << endl;
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, const char *string)
{
   const char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to convert floating point to char string.
void floatToString(char * destStr, int precision, float val)
{
	sprintf(destStr,"%f",val);
	destStr[precision] = '\0';
}

// Routine to write configurations data.
void Man::writeData()
{
   // Felipe: aumentei o numero de buffers para cada eixo
   char buffer [33];
   char buffer2[33];
   char buffer3[33];

   // Felipe: mostra a parte selecionada e o eixo
   glRasterPos3f(-28.0, 12.0, 0.0);
   writeBitmapString((void*)font, "Selected part: ");

   switch (selectedPart)
   {
      case 0: writeBitmapString((void*)font, "Torso"); break;
      case 1: writeBitmapString((void*)font, "Left Upper Arm"); break;
      case 2: writeBitmapString((void*)font, "Left Lower Arm"); break;
      case 3: writeBitmapString((void*)font, "Right Upper Arm"); break;
      case 4: writeBitmapString((void*)font, "Right Lower Arm"); break;
      case 5: writeBitmapString((void*)font, "Left Upper Leg"); break;
      case 6: writeBitmapString((void*)font, "Left Lower Leg"); break;
      case 7: writeBitmapString((void*)font, "Right Upper Leg"); break;
      case 8: writeBitmapString((void*)font, "Right Lower Leg"); break;
      case 9: writeBitmapString((void*)font, "Left Hand"); break;
      case 10: writeBitmapString((void*)font, "Right Hand"); break;
      case 11: writeBitmapString((void*)font, "Left Foot"); break;
      case 12: writeBitmapString((void*)font, "Right Foot"); break;
   }

   glRasterPos3f(-28.0, 10.0, 0.0);
   writeBitmapString((void*)font, "Selected axis: "); // Felipe: eixo selecionado
   if (selectedAxis == 0) writeBitmapString((void*)font, "X");
   else if (selectedAxis == 1) writeBitmapString((void*)font, "Y");
   else writeBitmapString((void*)font, "Z");

   // Felipe: mostra os angulos para a parte selecionado
   floatToString(buffer, 4, partAngles[selectedPart].x);
   floatToString(buffer2, 4, partAngles[selectedPart].y);
   floatToString(buffer3, 4, partAngles[selectedPart].z);

   glRasterPos3f(-28.0, 8.0, 0.0);
   writeBitmapString((void*)font, "Angles (X,Y,Z): ");
   writeBitmapString((void*)font, buffer);
   writeBitmapString((void*)font, ", ");
   writeBitmapString((void*)font, buffer2);
   writeBitmapString((void*)font, ", ");
   writeBitmapString((void*)font, buffer3);

   // Felipe: mostra os valores de movimento abaixo dos dados de angulo
   floatToString(buffer, 4, upMove);
   glRasterPos3f(-28.0, 6.0, 0.0);
   writeBitmapString((void*)font, "upMove = ");
   glRasterPos3f(-11.0, 6.0, 0.0);
   writeBitmapString((void*)font, buffer);

   floatToString(buffer, 4, forwardMove);
   glRasterPos3f(-28.0, 4.0, 0.0);
   writeBitmapString((void*)font, "forwardMove = ");
   glRasterPos3f(-11.0, 4.0, 0.0);
   writeBitmapString((void*)font, buffer);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   // Write text in isolated (i.e., before gluLookAt) translate block.
   glPushMatrix();
   glTranslatef(0.0, 0.0, -30.0);
   glColor3fv(highlightColor);
   glRasterPos3f(-28.0, 25.0, 0.0);
   if (!animateMode)
   {
	  writeBitmapString((void*)font, "DEVELOP MODE");
      manVectorIterator->writeData();
   }
   else writeBitmapString((void*)font, "ANIMATE MODE");
   glPopMatrix();

   // Place camera.
   gluLookAt(camera.getZoomDistance() * sin(camera.getViewDirection()*PI/180.0), 0.0,
	         camera.getZoomDistance() * cos(camera.getViewDirection()*PI/180.0), 0.0,
			 0.0, 0.0, 0.0, 1.0, 0.0);

   // Local iterator to traverse manVector.
   vector<Man>::iterator localManVectorIterator;

   // Move man right 10 units because of data text on left of screen.
   glTranslatef(10.0, 0.0, 0.0);

   if (!animateMode) // Develop mode.
   {
      localManVectorIterator = manVector.begin();

      // Draw all the configurations in manVector.
      while(localManVectorIterator != manVector.end() )
	  {
         localManVectorIterator->draw();
	     localManVectorIterator++;
	  }
   }
   else // Animated mode -
	    // use separate iterator so as to leave develop mode iterator unchanged.
   {
      manVectorAnimationIterator->draw();
   }

   // Other (fixed) objects in scene are drawn below starting here.

   // Black floor.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   for(float z=-25.0; z<100.0; z+=5.0)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(float x=-50.0; x<50.0; x+=5.0)
	  {
	     glVertex3f(x, -25.0, z);
	     glVertex3f(x, -25.0, z+5.0);
	  }
      glEnd();
   }

   // Green sphere.
   glColor3f(0.0, 1.0, 0.0);
   glTranslatef(0.0, -20.0, 10.0);
   glPushMatrix();
   glScalef(5.0, 5.0, 5.0);
   glutWireSphere(1.0, 10, 8);
   glPopMatrix();

   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   if (animateMode)
   {
      manVectorAnimationIterator++;
	  if (manVectorAnimationIterator == manVector.end() )
		  manVectorAnimationIterator = manVector.begin();
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Function to write configurations to file.
void outputConfigurations(void)
{
   // Local iterator to traverse manVector.
   vector<Man>::iterator localManVectorIterator;

   outFile.open("animateManDataOut.txt");
   localManVectorIterator = manVector.begin();
   while(localManVectorIterator != manVector.end() )
   {
      localManVectorIterator->outputData();
	  localManVectorIterator++;
   }
   outFile.close();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);

   // Felipe: Inicializa o objeto
   q = gluNewQuadric();
   gluQuadricDrawStyle(q, GLU_LINE);

   // Initialize global manVector with single configuration.
   manVector.push_back(Man() );

   // Initialize global iterators for manVector.
   manVectorIterator = manVector.begin();
   manVectorAnimationIterator = manVector.begin();

   // Initialize camera.
   camera = Camera();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   vector<Man>::iterator localManVectorIterator;
   switch(key)
   {
      case 27:
         exit(0);
         break;
	  case 'a': // Toggle between develop and animate modes.
         if (animateMode == 0)
		 {
	        manVectorAnimationIterator = manVector.begin();
			outputConfigurations(); // Write configurations data to file at end of develop mode.
			animateMode = 1;
		 }
		 else animateMode = 0;
         glutPostRedisplay();
		 break;
	  case 'r': // Rotate camera.
         camera.incrementViewDirection();
         glutPostRedisplay();
		 break;
	  case 'R': // Rotate camera.
         camera.decrementViewDirection();
         glutPostRedisplay();
		 break;
	  case 'z': // Zoom in.
	     camera.decrementZoomDistance();
         glutPostRedisplay();
		 break;
	  case 'Z': // Zoom out.
	     camera.incrementZoomDistance();
         glutPostRedisplay();
		 break;
	  case 'n': // Create new man configuration.
		 // Turn highlight off current configuration.
         manVectorIterator->setHighlight(0);

		 localManVectorIterator = manVectorIterator;
		 manVectorIterator++;

		 // Insert copy of current configuration and highlight.
         manVectorIterator =
		   manVector.insert(manVectorIterator, Man(*localManVectorIterator) );
         manVectorIterator->setHighlight(1);

		 glutPostRedisplay();
		 break;
      case ' ': // Select next body part.
		 manVectorIterator->incrementSelectedPart();
         glutPostRedisplay();
		 break;
      // Felipe: muda o eixo de rotação
      case 'c':
         manVectorIterator->incrementSelectedAxis();
         glutPostRedisplay();
         break;

	  // Tab - select next man configuration.
	  case 9:
		 // Turn highlight off current configuration.
		 manVectorIterator->setHighlight(0);

		 // Increment iterator - go to start if at end already.
		 manVectorIterator++;
         if (manVectorIterator == manVector.end() )
			 manVectorIterator = manVector.begin();

		 // Highlight current configuration.
		 manVectorIterator->setHighlight(1);

		 glutPostRedisplay();
		 break;

	  // Backspace - reset current man configuration,
	  case 8:
		 if (manVectorIterator != manVector.begin() ) // Not first configuration.
		 {
		    // Make copy of the previous configuration and highlight.
		    localManVectorIterator = manVectorIterator;
		    localManVectorIterator--;
            manVectorIterator =
		    manVector.insert(manVectorIterator, Man(*localManVectorIterator) );
		    manVectorIterator->setHighlight(1);

    		// Delete current configuration.
		    manVectorIterator++;
		    manVectorIterator = manVector.erase(manVectorIterator);

		    // Return iterator.
		    manVectorIterator--;
		 }
		 else // First configuration
		 {
			// Delete the current configuration.
	        manVectorIterator = manVector.erase(manVectorIterator);

			// Create new configuration.
			manVector.insert(manVectorIterator, Man() );
		 }

		 glutPostRedisplay();
		 break;

	  // Delete - delete current man configuration.
	  case 127:
		 if (manVector.size() > 1)
		 {
		    manVectorIterator = manVector.erase(manVectorIterator);
		    if (manVectorIterator != manVector.begin() )
				manVectorIterator--;

         // Highlight current configuration.
		 manVectorIterator->setHighlight(1);
		 }

		 glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_PAGE_DOWN) manVectorIterator->decrementPartAngle();
   if (key == GLUT_KEY_PAGE_UP) manVectorIterator->incrementPartAngle();
   if (key == GLUT_KEY_LEFT) manVectorIterator->decrementForwardMove();
   if (key == GLUT_KEY_RIGHT) manVectorIterator->incrementForwardMove();
   if (key == GLUT_KEY_DOWN)
   {
      if (!animateMode) manVectorIterator->decrementUpMove();
	  else animationPeriod +=10;
   }
   if( key == GLUT_KEY_UP)
   {
      if (!animateMode) manVectorIterator->incrementUpMove();
	  else if (animationPeriod > 10) animationPeriod -=10;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press a to toggle between develop and animate modes." << endl
	    << endl
        << "In develop mode:" << endl
	    << "Press the space bar to select a part." << endl
        // Felipe: adicione mais uma instrução de interação para mudar os eixos de rotação
        << "Press c to cycle through the axes of rotation (X, Y, Z)." << endl
        << "Press the page up/page down keys to rotate the selected part around the selected axis." << endl
        << "Press the left/right/up/down arrow keys to move the whole configuration." << endl
		<< "Press r/R to rotate the viewpoint." << endl
		<< "Press z/Z to zoom in/out." << endl
		<< "Press n to create a new configuration - other configurations are ghosted" << endl
		<< "(the new configuration is a copy of the current one so it must be moved to be visible)." << endl
		<< "Press tab to choose a configuration - it is highlighted, others ghosted." << endl
		<< "Press backspace to reset current configuration." << endl
		<< "Press delete to delete current configuration." << endl
		<<  endl
		<< "In animate mode:" << endl
		<< "Press the up/down arrow keys to speed up/slow down animation." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   // Felipe: Aumentei um pouco a tela, antes era 500x500
   glutInitWindowSize(600, 700);
   glutInitWindowPosition(100, 100);
   glutCreateWindow ("animateMan1.cpp (Modificado por Felipe Echeverria Vilhalva, seguindo o Exercicio 4.59 (a), (b) e (c)");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop();

   return 0;
}
