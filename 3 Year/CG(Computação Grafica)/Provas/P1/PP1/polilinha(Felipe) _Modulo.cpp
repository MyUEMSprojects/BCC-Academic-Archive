// Módulo de Desenhar Setor na Aplicação do Canvas (questão 5)- Felipe Echeverria Vilhalva RGM 45611
// Usando como base o canvas.cpp, e como apenas uma pessoa precisa implementar os itens 21 ao 26, deixei sem
// só colocar nó código principal, ignore os erros

// < minha implementação
#define POLYLINE 8 // minha implementação(primitiva + 3)(1 ao 10)
// aumentar numero de primitivas
//#define NUMBERPRIMITIVES 4 // atualiza o numero de primitivas

static float currentRed = 0.0, currentGreen = 0.0, currentBlue = 0.0; // Variáveis que vão ser usadas para armazenar a cor atual
static int isFilled = 0;

class Poline
{
public:
   Poline(int xVal, int yVal, float r = 0.0, float g = 0.0, float b = 0.0, int fill = 0)
   {
      red = r; green = g; blue = b;
      filled = fill;
      points.push_back(make_pair(xVal, yVal));
   }

   void addPoint(int xVal, int yVal)
   {
      points.push_back(make_pair(xVal, yVal));
   }

   void drawPoline();
private:
   vector<pair<int,int>> points; // vetor de pontos formando a polilinha
   float red, green, blue; // cor do ponto
   int filled; // preechimento
};

void Poline::drawPoline()
{
   glColor3f(red, green, blue); // Color of point.

   if(filled)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   //
   if(filled)
   {
      glBegin(GL_POLYGON);
      for(const auto& point : points)
      {
         glVertex3f(point.first, point.second, 0.0);
      }
      glEnd();
   }
   else
   {
      glBegin(GL_LINE_STRIP);
      for(const auto& point : points)
      {
         glVertex3f(point.first, point.second, 0.0);
      }
      glEnd();
   }
}

// vetor de polilinhas
vector<Poline> polines;
// iterador para percorrer o vetor
vector<Poline>::iterator polinesIterator;

static Poline* currentPoline = nullptr;

void drawPolines(void)
{
   // percorrer o vetor de polilinhas desenhando cada polilinha
   polinesIterator = polines.begin();
   while(polinesIterator != polines.end() )
   {
      polinesIterator->drawPoline();
     polinesIterator++;
   }
}

// minha implementação
void drwawMultiSegmentationLineSelectionBox(void)
{
   if (primitive == POLYLINE) glColor3f(1.0, 1.0, 1.0); // destaque.
   else glColor3f(0.8, 0.8, 0.8); // sem destaque.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // desenha a borda preta
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // desenha o ícone da polilinha
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINE_STRIP);
      glVertex3f(0.025*width, 0.625*height, 0.0);
      glVertex3f(0.05*width, 0.65*height, 0.0);
      glVertex3f(0.075*width, 0.625*height, 0.0);
   glEnd();
}

// implementações dentro das funções já existentes
/*
em drawScene:

   drwawMultiSegmentationLineSelectionBox();

   //- depois dos ifs  de LINE e RECTANGLE e antes de glutSwapBuffers();
   if(primitive == POLYLINE)
   {
      drawPolines();
      if(currentPoline != nullptr)
      {
         currentPoline->drawPoline();
      }
   }
em pickPrimitive:
   else if (y < (1 - 3*0.1)*height) primitive = POLYLINE;


em mouseControl:
   else if(primitive == POLYLINE)
       {
          if (pointCount == 0)
          {
             currentPoline = new Poline(x, y, currentRed, currentGreen, currentBlue, isFilled);
             pointCount++;
          }
          else
          {
             if(currentPoline != nullptr)
             {
                currentPoline->addPoint(x, y);
             }
          }
       }

   // bem, existe o "if principal" if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN
   // como else, antes do glutPostRedisplay(), insira:
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
   {
      if(primitive == POLYLINE && currentPoline != nullptr)
      {
         polines.push_back(*currentPoline);
         delete currentPoline;
         currentPoline = nullptr;
         pointCount = 0;
      }
   }
em clearAll():
   // adicione:
   polines.clear(); // minha implementação
   if(currentPoline != nullptr)
   {
      delete currentPoline;
      currentPoline = nullptr;
   }
*/