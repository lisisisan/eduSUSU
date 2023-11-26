#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <stdlib.h>
#include <array>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include "imageloader.h"
#include <array>
#include <math.h>

float x1, x2 = 2.0;

GLfloat light_position[] = {x1, x2, 1.0, 0.0};
// GLfloat light_position2[] = {-1.0, -1.0, 1.0, 0.0};
GLfloat light_diffuse[] = {0.0, 1.0, 1.0, 1.0};     
GLfloat material_diffuse[] = {0.7, 0.7, 0.7, 1.0};  // цвет рассеивающегося света, меняются свойства материала
GLfloat material_specular[] = {1.0, 1.0, 1.0, 1.0}; // цвет зеркальных бликов
GLfloat material_shininess[] = {100.0}; // степень зеркальной отражательной способностиы
GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };

std::array<GLuint, 4> gluintArr;

bool corner = false;
//GLuint tex;
GLUquadric* sphere;

float yaw = 0, pitch = 0,radius = 15;

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);   // генерируется 1 текстура с идентификатором textureId
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // по горизонатльной оси
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // по вертикальной оси
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image->width, image->height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
    return textureId;
}

void initRendering() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    // glLightfv(GL_LIGHT0, GL_POSITION, light_position); // точечный источник света

    // glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);   // блики

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); // прожектор
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    
    Image* image = loadBMP("image.bmp");
    gluintArr[0] = loadTexture(image);
    sphere = gluNewQuadric();
    

    //glEnable(GL_TEXTURE_2D);
    delete image;
}


float xRotated = 90.0, yRotated = 90.0, zRotated = 0.0;

void reshapeFunc (int x, int y)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective (40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
    glMatrixMode   (GL_MODELVIEW);
    glViewport     (0, 0, x, y);
}

void DrawSphereWithTexture(){
    gluQuadricDrawStyle(sphere, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, gluintArr[0]);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, 2.0, 32, 16);
}

void display (void)
{
    glMatrixMode   (GL_MODELVIEW);
    glClear        (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();
    // определения положения камеры
    gluLookAt
        (
            radius*sin(pitch)*sin(yaw), 
            radius*cos(pitch), 
            radius*sin(pitch)*cos(yaw),
            0, 0, 0,    // точка, куда направлена камера
            0, 0, 1
        );
    glEnable(GL_TEXTURE_2D);
    

    DrawSphereWithTexture();
    
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}
 
void idleFunc (void)
{
    zRotated += 0.3;
    glutPostRedisplay();
}
 

void specialKeys( int key, int x, int y )
{
    if(key == GLUT_KEY_RIGHT){
        
        pitch += 0.1;
    }
    if(key == GLUT_KEY_LEFT){
     
        pitch -= 0.1;
    }
    if(key == GLUT_KEY_UP){
    
        yaw+= 0.1;
    }
    if(key == GLUT_KEY_DOWN){
        if(yaw > -9)
        yaw-= 0.1;
    }
    
    glutPostRedisplay();
}

int main (int argc, char **argv)
{
    glutInit               (&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize     (800, 700);
    glutCreateWindow       ("вращение сферы");
 
    glClearColor (0.0, 0.0, 0.0, 0.0);
    
    initRendering();
    
    glutDisplayFunc (display);
    glutReshapeFunc (reshapeFunc);
    glutSpecialFunc( specialKeys );

    glutMainLoop();
    return  0;
}

