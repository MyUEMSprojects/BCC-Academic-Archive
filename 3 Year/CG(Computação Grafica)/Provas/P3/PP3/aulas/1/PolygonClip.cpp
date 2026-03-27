#include <windows.h>   // use as needed for your system
#include <gl/gl.h>
#include <gl/freeglut.h>
#include <iostream>
using namespace std;
//<<<<<<< Data structure
struct GLintPoint
{ GLint x,y;
};

struct GLfloatPoint
{ GLfloat x,y;
};

const int MAX = 100;
class GLintPointArray
{
public:
  int num;
  GLintPoint pt[MAX];
};

class GLfloatPointArray
{
public:
  int num;
  GLfloatPoint pt[MAX];
};

//subprograms
typedef GLfloat colorType[3];

void drawDot (GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b)
{ glColor3f(r,g,b);
  glBegin (GL_POINTS);
	  glVertex2i (x,y);
  glEnd();
}

void drawIntPolygon (GLintPointArray P, colorType c)
{ glColor3fv (c);
  glBegin(GL_LINE_LOOP);
    for (int i=0; i < P.num; i++)
	  glVertex2i (P.pt[i].x,P.pt[i].y);
  glEnd();
}

void drawFloatPolygon (GLfloatPointArray P, colorType c)
{ glColor3fv (c);
  glBegin(GL_LINE_LOOP);
   for (int i=0; i < P.num; i++)
     glVertex2f (P.pt[i].x,P.pt[i].y);
  glEnd();
}

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
    glClearColor(1.0,1.0,1.0,0.0);  // set white background color
	glColor3f (0.0f,0.0f,0.0f);    //default color
    //glPointSize(2.0);		       // a 'dot' is 4 by 4 pixels
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

//<<<<<<<<<<<<<<  drawing subprograms go here


void buildFloatPolygon (GLfloatPointArray &P)
{ P.num = 4;
  P.pt[0].x = 50.0; P.pt[0].y = 50.0;
  P.pt[1].x = 150.0; P.pt[1].y = 100.0;
  P.pt[2].x = 100.0; P.pt[2].y = 300.0;
  P.pt[3].x = 5.0; P.pt[3].y = 400.0;
}

void buildFloatWindow(GLfloatPointArray &W)
{ W.num = 4;
  W.pt[0].x = 15; W.pt[0].y = 20;
  W.pt[1].x = 300; W.pt[1].y = 20;
  W.pt[2].x = 300; W.pt[2].y = 350;
  W.pt[3].x = 15; W.pt[3].y = 350;
}

// Cohen Sutherland Clipping Algorithms for a polygon
typedef unsigned char outcode;
// set bit patterns for masks
enum regions {TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8};

outcode OutCode (float x, float y, float xmin, float xmax,
                 float ymin, float ymax)
{ outcode code = 0;
   if (y > ymax)     code |= TOP;
   else if ( y < ymin) code |= BOTTOM;
   if (x > xmax) code |= RIGHT;
   else if (x < xmin) code |= LEFT;
   return (code);
}

void CSLineClip (float x0, float y0, float x1, float y1,
                 float xmin, float xmax, float ymin, float ymax )
{ outcode outcode0,outcode1,outcode;
   bool accept = false, done = false;
   float x,y;

  outcode0 = OutCode (x0,y0,xmin,xmax,ymin,ymax);
  outcode1 = OutCode (x1,y1,xmin,xmax,ymin,ymax);
     // uses the point slope equation for a straight line
     // y - y0 = m(x-x0)  with m = (y1-y0)/(x1-x0)
     // solves for x or y
  do
  { if ( !(outcode0 | outcode1))           	//trivial case
     { accept = true;
       done  = true;
     }
     else if ( outcode0 & outcode1)
     { done = true;
     }
     else
     {  // chooses the point outside the region
        outcode = outcode0 ? outcode0 : outcode1;

        if (outcode & TOP)
        { x = x0 + (x1-x0)*(ymax-y0)/(y1-y0);
          y = ymax;
        }
        else if (outcode & BOTTOM)
        { x = x0 + (x1-x0)*(ymin-y0)/(y1-y0);
          y = ymin;
        }
        else if (outcode & RIGHT)
        { y = y0 + (y1-y0)*(xmax-x0)/(x1-x0);
          x = xmax;
        }
        else
        { y = y0 + (y1-y0)*(xmin-x0)/(x1-x0);
          x = xmin;
        }
        // reset the new end points
        if (outcode == outcode0)
        { x0 = x;
          y0 = y;
          outcode0 = OutCode (x0,y0,xmin,xmax,ymin,ymax);
        }
        else
        { x1 = x;
          y1 = y;
          outcode1 = OutCode (x1,y1,xmin,xmax,ymin,ymax);
        }
     }
  } while (!done);
  // draw the line segment that is inside the window
  if (accept)
  { glColor3f (1.0f,0.0f,0.0f);
    glBegin(GL_LINES);
	  glVertex2f (x0,y0);
	  glVertex2f (x1,y1);
    glEnd();
  }
}


//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
    GLfloatPointArray Window, Poly;
    colorType polyC = {0.0f,1.0f,0.0f};
    colorType windC = {0.0f,0.0f,1.0f};

    glClear(GL_COLOR_BUFFER_BIT);     // clear the screen
	// 1. create a polygon to clip
    buildFloatPolygon(Poly);
    drawFloatPolygon (Poly,polyC);
	// 2. create a window
    buildFloatWindow (Window);
    drawFloatPolygon (Window,windC);
    // 3. clip the polygon
    for(int i=0; i < Poly.num; i++)
    { int j = (i+1)%Poly.num;
      CSLineClip (Poly.pt[i].x,Poly.pt[i].y,Poly.pt[j].x,Poly.pt[j].y,
          Window.pt[0].x,Window.pt[2].x,Window.pt[0].y,Window.pt[2].y);
    }

    glFlush();
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowSize(640,480);     // set window size
	glutInitWindowPosition(100, 150); // set window position on screen
	glutCreateWindow("Basic clipping"); // open the screen window
	glutDisplayFunc(myDisplay);     // register redraw function
	myInit();
	glutMainLoop(); 		     // go into a perpetual loop
	return 0;
}

