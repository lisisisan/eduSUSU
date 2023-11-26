#include <cstdio>
#include <iostream>
#define GL_SILENCE_DEPRECATION 
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>

int rotationAngle = 0;      // текущий угол поворота квадрата
bool rotationEnabled = false;   // разрешено ли вращение квадрата в данный момент
float squareX = 0.0f;   // текущее положение по X
float squareY = 0.0f;   // текущее положение по Y

float triangleX = 0.0f;
float triangleY = 0.0f;

using namespace std;

// функция сборки примитива; мировая система координат; определение геометрии
void drawSquare() { // описываем атрибуты примитива
    glBegin(GL_QUADS);  // 4 угла, указан тип примитива
    glVertex2f(-0.1f, -0.1f);   // указываем набор вершин
    glVertex2f(0.1f, -0.1f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(-0.1f, 0.1f);
    glEnd();    // примитив объявлен
}

// определение геометрии треугольника
void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 1.0f, 0.0f);  // зеленый цвет для треугольника
    glVertex2f(0.0f + triangleX, 0.15f + triangleY);
    glVertex2f(-0.1f + triangleX, -0.05f + triangleY);
    glVertex2f(0.1f + triangleX, -0.05f + triangleY);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);   // очищаем буфер кадра и задаём цвет сцены (цвет по умолчанию)
    
    glPushMatrix();     // сохраняем текущую матрицу объектов в стеке матриц
    glTranslatef(squareX, squareY, 0.0f);   // трансляцию (сдвиг) координат квадрата; модельное преобразование
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f); //  вращение объекта (квадрата) вокруг оси Z
    
    glColor3f(1.0f, 0.0f, 0.0f);  // красный цвет для квадрата
    drawSquare();   // отрисовываем квадрат с заданными атрибутами
    
    glPopMatrix();  // для того, чтобы последующие рисования не затронули объекты, которые должны оставаться неподвижными
    
    glColor3f(0.0f, 0.0f, 1.0f);  // синий цвет для другой фигуры
    drawTriangle(); // отрисовываем треуг с заданными атрибутами
    
    glutSwapBuffers();  // вывод на экран всего, что было отрисовано в буфере кадра
}

void timer(int value) {
    if (rotationEnabled) {
        rotationAngle += 2;
        if (rotationAngle >= 360) {
            rotationAngle -= 360;
        }
        glutPostRedisplay();
    }
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        triangleY += 0.01f;
    } else if (key == 's' || key == 'S') {
        triangleY -= 0.01f;
    } else if (key == 'a' || key == 'A') {
        triangleX -= 0.01f;
    } else if (key == 'd' || key == 'D') {
        triangleX += 0.01f;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            squareX -= 0.01f;
            break;
        case GLUT_KEY_RIGHT:
            squareX += 0.01f;
            break;
        case GLUT_KEY_UP:
            squareY += 0.01f;
            break;
        case GLUT_KEY_DOWN:
            squareY -= 0.01f;
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        rotationEnabled = !rotationEnabled;
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        rotationEnabled = false;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // устанавливаем двойной буфер + используем RGB
    glutInitWindowSize(800, 600);   // объявляем окно для рисования
    glutCreateWindow("Rotating Square");    // создаем его

    glutDisplayFunc(display);   // отображаем сцену
    glutKeyboardFunc(keyboard); // обработка событий клавиатуры
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    
    glutTimerFunc(0, timer, 0); // для анимации (каждые 16 мс)

    glMatrixMode(GL_PROJECTION);    // устанавливаем текущую матрицу – матрицу проекции (как из 3D в)
    glLoadIdentity();   // обнуляем любые предыдущие преобразования (единичная матрица)
    glOrtho(-1, 1, -1, 1, -1, 1);   // ортографическая проекция (экранные координаты = мировые)
    glMatrixMode(GL_MODELVIEW); // матрица для определения положения и ориентации объектов в сцене
    
    glutMainLoop();     // все в бесконечном цикле
    
    return 0;
}
