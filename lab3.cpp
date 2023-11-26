#define GL_SILENCE_DEPRECATION 
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// переменные для управления камерой
float eyex = 0.0f;
float eyey = 0.0f;
float eyez = 5.0f;

// Переменные для управления вращением куба
float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;

// функция отрисовки куба~определение геометрии; мировая система координат
void drawCube() {
    glBegin(GL_QUADS);  // четырехугольник

    // передняя грань
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // задняя грань
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // грань слева
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // грань справа
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // верхняя грань
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // нижняя грань
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

// функция отрисовки сцены
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(eyex, eyey, eyez, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Вращение куба
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);

    drawCube();

    glutSwapBuffers();
}

// функция обработки нажатий клавиш
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            angleX += 5.0f; // Вращение по X вперед
            break;
        case 's':
            angleX -= 5.0f; // Вращение по X назад
            break;
        case 'a':
            angleY += 5.0f; // Вращение по Y влево
            break;
        case 'd':
            angleY -= 5.0f; // Вращение по Y вправо
            break;
        case 'q':
            angleZ += 5.0f; // Вращение по Z по часовой стрелке
            break;
        case 'e':
            angleZ -= 5.0f; // Вращение по Z против часовой стрелки
            break;
    }
    glutPostRedisplay();
}

// функция изменения размеров окна
void reshape(int width, int height) {
    glViewport(0, 0, width, height);    // устанавливаем область вывода (viewport); экранная система координат
    glMatrixMode(GL_PROJECTION);    // видовове преобразование: матрица моделирования -> проекции (то как 3D будет на экране) 
    glLoadIdentity();   // из матрицы проекции -> единичную => чистый лист для преобразований
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);   // перспективная проекция с углом обзора 45
    glMatrixMode(GL_MODELVIEW); // братно на матрицу моделирования для усправления положением и ориентацией объектов в сцене
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // режим отображения с двойной буферизацией, цветовым режимом RGB и включает буфер глубины
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube Viewer");

    glutDisplayFunc(display);   // функция отрисовки сцены
    glutReshapeFunc(reshape);   // обработка изменений размеров окна
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);    // включает тест глубины для правильной отрисовки 3D-сцены

    glutMainLoop();

    return 0;
}
