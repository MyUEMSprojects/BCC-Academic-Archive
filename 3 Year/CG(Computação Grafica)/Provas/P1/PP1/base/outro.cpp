// Aluno: Felipe Echeverria Vilhalva RGM: 45611
// implementação de Itens 5 e de 21 ao 26
// Professora Mercedes - Computação Gráfica

#include <cstdlib>
#include <vector>
#include <iostream>

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

// minha implementação
#define POLYLINE 4
#define TEXT 5
#define NUMBERPRIMITIVES 5

using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?

// minha implementação
static int gridSize = 10; // Grid size (10 = default)

// Drawing properties
static float currentColor[3] = {0.0, 0.0, 0.0}; // Default: black
static int currentFillMode = GL_LINE; // Default: wireframe
static int currentLinePattern = 0; // Default: solid line

// minha implementação
class Shape {
protected:
    float color[3];
    int fillMode;
    int linePattern;

public:
    Shape() {
        // Capture current format settings when created
        color[0] = currentColor[0];
        color[1] = currentColor[1];
        color[2] = currentColor[2];
        fillMode = currentFillMode;
        linePattern = currentLinePattern;
    }
};
//

// Point class.
class Point : public Shape
{
public:
   Point(int xVal, int yVal)
   {
	  x = xVal; y = yVal;
   }
   void drawPoint(); // Function to draw a point.
private:
   int x, y; // x and y co-ordinates of point.
   static float size; // Size of point.
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
   glColor3fv(color);
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
class Line : public Shape
{
public:
   Line(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};


// Function to draw a line.
void Line::drawLine()
{
   glColor3fv(color);
   if (linePattern > 0) {
       glEnable(GL_LINE_STIPPLE);
       glLineStipple(1, linePattern == 1 ? 0x00FF : 0x0101);
   }
   glBegin(GL_LINES);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();
   if (linePattern > 0) {
       glDisable(GL_LINE_STIPPLE);
   }
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
class Rectangle : public Shape
{
public:
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
   glColor3fv(color);
   glPolygonMode(GL_FRONT_AND_BACK, fillMode);
   if (linePattern > 0 && fillMode == GL_LINE) {
       glEnable(GL_LINE_STIPPLE);
       glLineStipple(1, linePattern == 1 ? 0x00FF : 0x0101);
   }
   glRectf(x1, y1, x2, y2);
   if (linePattern > 0 && fillMode == GL_LINE) {
       glDisable(GL_LINE_STIPPLE);
   }
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

// minha implementação
class Poline : public Shape
{
public:
   Poline(int xVal, int yVal)
   {
      points.push_back(make_pair(xVal, yVal));
   }
   void addPoint(int xVal, int yVal)
   {
      points.push_back(make_pair(xVal, yVal));
   }
   void drawPoline();
private:
   vector<pair<int,int>> points; // Vector of points forming the polyline
};

void Poline::drawPoline()
{
   glColor3fv(color);
   if (linePattern > 0) {
       glEnable(GL_LINE_STIPPLE);
       glLineStipple(1, linePattern == 1 ? 0x00FF : 0x0101);
   }
   glBegin(GL_LINE_STRIP);
   for(const auto& point : points) {
      glVertex3f(point.first, point.second, 0.0);
   }
   glEnd();
   if (linePattern > 0) {
       glDisable(GL_LINE_STIPPLE);
   }
}

// Adicione a declaração do currentPoline aqui, após a definição da classe
static Poline* currentPoline = nullptr;

// Vector of polines.
vector<Poline> polines;

// Iterator to traverse a Poline array.
vector<Poline>::iterator polinesIterator;

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
// minha implementação

// Text class
class Text {
public:
    Text(int xVal, int yVal, const string& str)
        : x(xVal), y(yVal), text(str) {}

    void drawText() {
        glRasterPos2i(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
private:
    int x, y;
    string text;
};

// Vector of texts
vector<Text> texts;
vector<Text>::iterator textsIterator;

// Function to draw all texts
void drawTexts(void) {
    glColor3f(0.0, 0.0, 0.0);
    textsIterator = texts.begin();
    while(textsIterator != texts.end()) {
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
// Function to draw multi-segmentation line (polilinha) box in left selection area.
void drawMultiSegmentationLineSelectionBox(void)
{
   if (primitive == POLYLINE) glColor3f(1.0, 1.0, 1.0); // Highlight correto
   else glColor3f(0.8, 0.8, 0.8);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw black boundary
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw multi-segmentation line icon
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINE_STRIP);
      glVertex3f(0.025*width, 0.625*height, 0.0);
      glVertex3f(0.05*width, 0.65*height, 0.0);
      glVertex3f(0.075*width, 0.625*height, 0.0);
   glEnd();
}

// Function to draw text selection box in left selection area.
void drawTextSelectionBox(void) {
    if (primitive == TEXT) glColor3f(1.0, 1.0, 1.0);
    else glColor3f(0.8, 0.8, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

    // Draw black boundary
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

    // Draw text icon
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(0.025*width, 0.54*height);
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

// Function to draw a grid.
void drawGrid(void)
{
   int i;
   float step = 1.0/gridSize; // Calculate step size based on grid size

   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x5555);
   glColor3f(0.75, 0.75, 0.75);

   glBegin(GL_LINES);
      // Vertical lines
      for (i=1; i < gridSize; i++)
      {
         glVertex3f((0.1 + i*step*(0.9))*width, 0.0, 0.0);
         glVertex3f((0.1 + i*step*(0.9))*width, height, 0.0);
      }
      // Horizontal lines
      for (i=1; i < gridSize; i++)
      {
         glVertex3f(0.1*width, i*step*height, 0.0);
         glVertex3f(width, i*step*height, 0.0);
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
   drawMultiSegmentationLineSelectionBox(); // minha implementação
   drawTextSelectionBox(); // minha implementação
   drawInactiveArea();

   drawPoints();
   drawLines();
   drawRectangles();
   drawPolines();
   drawTexts();

   // minha implementação
   // Desenha a polilinha atual se estiver em construção
   if (currentPoline != nullptr) {
      currentPoline->drawPoline();
   }

   if (primitive == LINE || primitive == RECTANGLE) {
      if (pointCount == 1) drawTempPoint();
   }

   // minha implementação
   if (isGrid) drawGrid();

   glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y) {
    if (y < (1- NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
    else if (y < (1 - 4*0.1)*height) primitive = TEXT;
    else if (y < (1 - 3*0.1)*height) primitive = POLYLINE; // minha implementação
    else if (y < (1 - 2*0.1)*height) primitive = RECTANGLE;
    else if (y < (1 - 1*0.1)*height) primitive = LINE;
    else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   if (state == GLUT_DOWN)
   {
      y = height - y; // Correct from mouse to OpenGL co-ordinates.

      // Click outside canvas - do nothing.
      if ( x < 0 || x > width || y < 0 || y > height ) ;

      // Click in left selection area.
      else if ( x < 0.1*width )
      {
         pickPrimitive(y);
         pointCount = 0;
         // minha implementação
         if (currentPoline != nullptr) {
            delete currentPoline;
            currentPoline = nullptr;
         }
      }

      // Click in canvas.
      else if (button == GLUT_LEFT_BUTTON)
      {
         if (primitive == POINT) points.push_back( Point(x,y) );
         else if (primitive == LINE)
         {
            if (pointCount == 0)
            {
               tempX = x; tempY = y;
               pointCount++;
            }
            else
            {
               lines.push_back( Line(tempX, tempY, x, y) );
               pointCount = 0;
            }
         }
         else if (primitive == RECTANGLE)
         {
            if (pointCount == 0)
            {
               tempX = x; tempY = y;
               pointCount++;
            }
            else
            {
               rectangles.push_back( Rectangle(tempX, tempY, x, y) );
               pointCount = 0;
            }
         }
         else if (primitive == POLYLINE) // minha implementação
         {
            if (pointCount == 0)
            {
               currentPoline = new Poline(x, y);
               pointCount++;
            }
            else
            {
               currentPoline->addPoint(x, y);
            }
         }
         else if (primitive == TEXT) { // minha implementação
            string input;
            cout << "Digite o texto: ";
            getline(cin, input);
            texts.push_back(Text(x, y, input));
            primitive = INACTIVE;
         }
      } // minha implementação
      else if (button == GLUT_MIDDLE_BUTTON && primitive == POLYLINE && pointCount > 0)
      {
         polines.push_back(*currentPoline);
         delete currentPoline;
         currentPoline = nullptr;
         pointCount = 0;
         primitive = INACTIVE;
      }
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
   switch (key)
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void) {
    points.clear();
    lines.clear();
    rectangles.clear();
    polines.clear(); // minha implementação
    texts.clear(); // Adicione esta linha
    if (currentPoline != nullptr) { // minha implementação
        delete currentPoline;
        currentPoline = nullptr;
    }
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

// minha implementação
// The sub-menu callback function.
void grid_menu(int id)
{
   switch(id) {
      case 3: // Grid On
         isGrid = 1;
         break;
      case 4: // Grid Off
         isGrid = 0;
         break;
      case 5: // Fine Grid (20x20)
         gridSize = 20;
         isGrid = 1;
         break;
      case 6: // Medium Grid (10x10)
         gridSize = 10;
         isGrid = 1;
         break;
      case 7: // Coarse Grid (5x5)
         gridSize = 5;
         isGrid = 1;
         break;
   }
   glutPostRedisplay();
}

// minha implementação
void color_menu(int id)
{
    switch(id) {
        case 1: // Black
            currentColor[0] = 0.0; currentColor[1] = 0.0; currentColor[2] = 0.0;
            break;
        case 2: // Red
            currentColor[0] = 1.0; currentColor[1] = 0.0; currentColor[2] = 0.0;
            break;
        case 3: // Green
            currentColor[0] = 0.0; currentColor[1] = 1.0; currentColor[2] = 0.0;
            break;
        case 4: // Blue
            currentColor[0] = 0.0; currentColor[1] = 0.0; currentColor[2] = 1.0;
            break;
    }
    glutPostRedisplay();
}

// minha implementação
void fill_menu(int id)
{
    switch(id) {
        case 1: // Wireframe
            currentFillMode = GL_LINE;
            break;
        case 2: // Filled
            currentFillMode = GL_FILL;
            break;
    }
    glutPostRedisplay();
}

// minha implementação
void line_menu(int id)
{
    switch(id) {
        case 1: // Solid
            currentLinePattern = 0;
            break;
        case 2: // Dashed
            currentLinePattern = 1;
            break;
        case 3: // Dotted
            currentLinePattern = 2;
            break;
    }
    glutPostRedisplay();
}

// minha implementação
// Function to create menu.
void makeMenu(void)
{
    int color_submenu = glutCreateMenu(color_menu);
    glutAddMenuEntry("Black", 1);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Blue", 4);

    int fill_submenu = glutCreateMenu(fill_menu);
    glutAddMenuEntry("Wireframe", 1);
    glutAddMenuEntry("Filled", 2);

    int line_submenu = glutCreateMenu(line_menu);
    glutAddMenuEntry("Solid", 1);
    glutAddMenuEntry("Dashed", 2);
    glutAddMenuEntry("Dotted", 3);

    int grid_submenu = glutCreateMenu(grid_menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off", 4);
    glutAddMenuEntry("Fine Grid (20x20)", 5);
    glutAddMenuEntry("Medium Grid (10x10)", 6);
    glutAddMenuEntry("Coarse Grid (5x5)", 7);

    glutCreateMenu(rightMenu);
    glutAddSubMenu("Color", color_submenu);
    glutAddSubMenu("Fill Mode", fill_submenu);
    glutAddSubMenu("Line Style", line_submenu);
    glutAddSubMenu("Grid", grid_submenu);
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
        << "Right click for menu options:" << endl
        << "- Grid: On/Off and size options (Fine/Medium/Coarse)" << endl
        << "- Clear: Reset canvas" << endl
        << "- Quit: Exit program" << endl;
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

   makeMenu(); // Create menu.

   glutMainLoop();

   return 0;
}