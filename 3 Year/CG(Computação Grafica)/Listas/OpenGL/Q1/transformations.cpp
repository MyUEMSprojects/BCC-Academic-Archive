#include <GL/glut.h>
#include <cmath>

// Globals
float translateX = 0.0f;
float translateY = 0.0f;
float scaleX = 1.0f;
float scaleY = 1.0f;
float rotateAngle = 0.0f;

void drawSquare() {
    glBegin(GL_POLYGON);
        glVertex2f(-0.2f, -0.2f);
        glVertex2f(0.2f, -0.2f);
        glVertex2f(0.2f, 0.2f);
        glVertex2f(-0.2f, 0.2f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Original square (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    drawSquare();

    // Translated square (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
        glTranslatef(translateX, translateY, 0.0f);
        drawSquare();
    glPopMatrix();

    // Scaled square (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
        glScalef(scaleX, scaleY, 1.0f);
        drawSquare();
    glPopMatrix();

    // Rotated square (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
        glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
        drawSquare();
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        // Translation controls
        case 'w': translateY += 0.1f; break;
        case 's': translateY -= 0.1f; break;
        case 'a': translateX -= 0.1f; break;
        case 'd': translateX += 0.1f; break;

        // Scaling controls
        case '+': scaleX *= 1.1f; scaleY *= 1.1f; break;
        case '-': scaleX *= 0.9f; scaleY *= 0.9f; break;

        // Rotation control
        case 'r': rotateAngle += 5.0f; break;
        case 'R': rotateAngle -= 5.0f; break;

        // Reset
        case ' ':
            translateX = translateY = 0.0f;
            scaleX = scaleY = 1.0f;
            rotateAngle = 0.0f;
            break;

        case 27: // ESC
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
                2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
                2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Transformations Example");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}