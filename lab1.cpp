#define GL_SILENCE_DEPRECATION    // для подавиления предупреждения об устаревании
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <iostream>

using namespace std;

bool vp_valid = true;   // отслеживание действительности (валидности) параметров отображения
int  width, height;

void display(void)
{
    if (!vp_valid)  // действительны ли параметры отображения
    {
        const float aspectRatio = (float)width / height;    //вычисляем соотношение сторон окна
        float sx = aspectRatio > 1.0f ? aspectRatio : 1.0f; // для сохрания пропорции, отсчет идёт от соотношения сторон окна
        float sy = aspectRatio > 1.0f ? 1.0f : 1.0f / aspectRatio;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-sx, sx, -sy, sy, -1.0, 1.0);  // устанавливаем ортографическую проекцию (параллельные линии в трехмерном пространстве остаются параллельными на двумерном экране)
        glViewport(0, 0, width, height);  // установка параметров проекции и видовой порт
    }
    glClear(GL_COLOR_BUFFER_BIT); // очищает буфер цвета

    glBegin(GL_POLYGON); // определяем полигон (=квадрат), путём задания координат для вершин
    glVertex2f(-0.9, -0.9);
    glVertex2f(-0.9, 0.9);
    glVertex2f(0.9, 0.9);
    glVertex2f(0.9, -0.9);
    glEnd();

    glFlush();  // выполняем рисование кадра, отправляя его на экран
}

void reshape(int w, int h) {
    vp_valid = false;
    width = w;
    height = h;
}

void keyboardCB(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:    // Esc
        exit(0);
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) // необходимы для корректной инициализации библиотеки GLUT и управления окном приложения
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   // одиночный буфер (без back buffer) и цвета RGB
    glutInitWindowSize(800, 600);   // начальные размеры окна в пикселях
    glutCreateWindow("Квадрат");    // создаём окно с заголовком "Square"
    glutDisplayFunc(display);       // display будет вызываться каждый раз, когда нужно перерисовать содержимое окна
    glutReshapeFunc(reshape);   // reshape будет вызываться каждый раз, когда размеры окна изменяются
    glutKeyboardFunc(keyboardCB);   
    glutMainLoop();  // запускает основной цикл GLUT, который обрабатывает события в окне, включая отрисовку, изменение размеров окна и события клавиатуры
    return 0;
}
