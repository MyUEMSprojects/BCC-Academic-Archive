#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>

using namespace std;

#define HEIGHT 480
#define WIDTH 640

int keyboard_event_x = 0;
int keyboard_event_y = 0;
double objx = 0, objy=0, objz=0;
double angle_x = 0, angle_y = 0;
double move_x = 0;
double move_y = 0;
double move_z = 0;
int move_cube = 0;
/*THE FUNCTION TO DRAW THE STUFF ON THE SCREEN*/
void display( )
{
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    int offset = 10;
    //The big green floor like polygon
    //The big green floor like polygon
    //The big green floor like polygon
    if (keyboard_event_x == 1){
        glRotatef(angle_x, 0.0, 1.0, 0.0);

    }
    else if (keyboard_event_y == 1){
        //glRotatef(angle_x, 0.0, 1.0, 0.0);
        glRotatef(angle_y, 1.0, 0.0, 0.0);
    }
    //glRotatef(angle_y, 1.0, 0.0, 0.0);
    glPushMatrix();
    glBegin( GL_LINES );
        glColor3f(0, 0, 1);
        glVertex3f( 0, 0,  0  );
        glVertex3f( 0, 0, -1000  );
    glEnd( );
    glBegin( GL_LINES );
        glColor3f(1, 0, 0);
        glVertex3f( 0, 0,  0  );
        glVertex3f( 800, 0,  0  );
    glEnd( );
    glColor3f(0.8, 0.8, 0.8);
    for (int i = 0; i < 1700 ; i+=100){
        glBegin( GL_LINES );
        glVertex3f( -800+i, 0,  1000  );
        glVertex3f( -800+i, 0, -1000  );
        glEnd( );
    }
    //glColor3f(0, 1, 0);
    for (int i = 0; i < 2200 ; i+= 200){
        glBegin( GL_LINES );
        glVertex3f( -800, 0,  1000-i  );
        glVertex3f(  800, 0,  1000-i  );
        glEnd( );
    }

    //The red cube to be drawn at clicked position
    glColor3f(0.8, 0.8, 0.8);
    glPushMatrix( );
        glColor3f( 1, 0, 0 );
        glTranslatef( objx, objy, objz );
        glTranslatef(move_x, move_y, move_z);
        glBegin( GL_LINES );
        glColor3f(0, 0, 1);
        glVertex3f( 0, 0,  0  );
        glVertex3f( 0, 0, 200  );
        glEnd( );
        glBegin( GL_LINES );
        glColor3f(1, 0, 0);
        glVertex3f( 0, 0,  0  );
        glVertex3f( 200, 0,  0  );
        glEnd( );
        glBegin( GL_LINES );
        glColor3f(0, 1, 0);
        glVertex3f( 0, 0,  0  );
        glVertex3f( 0, 200,  0  );
        glEnd( );
        glutSolidCube( 85 );
    glPopMatrix( );


    glPopMatrix();
    glFlush( );
    glutSwapBuffers( );
}

void mouse( int button, int state, int x, int y)
{
    double mvmatrix[16], projmatrix[16];
    int viewport[4];
    float z = 1 - 0.0001;
    //float z = 1 - 0.001;
    GLint realy;  /*  OpenGL y coordinate position  */
   GLdouble wx, wy, wz;  /*  returned world x, y, z coords  */
   GLdouble px, py, pz;  /*  returned world x, y, z coords  */

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {

            glGetIntegerv (GL_VIEWPORT, viewport);
            glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
            glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
/*  note viewport[3] is height of window in pixels  */
            realy = viewport[3] - (GLint) y;
            printf ("\nCoordinates at cursor are:\n(%4d, %4d)\n", x, realy);
            gluUnProject ((GLdouble) x, (GLdouble) realy, 0.0,
               mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
            printf ("World coords at z=0.0 (near plane) are:\n(%f, %f, %f)\n",
               wx, wy, wz);
            gluProject((GLdouble) wx, (GLdouble) wy, wz,
               mvmatrix, projmatrix, viewport, &px, &py, &pz);
            printf ("Projected coords of near plane on screen :\n(%f, %f, %f)\n\n",
               px, py, pz);
            gluUnProject ((GLdouble) x, (GLdouble) realy, 1.0,
               mvmatrix, projmatrix, viewport, &objx, &objy, &objz);
            printf ("World coords at z=1.0 (far plane) are:\n(%f, %f, %f)\n",
               objx, objy, objz);
            gluProject((GLdouble) objx, (GLdouble) objy, objz,
               mvmatrix, projmatrix, viewport, &px, &py, &pz);
            printf ("Projected coords of far plane are:\n(%f, %f, %f)\n",
               px, py, pz);
            keyboard_event_y = 0;
            keyboard_event_x = 0;
            move_x = 0;
            move_y = 0;
            move_z = 0;
        }
    }
    glutPostRedisplay();
}
void keyboard( int key, int x, int y )
{


    if (key == GLUT_KEY_RIGHT){
        keyboard_event_x = 1;
        keyboard_event_y = 0;
        angle_x=2.0;
        glutPostRedisplay();
        //cout << "aqui" << endl;
    }

    else if(key == GLUT_KEY_LEFT){
        keyboard_event_x = 1;
        keyboard_event_y = 0;
        angle_x=-2.0;
        glutPostRedisplay();
        //cout << "aqui-1" << endl;
    }

    else if(key == GLUT_KEY_UP){
        keyboard_event_y = 1;
        keyboard_event_x = 0;
        angle_y=2.0;
        glutPostRedisplay();
        //cout << "aqui-3" << endl;
    }

    else if (key ==  GLUT_KEY_DOWN) {
        keyboard_event_y = 1;
        keyboard_event_x = 0;
        angle_y=-2.0;
        glutPostRedisplay();
        //cout << "aqui-4" << endl;
    }

}

void keyInput(unsigned char key, int x, int y)
{
    keyboard_event_y = keyboard_event_x = 0;
    switch (key) {
        case 'x':
            move_x+=1;
            cout << "dentro" << endl;
            break;
        case 'X':
            move_x-=1;
            break;
        case 'y':
            move_y+=1;
            break;
        case 'Y':
            move_y-=1;
            break;
        case 'z':
            move_z+=1;
            break;
        case 'Z':
            move_z-=1;
            break;

  }
  glutPostRedisplay();
}

void init( int width, int height )
{
        glClearColor( 0.3, 0.3, 0.3, 1 );
        glViewport( 0, 0, width, height );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
//        gluPerspective( 45, 1.33, 0.1, 400 );
// Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)width, 0.0, (float)height, 1, 1200.0);
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );
        gluLookAt( 0, 0, 250, 0, 0, 0, 0, 1, 0 );
}

int main( int argc, char **argv )
{
        glutInit( &argc, argv );
        //The most important part specify the things your
        //glut window should provide
        glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
        glutInitWindowSize( 1280, 690 );
        glutInitWindowPosition(100, 2);
        glutCreateWindow("Teste usando gluUnProject");

        //enable z buffer
        glEnable( GL_DEPTH_TEST );
        //set the value in z-buffer as 1.0
        glClearDepth( 1.0 );
        init( 1280, 690 );
        glutDisplayFunc( display );
        glutReshapeFunc( init );
        //glutIdleFunc( display );
        glutMouseFunc( mouse );
        glutKeyboardFunc(keyInput);
        glutSpecialFunc( keyboard );
        glutMainLoop( );
}
