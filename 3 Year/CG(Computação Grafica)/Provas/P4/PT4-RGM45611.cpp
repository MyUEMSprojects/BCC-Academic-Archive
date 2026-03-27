#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// Nome: Felipe Echeverria Vilhalva - Computação gráfica 2025 - Exercicio 7(2 da L43) da PT4

// configs do ambiente
const float BOX_SIZE = 15.0f;
const float GRAVITY = 0.0f;   // gravidade zero
const float DRAG = 0.99f;     // atrito da bola

struct Vec3 { double x, y, z; }; // 3D

struct Sphere {
    Vec3 center;
    Vec3 velocity;
    double radius;

    float r, g, b;
    float oR, oG, oB;

    bool isMoving; // verificar se esta se movendo(instancia da esfera)
};

// variaveis globais
int width = 800;
int height = 600;
std::vector<Sphere> spheres;

enum State {
    STATE_SELECT,
    STATE_AIM
};
State currentState = STATE_SELECT;

int selectedSphereIdx = -1;
Vec3 targetPos;

float camDist = 50.0f;
float camAlpha = 0.5f;
float camBeta = 0.3f;
int lastMouseX, lastMouseY;
bool isRotating = false;

// matematica vetorial
Vec3 sub(Vec3 a, Vec3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3 add(Vec3 a, Vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 scale(Vec3 v, double s) { return {v.x * s, v.y * s, v.z * s}; }
double dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double length(Vec3 v) { return sqrt(dot(v, v)); }

// transforma qualquer vetor em tamanho 1 - para deixar apenas a direção
Vec3 normalize(Vec3 v) {
    double len = length(v);
    if (len == 0) return {0,0,0};
    return scale(v, 1.0 / len);
}

// distancia entre dois pontos
double distSq(Vec3 a, Vec3 b) { Vec3 d = sub(a, b); return dot(d, d); }

// orocedimento para adicionar esfera salvando a cor original
void addSphere(float x, float y, float z, float r, float red, float green, float blue) {
    Sphere s;
    s.center = {x, y, z};
    s.velocity = {0,0,0};
    s.radius = r;

    // Define cor atual e original iguais no inicio
    s.r = red; s.g = green; s.b = blue;
    s.oR = red; s.oG = green; s.oB = blue;

    s.isMoving = false;
    spheres.push_back(s);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat light_pos[] = { 0.0, 20.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // criação das esferas (12)
    //         X     Y     Z    Raio   R    G    B
    addSphere( 0.0, -5.0,  0.0, 2.0,  1.0, 0.0, 0.0); // Vermelho
    addSphere( 5.0,  5.0,  5.0, 1.5,  0.0, 1.0, 0.0); // Verde
    addSphere(-5.0,  0.0, -5.0, 1.5,  0.0, 0.0, 1.0); // Azul
    addSphere( 5.0, -5.0, -5.0, 1.5,  1.0, 1.0, 0.0); // Amarelo

    addSphere(-8.0,  8.0,  8.0, 1.2,  0.0, 1.0, 1.0); // Ciano
    addSphere( 8.0, -8.0,  2.0, 1.8,  1.0, 0.0, 1.0); // Magenta
    addSphere( 0.0, 10.0,-10.0, 1.5,  0.5, 0.5, 0.5); // Cinza
    addSphere(-10.0,-5.0,  5.0, 1.3,  1.0, 0.5, 0.0); // Laranja

    addSphere( 2.0,  2.0, -2.0, 1.0,  0.2, 0.8, 0.2); // Verde Lima
    addSphere(-4.0,  8.0, -2.0, 1.6,  0.5, 0.0, 0.5); // Roxo
    addSphere( 6.0,  0.0,  8.0, 1.4,  0.0, 0.5, 1.0); // Azul Claro
    addSphere(-8.0, -8.0, -8.0, 1.5,  0.8, 0.2, 0.2); // Salmão
}

// Ray casting - A função projeta dois pontos no espaço 3D.
void getRay(int mouseX, int mouseY, Vec3 &origin, Vec3 &direction) {
    GLdouble model[16], proj[16];
    GLint view[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);

    double winX = (double)mouseX;
    double winY = (double)view[3] - (double)mouseY; // windows
    Vec3 start, end;
    // z = 0(near); z = 1(far)
    gluUnProject(winX, winY, 0.0, model, proj, view, &start.x, &start.y, &start.z);
    gluUnProject(winX, winY, 1.0, model, proj, view, &end.x, &end.y, &end.z);
    origin = start;
    // end - start = vetor direção(o raio) que sai do olho da camera e entra na cena
    direction = normalize(sub(end, start)); // normalizar para ser apenas uma direção
}

// calcular a que distancia(onde) um raio imaginario fura um plano infinito
double intersectPlane(Vec3 origin, Vec3 dir, Vec3 planeNormal, double planeD) {

    // denominador
    double denom = dot(dir, planeNormal);
    // evia divisão por zero
    // se for zero é paralelo ao plano(ignorar) - nao há intersecção
    if (abs(denom) > 1e-6) {
        return -(dot(origin, planeNormal) + planeD) / denom; // retorna onde o raio toca na parede
    }
    return -1.0;
}

// intesecção a caixa(Raio x caixa)
// sistema de mira (serve pra mira nao sair da caixa)
// ro - ray origin; rd - ray direction
Vec3 getBoxIntersection(Vec3 ro, Vec3 rd) {
    double minT = 99999.0;
    bool hit = false;
    struct Plane { Vec3 n; double d; };
    Plane planes[] = {
        // {1,0,0}, -BOX_SIZE: Parede da Direita
        // {-1,0,0}, -BOX_SIZE: Parede da Esquerda.
        // {0,1,0}... (teto), {0,-1,0}... (Chao), ...
        {{1,0,0}, -BOX_SIZE}, {{-1,0,0}, -BOX_SIZE},
        {{0,1,0}, -BOX_SIZE}, {{0,-1,0}, -BOX_SIZE},
        {{0,0,1}, -BOX_SIZE}, {{0,0,-1}, -BOX_SIZE}
    };
    // verifica(itera) se o raio bate no plano das paredes(faces)
    for(int i=0; i<6; i++) {
        // qual parede o mouse esta apontando
        // no caso intersectPlane a reta esta indo pro infinito
        // essa função corta isso
        double t = intersectPlane(ro, rd, planes[i].n, planes[i].d);
        if (t > 0) {
            Vec3 p = add(ro, scale(rd, t));
            // verifica se o ponto P esta dentro do quadrado da parede
            if (p.x >= -BOX_SIZE-0.5 && p.x <= BOX_SIZE+0.5 &&
                p.y >= -BOX_SIZE-0.5 && p.y <= BOX_SIZE+0.5 &&
                p.z >= -BOX_SIZE-0.5 && p.z <= BOX_SIZE+0.5) {
                // gurardar a parede q esta visivel
                if (t < minT) { minT = t; hit = true; }
            }
        }
    }
    
    if (hit) return add(ro, scale(rd, minT));
    return {0,0,0};
}

// intersecção a esfera(Raio x esfera)
// decide se o clique do mouse acertou ou errou a esfera(bola)
// calcule a distância d do centro de cada esfera R até o raio usando a distância ponto-reta
bool raySphereIntersect(Vec3 rayOrigin, Vec3 rayDir, Sphere s, double &distOut) {
    // vetor q vai da ponta da linha até o centro da bola
    Vec3 oc = sub(s.center, rayOrigin);
    // projeta  o vetor do centro na linha do raio
    double t = dot(oc, rayDir);
    // ponto mais proximo do centro da esfera
    Vec3 closestPoint = add(rayOrigin, scale(rayDir, t));
    // calcula a distancia do centro da bola
    double d_sq = distSq(s.center, closestPoint); // dist ao quadrado
    double r_sq = s.radius * s.radius; // raio ao quadrado
    // se dist(d) for menor q o tamanho da bola(raio) - houve colisão
    if (d_sq < r_sq && t > 0) { // t > 0 verifica se a bola esta na frente da camera
        distOut = t;
        return true;
    }
    return false;
}

// fisica
void update(int value) {
    for (size_t i = 0; i < spheres.size(); i++) {
        // se ismoving for true(esfera se mexendo) calcula a fisica
        if (spheres[i].isMoving) {
            // Posição Final = Posição Inicial + Velocidade
            spheres[i].center = add(spheres[i].center, spheres[i].velocity);
            // deixar com efeito de gravidade zero(ou d baixo dagua)
            spheres[i].velocity = scale(spheres[i].velocity, DRAG);

            // a esfera(bola) nao esta se mexendo("esta parando")
            if (length(spheres[i].velocity) < 0.005) {
                spheres[i].isMoving = false;
                spheres[i].velocity = {0,0,0};
            }

            // Colisoes com a caixa(paredes)
            if (spheres[i].center.x + spheres[i].radius > BOX_SIZE) { // verifica se batei na direita
                spheres[i].center.x = BOX_SIZE - spheres[i].radius; // 
                spheres[i].velocity.x *= -0.8; // inverte a velocidade(diminui)
            } else if (spheres[i].center.x - spheres[i].radius < -BOX_SIZE) {
                spheres[i].center.x = -BOX_SIZE + spheres[i].radius;
                spheres[i].velocity.x *= -0.8;
            }

            if (spheres[i].center.y + spheres[i].radius > BOX_SIZE) { // msm coisa...
                spheres[i].center.y = BOX_SIZE - spheres[i].radius;
                spheres[i].velocity.y *= -0.8;
            } else if (spheres[i].center.y - spheres[i].radius < -BOX_SIZE) {
                spheres[i].center.y = -BOX_SIZE + spheres[i].radius;
                spheres[i].velocity.y *= -0.8;
            }

            if (spheres[i].center.z + spheres[i].radius > BOX_SIZE) {
                spheres[i].center.z = BOX_SIZE - spheres[i].radius;
                spheres[i].velocity.z *= -0.8;
            } else if (spheres[i].center.z - spheres[i].radius < -BOX_SIZE) {
                spheres[i].center.z = -BOX_SIZE + spheres[i].radius;
                spheres[i].velocity.z *= -0.8;
            }

            // Colisoes Esfera x Esfera
            for (size_t j = 0; j < spheres.size(); j++) {
                if (i == j) continue;
                double dist = sqrt(distSq(spheres[i].center, spheres[j].center));
                double minSeparation = spheres[i].radius + spheres[j].radius; // R1 + R2

                // dis < R1 + R2
                // fisica de reflexão(a bola rebate a outra)
                if (dist < minSeparation) {
                    // vetor normal(direção da batida) - linha q liga os dois centros
                    Vec3 normal = normalize(sub(spheres[i].center, spheres[j].center));
                    // reflete a velocidade da bola i na normal
                    double v_dot_n = dot(spheres[i].velocity, normal);
                    // formula de reflexao especular
                    Vec3 reflection = sub(spheres[i].velocity, scale(normal, 2 * v_dot_n));
                    spheres[i].velocity = scale(reflection, 0.8);

                    double overlap = minSeparation - dist;
                    spheres[i].center = add(spheres[i].center, scale(normal, overlap + 0.05));
                    
                    // bola parada ganha velocidade se outra colidir com ela
                    spheres[j].velocity = scale(normal, -length(spheres[i].velocity) * 0.8);
                    spheres[j].isMoving = true;
                }
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// input
void mouse(int button, int state, int x, int y) {
    lastMouseX = x;
    lastMouseY = y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (isRotating) return;

        Vec3 rayOrigin, rayDir;
        getRay(x, y, rayOrigin, rayDir);


        // esfera selecionada(pinta de amarelo) e muda para o estado de mira
        if (currentState == STATE_SELECT) {
            double minDesc = 99999.0;

            // indice da esfera selecionada
            int hitIdx = -1;
            for (size_t i = 0; i < spheres.size(); i++) {
                double t;
                if (raySphereIntersect(rayOrigin, rayDir, spheres[i], t)) {
                    if (t < minDesc) { minDesc = t; hitIdx = i; }
                }
            }
            // achou a esfera(hit)? muda pro estado de mira
            if (hitIdx != -1) {
                selectedSphereIdx = hitIdx;
                currentState = STATE_AIM;
                // Muda cor para Amarelo ao selecionar (mas não perde a original)
                spheres[hitIdx].r = 1.0; spheres[hitIdx].g = 1.0; spheres[hitIdx].b = 0.0;
            }
        }
        else if (currentState == STATE_AIM) {
            // vetor da mira (direção)
            Vec3 aimDir = sub(targetPos, spheres[selectedSphereIdx].center);
            double force = length(aimDir) * 0.03; // força da mira
            if (force > 1.0) force = 1.0; // limite

            spheres[selectedSphereIdx].velocity = scale(normalize(aimDir), force);
            spheres[selectedSphereIdx].isMoving = true;

            // restaura a cor original guardada (oR, oG, oB) dps de soltar
            spheres[selectedSphereIdx].r = spheres[selectedSphereIdx].oR;
            spheres[selectedSphereIdx].g = spheres[selectedSphereIdx].oG;
            spheres[selectedSphereIdx].b = spheres[selectedSphereIdx].oB;

            currentState = STATE_SELECT; // volta pro estado original, pronto pra selecionar outra bola
            selectedSphereIdx = -1;
        }
    }
    // cancela a mira
    if (button == GLUT_RIGHT_BUTTON) {
        isRotating = (state == GLUT_DOWN);
        // Se cancelar mira com botão direito, restaura cor original tbm
        if(state == GLUT_DOWN && currentState == STATE_AIM) {
             currentState = STATE_SELECT;
             if(selectedSphereIdx != -1) {
                spheres[selectedSphereIdx].r = spheres[selectedSphereIdx].oR;
                spheres[selectedSphereIdx].g = spheres[selectedSphereIdx].oG;
                spheres[selectedSphereIdx].b = spheres[selectedSphereIdx].oB;
             }
             selectedSphereIdx = -1;
        }
    }
}

// rotação da mira
void passiveMotion(int x, int y) {
    if (currentState == STATE_AIM) {
        Vec3 rayOrigin, rayDir;
        getRay(x, y, rayOrigin, rayDir);
        targetPos = getBoxIntersection(rayOrigin, rayDir);
        glutPostRedisplay();
    }
}


// rotação da camera
void motion(int x, int y) {
    if (isRotating) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        camAlpha += dx * 0.01f;
        camBeta  += dy * 0.01f;
        if (camBeta > 1.5f) camBeta = 1.5f;
        if (camBeta < -1.5f) camBeta = -1.5f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void drawBox() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutWireCube(BOX_SIZE * 2);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // camera
    float camX = camDist * sin(camAlpha) * cos(camBeta);
    float camY = camDist * sin(camBeta);
    float camZ = camDist * cos(camAlpha) * cos(camBeta);
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // desenha as esferas
    for (size_t i = 0; i < spheres.size(); i++) {
        glPushMatrix();
        glTranslatef(spheres[i].center.x, spheres[i].center.y, spheres[i].center.z);
        glColor3f(spheres[i].r, spheres[i].g, spheres[i].b);
        glutSolidSphere(spheres[i].radius, 30, 30);
        glPopMatrix();
    }

    // desenha caixa
    drawBox();

    // desenha a linha(mira)
    if (currentState == STATE_AIM && selectedSphereIdx != -1) {
        glDisable(GL_LIGHTING);
        glColor3f(1.0, 1.0, 0.0);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        Vec3 c = spheres[selectedSphereIdx].center;
        glVertex3f(c.x, c.y, c.z);
        glVertex3f(targetPos.x, targetPos.y, targetPos.z);
        glEnd();

        glPushMatrix();
        glTranslatef(targetPos.x, targetPos.y, targetPos.z);
        glutSolidSphere(0.2, 10, 10);
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    width = w; height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("PT4 - Exercicio 7 (Caixa e esferas)");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutTimerFunc(0, update, 0);

    printf("--- CONTROLES ---\n");
    printf("1. Clique para SELECIONAR.\n");
    printf("2. Mire e clique para LANCAR.\n");
    printf("3. Botao DIREITO para girar a camera.\n");

    glutMainLoop();
    return 0;
}