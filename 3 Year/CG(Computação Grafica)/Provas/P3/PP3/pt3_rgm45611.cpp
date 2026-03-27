#include <GL/glut.h>
#include <stdio.h>

// Nome: Felipe Echeverria Vilhalva RGM: 45611


/* 12(recorte) - Estenda o algoritmo Sutherland-Hodgeman disponível no site da disciplina, que está limitado a
apenas polígonos convexos, para que considere o recorte de qualquer polígono (inclusive polígonos côncavos).
*/

struct Point{
    float x,y;
} w[4];

#define MAX_VERTICES 24

int Nin; // conta os vertices de entrada

void drawLine(Point p[],int n){
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<n;i++)
        glVertex2f(p[i].x,p[i].y);
    glEnd();
}
void drawPoly(Point p[],int n){
    glBegin(GL_POLYGON);
    for(int i=0;i<n;i++)
        glVertex2f(p[i].x,p[i].y);
    glEnd();
}

// Verifica se um ponto P está dentro da área de visualização definida
bool insideVer(Point p, int i){
        if(i==0 && p.x>=w[i].x)
              return true;
        else if (i==1 && p.y<=w[i].y)
              return true;
        else if (i==2 && p.x<=w[i].x)
                return true;
        else if (i==3 && p.y>=w[i].y)
                return true;
        return false;
}

// Calcula o ponto de interseção entre o segmento de polígono S→P e a aresta de recorte
Point getInterSect(Point s,Point p,int edge){
    Point in;
    float m;
    if (p.x==s.x)
       m=90000.0; /* cuidando a divisao por zero quando o segmento for vertical*/
    else
       m=(p.y-s.y)/(p.x-s.x);
    if(w[edge].x==w[(edge+1)%4].x){ //Vertical Line
        in.x=w[edge].x;
        in.y=(in.x-s.x)*m+s.y;
    }
    else{//Horizontal Line
        in.y=w[edge].y;
        in.x=((in.y-s.y)/m)+s.x;
    }
    return in;
}

// N_in é o numero de vertices
int clipPolygon(Point inVer[], int N_in, Point outVer[MAX_VERTICES]){

    // Buffers locais, garante que pode ser chamado varias vezes em sequencia
    Point bufferA[MAX_VERTICES]; // armazena entrada fase recorte
    Point bufferB[MAX_VERTICES]; // armazena saida

    // Copia a entrada inicial para o buffer principal
    for(int i=0; i < N_in; i++){
        bufferA[i] = inVer[i];
    }
    int N_current = N_in;

    Point *currentIn = bufferA;
    Point *currentOut = bufferB;

    // O loop sobre as 4 arestas de recorte
    for(int i=0; i<4; i++)
    {
        int Nout = 0;
        Point s = currentIn[N_current-1];

        // Sutherland-Hodgman para cada um das 4 arestas da janela
        for(int j=0; j < N_current; ++j){
            Point p = currentIn[j];

            bool p_inside = insideVer(p, i);
            bool s_inside = insideVer(s, i);

            if(p_inside){
                 if(s_inside){
                    currentOut[Nout++] = p;
                } else {
                    Point interSec = getInterSect(s, p, i);
                    currentOut[Nout++] = interSec;
                    currentOut[Nout++] = p;
                }
            } else {
                if(s_inside){
                    Point interSec = getInterSect(s, p, i);
                    currentOut[Nout++] = interSec;
                }
            }
            s = p;
        }

        // troca os buffers para a proxima iteracao e att o contador
        N_current = Nout; // Nout é o numero de vertices gerados
        Point *temp = currentIn;
        currentIn = currentOut; // inverte os ponteiros(currentIn se torna  o novo currentOut)
        currentOut = temp; //

        // se o poligono ja foi TOTALMENTE recortado
        if (N_current == 0) break;
    }

    // copia o resultado final do buffer de saida para o array outVer fornecido
    for (int j=0; j < N_current; j++){
        outVer[j] = currentIn[j];
    }
    return N_current;
}

void init(){
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,120.0,0.0,120.0,0.0,100.0);
    glClear(GL_COLOR_BUFFER_BIT);
    w[0].x =20,w[0].y=10;
    w[1].x =20,w[1].y=80;
    w[2].x =80,w[2].y=80;
    w[3].x =80,w[3].y=10;
}
void display(void){
    init();

    // poligono concavo - 5 vertices
    Point concavePoly[] = {
        {30, 40}, // P0
        {10, 60}, // P1
        {50, 30}, // P2
        {90, 60}, // P3
        {70, 40}  // P4
    };
    int N_concave = 5;

    // decomposicao em triangulos convexos
    Point T1[] = {concavePoly[0], concavePoly[1], concavePoly[2]};
    Point T2[] = {concavePoly[0], concavePoly[2], concavePoly[3]};
    Point T3[] = {concavePoly[0], concavePoly[3], concavePoly[4]};

    Point *triangles[] = {T1, T2, T3};
    int N_triangles = 3;

    // As Window for Clipping
    glColor3f(1.0f, 0.0f, 0.0f);
    drawPoly(w, 4);

    // linha verde
    glColor3f(0.0f, 1.0f, 0.0f);
    drawLine(concavePoly, N_concave);

    // recorte para cada triangulo decomposto em azul
    Point clippedResult[MAX_VERTICES];
    glColor3f(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < N_triangles; i++) {
        int N_clipped;
        Point *currentTriangle = triangles[i]; // itera para cada triangulo

        Nin = 3;

        N_clipped = clipPolygon(currentTriangle, 3, clippedResult);

        // desenha o resultado final se o poligono resultante for maior que 0
        if (N_clipped > 0) {
            drawPoly(clippedResult, N_clipped);
        }
    }

    glFlush();
}

int main(int argc,char *argv[]){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Polygon Clipping!");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}