////////////////////////////////////////////////////////////////////////////////////
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
//  Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath> // minha implementação

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define POLYLINE 8 // minha implementação(primitiva + 3)(1 ao 10)
#define TEXT 9
#define NUMBERPRIMITIVES 5 // Aumentar para incluir TEXT

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?

static int currentX, currentY; // Current mouse position
static bool isDrawing = false; // Flag to indicate if we're in the middle of drawing

static float currentRed = 0.0, currentGreen = 0.0, currentBlue = 0.0; // Variáveis que vão ser usadas para armazenar a cor atual
static int isFilled = 0; //

static int gridSize = 10; // Default grid size (10x10)

// Adicione estas variáveis globais
static bool currentFilled = false;  // Estado atual de preenchimento
static bool currentDashed = false;  // Estado atual de linha tracejada

// Adicione estas variáveis globais
static string currentText = "";
static bool isTyping = false;

// Point class.
class Point
{
public:
   Point(int xVal, int yVal, float r = 0.0, float g = 0.0, float b = 0.0)
   {
      x = xVal; y = yVal;
      red = r; green = g; blue = b;
   }
   void drawPoint(void); // Function to draw a point.
private:
   int x, y; // x and y co-ordinates of point.
   static float size; // Size of point.
   float red, green, blue;
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
   glColor3f(red, green, blue);
   glPointSize(size);
   glBegin(GL_POINTS);
      glVertex3f(x, y, 0.0);
   glEnd();
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator;

// Function to draw all points in the points array.
void drawPoints(void)
{
   // Loop through the points array drawing each point.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() )
   {
      pointsIterator->drawPoint();
	  pointsIterator++;
   }
}

// Line class.
class Line
{
public:
   Line(int x1Val, int y1Val, int x2Val, int y2Val, float r = 0.0, float g = 0.0, float b = 0.0, bool dashed = false)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      red = r; green = g; blue = b;
      isDashed = dashed;
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
   float red, green, blue;
   bool isDashed;
};


// Function to draw a line.
void Line::drawLine()
{
   glColor3f(red, green, blue);

   if (isDashed)
   {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00FF);
   }

   glBegin(GL_LINES);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();

   if (isDashed)
      glDisable(GL_LINE_STIPPLE);
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines(void)
{
   // Loop through the lines array drawing each line.
   linesIterator = lines.begin();
   while(linesIterator != lines.end() )
   {
      linesIterator->drawLine();
	  linesIterator++;
   }
}

// Rectangle class.
class Rectangle
{
public:
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val, float r = 0.0, float g = 0.0, float b = 0.0, bool fill = false, bool dashed = false)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      red = r; green = g; blue = b;
      isFilled = fill;
      isDashed = dashed;
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
   float red, green, blue;
   bool isFilled;
   bool isDashed;
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
   glColor3f(red, green, blue);

   if (isDashed)
   {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00FF);
   }

   if (isFilled)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glRectf(x1, y1, x2, y2);

   if (isDashed)
      glDisable(GL_LINE_STIPPLE);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
   // Loop through the rectangles array drawing each rectangle.
   rectanglesIterator = rectangles.begin();
   while(rectanglesIterator != rectangles.end() )
   {
      rectanglesIterator->drawRectangle();
	  rectanglesIterator++;
   }
}

// < minha implementação

// Modificar a classe Poline
class Poline
{
public:
   Poline(int xVal, int yVal, float r = 0.0, float g = 0.0, float b = 0.0,
          int fill = 0, bool dashed = false)  // Adicionado parâmetro dashed
   {
      red = r; green = g; blue = b;
      filled = fill;
      isDashed = dashed;  // Novo atributo
      points.push_back(make_pair(xVal, yVal));
   }

   void addPoint(int xVal, int yVal)
   {
      points.push_back(make_pair(xVal, yVal));
   }

   void drawPoline();
   const vector<pair<int,int>>& getPoints() const { return points; }
private:
   vector<pair<int,int>> points; // Vector of points forming the polyline
   float red, green, blue; // Color of point.
   int filled; // preechimento
   bool isDashed;  // Novo atributo
};

// Modificar o método drawPoline
void Poline::drawPoline()
{
   glColor3f(red, green, blue);

   if(isDashed)  // Adicionar controle de linha tracejada
   {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00FF);
   }

   if(filled)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_POLYGON);
      for(const auto& point : points)
      {
         glVertex3f(point.first, point.second, 0.0);
      }
      glEnd();
   }
   else
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glBegin(GL_LINE_STRIP);
      for(const auto& point : points)
      {
         glVertex3f(point.first, point.second, 0.0);
      }
      glEnd();
   }

   if(isDashed)  // Desabilitar linha tracejada
   {
      glDisable(GL_LINE_STIPPLE);
   }
}

// Vector of polines.
vector<Poline> polines;
// Iterator to traverse a Poline array.
vector<Poline>::iterator polinesIterator;

static Poline* currentPoline = nullptr;

void drawPolines(void)
{
   // Loop through the polines array drawing each poline.
   polinesIterator = polines.begin();
   while(polinesIterator != polines.end() )
   {
      polinesIterator->drawPoline();
     polinesIterator++;
   }
}
//>

// Adicione esta nova classe após as outras classes
class Text
{
public:
    Text(int xVal, int yVal, string textVal, float r = 0.0, float g = 0.0, float b = 0.0)
    {
        x = xVal; y = yVal;
        text = textVal;
        red = r; green = g; blue = b;
    }
    void drawText();
private:
    int x, y;
    string text;
    float red, green, blue;
};

void Text::drawText()
{
    glColor3f(red, green, blue);
    glRasterPos2i(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

// Adicione estes containers globais
vector<Text> texts;
vector<Text>::iterator textsIterator;

void drawTexts(void)
{
    textsIterator = texts.begin();
    while(textsIterator != texts.end())
    {
        textsIterator->drawText();
        textsIterator++;
    }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{
   if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Draw point icon.
   glPointSize(pointSize);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3f(0.05*width, 0.95*height, 0.0);
   glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{
   if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw line icon.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.825*height, 0.0);
      glVertex3f(0.075*width, 0.875*height, 0.0);
   glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{
   if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw rectangle icon.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
   glEnd();
}

// minha implementação
void drwawMultiSegmentationLineSelectionBox(void)
{
   if (primitive == POLYLINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw multi-segmentation line icon.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINE_STRIP);
      glVertex3f(0.025*width, 0.625*height, 0.0);
      glVertex3f(0.05*width, 0.65*height, 0.0);
      glVertex3f(0.075*width, 0.625*height, 0.0);
   glEnd();
}

// Adicione esta nova função junto com as outras funções de desenho de seleção
void drawTextSelectionBox(void)
{
    if (primitive == TEXT) glColor3f(1.0, 1.0, 1.0); // Highlight.
    else glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

    // Draw text icon.
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(0.03*width, 0.54*height);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'T');
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
   glColor3f(0.6, 0.6, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(tempX, tempY, 0.0);
   glEnd();
}

// Modify the drawGrid function
void drawGrid(void)
{
   int i;
   float cellWidth = width / (float)gridSize;
   float cellHeight = height / (float)gridSize;

   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x5555);
   glColor3f(0.75, 0.75, 0.75);

   glBegin(GL_LINES);
      // Vertical lines
      for (i = 1; i < gridSize; i++)
      {
         glVertex3f(i * cellWidth, 0.0, 0.0);
         glVertex3f(i * cellWidth, height, 0.0);
      }
      // Horizontal lines
      for (i = 1; i < gridSize; i++)
      {
         glVertex3f(0.1*width, i * cellHeight, 0.0);
         glVertex3f(width, i * cellHeight, 0.0);
      }
   glEnd();
   glDisable(GL_LINE_STIPPLE);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   drawPointSelectionBox();
   drawLineSelectionBox();
   drawRectangleSelectionBox();
   drwawMultiSegmentationLineSelectionBox();
   drawTextSelectionBox();
   drawInactiveArea();

   drawPoints();
   drawLines();
   drawRectangles();
   drawPolines();
   drawTexts();

   if(primitive == LINE && pointCount == 1)
   {
      // Preview da linha
      glColor3f(1.0, 0.0, 0.0);
      glBegin(GL_LINES);
         glVertex3f(tempX, tempY, 0.0);
         glVertex3f(currentX, currentY, 0.0);
      glEnd();
   }
   else if(primitive == RECTANGLE && pointCount == 1)
   {
      // Preview do retângulo
      glColor3f(1.0, 0.0, 0.0);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glRectf(tempX, tempY, currentX, currentY);
   }
   else if(primitive == POLYLINE && currentPoline != nullptr)
   {
      // Desenha a polilinha atual
      currentPoline->drawPoline();

      // Preview da linha do último ponto até o mouse
      if(currentPoline->getPoints().size() > 0)
      {
         const auto& lastPoint = currentPoline->getPoints().back();
         glColor3f(1.0, 0.0, 0.0);
         glBegin(GL_LINES);
            glVertex3f(lastPoint.first, lastPoint.second, 0.0);
            glVertex3f(currentX, currentY, 0.0);
         glEnd();
      }
   }

   // Adicione isto para mostrar o texto sendo digitado
   if (isTyping)
   {
       glColor3f(currentRed, currentGreen, currentBlue);
       glRasterPos2i(tempX, tempY);
       for (char c : currentText)
       {
           glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
       }
   }

   if (isGrid) drawGrid();

   glutSwapBuffers();
}

// Modificar a função pickPrimitive para salvar a polilinha antes de mudar
void pickPrimitive(int y)
{
   // Salvar polilinha atual se existir
   if(primitive == POLYLINE && currentPoline != nullptr)
   {
      polines.push_back(*currentPoline);
      delete currentPoline;
      currentPoline = nullptr;
   }

   // Então mudar a primitiva
   if ( y < (1- NUMBERPRIMITIVES*0.1)*height ) primitive = INACTIVE;
   else if ( y < (1 - 4*0.1)*height ) primitive = TEXT;
   else if ( y < (1 - 3*0.1)*height ) primitive = POLYLINE;
   else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;
   else if ( y < (1 - 1*0.1)*height ) primitive = LINE;
   else primitive = POINT;
}

// Modificar mouseControl para adicionar suporte ao botão do meio
void mouseControl(int button, int state, int x, int y)
{
    y = height - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if ( x < 0 || x > width || y < 0 || y > height ) ;

        // Click in left selection area.
        else if ( x < 0.1*width )
        {
            pickPrimitive(y);
            pointCount = 0;
            isDrawing = false;
        }

        // Click in canvas.
        else
        {
            if (primitive == POINT)
            {
                points.push_back(Point(x, y, currentRed, currentGreen, currentBlue));
                isDrawing = false;
            }
            else if (primitive == LINE)
            {
                if (pointCount == 0)
                {
                    tempX = x; tempY = y;
                    pointCount++;
                    isDrawing = true;
                }
                else
                {
                    lines.push_back(Line(tempX, tempY, x, y, currentRed, currentGreen, currentBlue, currentDashed));
                    pointCount = 0;
                    isDrawing = false;
                }
            }
            else if (primitive == RECTANGLE)
            {
                if (pointCount == 0)
                {
                    tempX = x; tempY = y;
                    pointCount++;
                    isDrawing = true;
                }
                else
                {
                    rectangles.push_back(Rectangle(tempX, tempY, x, y, currentRed, currentGreen, currentBlue, currentFilled, currentDashed));
                    pointCount = 0;
                    isDrawing = false;
                }
            }
            else if(primitive == POLYLINE)
            {
                if (pointCount == 0)
                {
                    currentPoline = new Poline(x, y, currentRed, currentGreen, currentBlue,
                                              currentFilled, currentDashed);  // Adicionar currentDashed
                    pointCount++;
                    isDrawing = true;
                }
                else
                {
                    if(currentPoline != nullptr)
                    {
                        currentPoline->addPoint(x, y);
                    }
                }
            }
            else if (primitive == TEXT && !isTyping)
            {
                tempX = x;
                tempY = y;
                isTyping = true;
                currentText = "";
            }
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
    {
        // Finalizar polilinha com clique do meio
        if(primitive == POLYLINE && currentPoline != nullptr)
        {
            polines.push_back(*currentPoline);
            delete currentPoline;
            currentPoline = nullptr;
            pointCount = 0;
            isDrawing = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // Remover o código de finalização da polilinha daqui
        // já que agora usamos o botão do meio
    }
    glutPostRedisplay();
}

// Add this function before main()
void mouseMotion(int x, int y)
{
    y = height - y; // Correct from mouse to OpenGL co-ordinates.
    currentX = x;
    currentY = y;
    glutPostRedisplay();
}

// Add this new menu callback function
void grid_size_menu(int id)
{
   switch(id)
   {
      case 5:  // 5x5 grid
         gridSize = 5;
         break;
      case 6:  // 10x10 grid
         gridSize = 10;
         break;
      case 7:  // 20x20 grid
         gridSize = 20;
         break;
      case 8:  // 50x50 grid
         gridSize = 50;
         break;
   }
   glutPostRedisplay();
}

// Add this new menu callback function
void color_menu(int id)
{
   switch(id)
   {
      case 9:  // Black
         currentRed = 0.0; currentGreen = 0.0; currentBlue = 0.0;
         break;
      case 10: // Red
         currentRed = 1.0; currentGreen = 0.0; currentBlue = 0.0;
         break;
      case 11: // Green
         currentRed = 0.0; currentGreen = 1.0; currentBlue = 0.0;
         break;
      case 12: // Blue
         currentRed = 0.0; currentGreen = 0.0; currentBlue = 1.0;
         break;
      case 13: // Yellow
         currentRed = 1.0; currentGreen = 1.0; currentBlue = 0.0;
         break;
      case 14: // Magenta
         currentRed = 1.0; currentGreen = 0.0; currentBlue = 1.0;
         break;
      case 15: // Cyan
         currentRed = 0.0; currentGreen = 1.0; currentBlue = 1.0;
         break;
   }
   glutPostRedisplay();
}

void fill_menu(int id)
{
    switch(id)
    {
        case 16: // Filled
            currentFilled = true;
            break;
        case 17: // Wireframe
            currentFilled = false;
            break;
    }
    glutPostRedisplay();
}

void line_style_menu(int id)
{
    switch(id)
    {
        case 18: // Solid
            currentDashed = false;
            break;
        case 19: // Dashed
            currentDashed = true;
            break;
    }
    glutPostRedisplay();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

   // Pass the size of the OpenGL window to globals.
   width = w;
   height = h;

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    if (isTyping)
    {
        if (key == 13) // Enter key
        {
            texts.push_back(Text(tempX, tempY, currentText,
                               currentRed, currentGreen, currentBlue));
            currentText = "";
            isTyping = false;
            primitive = INACTIVE;
        }
        else if (key == 8 && !currentText.empty()) // Backspace
        {
            currentText.pop_back();
        }
        else if (key >= 32 && key <= 126) // ASCII printable characters
        {
            currentText += key;
        }
        glutPostRedisplay();
        return;
    }

    switch (key)
    {
        case 27: // Escape
            exit(0);
            break;
        default:
            break;
    }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
   points.clear();
   lines.clear();
   rectangles.clear();

   polines.clear(); // minha implementação
   if(currentPoline != nullptr)
   {
      delete currentPoline;
      currentPoline = nullptr;
   }

   texts.clear();
   currentText = "";
   isTyping = false;

   primitive = INACTIVE;
   pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
   if (id==1)
   {
	  clearAll();
	  glutPostRedisplay();
   }
   if (id==2) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
   if (id==3) isGrid = 1;
   if (id==4) isGrid = 0;
   glutPostRedisplay();
}

// Modify the makeMenu function
void makeMenu(void)
{
    int grid_submenu;
    int grid_size_submenu;
    int color_submenu;
    int fill_submenu;
    int line_style_submenu;

    // Grid on/off submenu
    grid_submenu = glutCreateMenu(grid_menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off", 4);

    // Grid size submenu
    grid_size_submenu = glutCreateMenu(grid_size_menu);
    glutAddMenuEntry("5 x 5", 5);
    glutAddMenuEntry("10 x 10", 6);
    glutAddMenuEntry("20 x 20", 7);
    glutAddMenuEntry("50 x 50", 8);

    // Color submenu
    color_submenu = glutCreateMenu(color_menu);
    glutAddMenuEntry("Black", 9);
    glutAddMenuEntry("Red", 10);
    glutAddMenuEntry("Green", 11);
    glutAddMenuEntry("Blue", 12);
    glutAddMenuEntry("Yellow", 13);
    glutAddMenuEntry("Magenta", 14);
    glutAddMenuEntry("Cyan", 15);

    // Fill submenu
    fill_submenu = glutCreateMenu(fill_menu);
    glutAddMenuEntry("Filled", 16);
    glutAddMenuEntry("Wireframe", 17);

    // Line style submenu
    line_style_submenu = glutCreateMenu(line_style_menu);
    glutAddMenuEntry("Solid", 18);
    glutAddMenuEntry("Dashed", 19);

    // Main menu
    glutCreateMenu(rightMenu);
    glutAddSubMenu("Grid", grid_submenu);
    glutAddSubMenu("Grid Size", grid_size_submenu);
    glutAddSubMenu("Color", color_submenu);
    glutAddSubMenu("Fill", fill_submenu);
    glutAddSubMenu("Line Style", line_style_submenu);
    glutAddMenuEntry("Clear", 1);
    glutAddMenuEntry("Quit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Left click on a box on the left to select a primitive." << endl
         << "Then left click on the drawing area: once for point, twice for line or rectangle." << endl
         << "For polyline, use left clicks to add points and middle click to finish." << endl
         << "For text, click where you want to start typing, type your text and press Enter to finish." << endl
         << "Right click for menu options (grid on/off, grid size, colors, clear, quit)." <<  endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("canvas.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseControl);

   glutMotionFunc(mouseMotion);
   glutPassiveMotionFunc(mouseMotion);

   makeMenu(); // Create menu.

   glutMainLoop();

   return 0;
}