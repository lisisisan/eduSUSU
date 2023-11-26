#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1.0f}; // цвет тумана

void init() {
    glEnable(GL_DEPTH_TEST);
    
    // включаем туман
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR); // режим тумана – линейный (интенсивность линейно изменяется от начальной до конечной дистанции)
    glFogf(GL_FOG_START, 1.0f); // начало тумана
    glFogf(GL_FOG_END, 5.0f);   // конец тумана
    
    // устанавливаем параметры света и материалов
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // настраиваем отражение
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat mat_ambient[] = {0.7, 0.7, 0.7, 1.0};
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};  // зеркальный свет
    GLfloat high_shininess[] = {100.0}; // высокая степень блеска
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);    // окружающий свет
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);    // диффузный свет
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  // зеркальный свет 
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);   // блики
}

void display() {
    // очищаем буфер цвета и буфер глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // рисуем красный сферический объект
    glColor3f(1.0f, 0.0f, 0.0f);  // устанавливаем цвет для следующих объектов (R, G, B)
    glutSolidSphere(1.0, 20, 20);  // рисуем сферу с радиусом 1.0, используя 20 сегментов по горизонтали и 20 сегментов по вертикали

    // рисуем круг с зеркальными гранями
    glColor3f(0.0f, 0.0f, 1.0f);  // устанавливаем цвет для следующих объектов (R, G, B)
    glPushMatrix();  // сохраняем текущую матрицу модели в стеке
    glTranslatef(-2.0f, 0.0f, 0.0f);  // сдвигаем текущую матрицу модели на (-2.0, 0.0, 0.0)
    glutSolidTorus(0.3, 1.0, 50, 50);  // рисуем тор (кольцо) с внешним радиусом 1.0, внутренним радиусом 0.3, используя 50 сегментов по горизонтали и 50 сегментов по вертикали
    glPopMatrix();  // восстанавливаем предыдущую матрицу модели из стека

    glutSwapBuffers();  // обмениваем буферы (double buffering) для предотвращения мерцания
}


void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 400);
    glutCreateWindow("туман и кольцо");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutMainLoop();
    
    return 0;
}
