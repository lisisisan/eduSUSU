#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <stdlib.h>
#include <array>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION	// включен с реализацией, а не только с объявлениями
#include "stb_image.h"  // для загрузки изображений в текстуры

#define PI 3.14159265358979323846


float windowWidth = 800;
float windowHeight = 600;

void init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
GLuint im_load(const char *image_file_path);

void changePerspective();
void toggle();

float cameraRadius = 5;
float cameraAngleX = 0;
float cameraAngleY = 0;
float cameraX = 0;
float cameraY = 1;
float cameraZ = 5;

bool left = false;
bool right = false;
bool up = false;
bool down = false;

bool isPerspective = true;

void loadTextures();
void drawCube();

GLuint chocolateTextureID;
GLuint glassTextureID;
GLuint brickTextureID;
GLuint textureID;
GLuint texture2ID;
GLuint texture3ID;


int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);


  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("textured_cube");

  cameraAngleX = 1.0f;
  cameraAngleY = 1.0f;
  cameraRadius = 5.0f;

  init();
  loadTextures();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutMainLoop();
  return 0;
}

void init() {
	glEnable(GL_DEPTH_TEST);	// установка теста глубины для отрисовки
	glShadeModel(GL_SMOOTH);	// модель затенения GL_SMOOTH (цвет каждого фрагмента будет интерполирован между вершинами)
	// glClearColor(0, 0, 0, 0.5);	// каким цветом будет заполнен экран, когда вызывается glClear(
	glClearDepth(1.0);	// вся область глубины будет очищена до максимальной глубины
	changePerspective();	// настройка матрици проекции для отображения сцены
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// очищаем прошлое
	glMatrixMode(GL_MODELVIEW);	// последующие преобразования будут применяться к матрице модели и матрице вида
	glLoadIdentity();	// текущую матрицу модели-вида в единичную матрицу (камера вдоль оси Z)

	// преобразование сферических координат (радиус и два угла) в декартовы(x, y, z)
	cameraX = cameraRadius * sin(cameraAngleY) * cos(cameraAngleX);
	cameraY = cameraRadius * cos(cameraAngleY);
	cameraZ = cameraRadius * sin(cameraAngleY) * sin(cameraAngleX);

	int upY = 1;
	if ((cameraAngleY < -PI && cameraAngleY >= -2 * PI) ||
		(cameraAngleY > 0 && cameraAngleY <= PI)) {
		upY = -1;	// камера смотрит вниз
	}

	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, upY, 0);
	// glColor3f(0, 1, 0);	// текущий цвет рисования
	drawCube();	// отрисовка куба

	glutSwapBuffers();	// смена буферов для плавного ототбражения
}

void drawCube() {
	glEnable(GL_TEXTURE_2D);	// включаем текстуры
	glBindTexture(GL_TEXTURE_2D, chocolateTextureID);

	glColor3f(1, 1, 1);	// цвет рисования белый
	glBegin(GL_QUADS);
	// передняя грань
	glTexCoord2f(0, 0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1, 0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1, 1);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0, 1);
	glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	// задняя грань с дублированием текстуры
	glBindTexture(GL_TEXTURE_2D, glassTextureID);	// теперь с этой текстурой
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(4, 0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(4, 4);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0, 4);
	glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	// тоже дублирование
	glBindTexture(GL_TEXTURE_2D, brickTextureID);
	float repeatFactor = 4.0f;	// отображаем 4 раза в и по Х, и по У
	glBegin(GL_QUADS);
	// левая грань
	glTexCoord2f(0, 0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(repeatFactor, 0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(repeatFactor, repeatFactor);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(0, repeatFactor);
	glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	// правая грань
	glTexCoord2f(0, 0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1, 0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1, 1);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0, 1);
	glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture2ID);
	glBegin(GL_QUADS);
	// верхняя грань
	glTexCoord2f(0, 0);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1, 0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1, 1);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0, 1);
	glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture3ID);
	glBegin(GL_QUADS);
	// нижняя грань
	glTexCoord2f(0, 0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1, 0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1, 1);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0, 1);
	glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void reshape(int width, int height) {
  windowWidth = width;
  windowHeight = height;
  // параметры области вывода
  glViewport(0, 0, windowWidth, windowHeight);
  changePerspective();
}

void changePerspective() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (isPerspective) {
    gluPerspective(45.0, (float)windowWidth / (float)windowHeight, 1.0, 100.0);
  } else {
    float aspect = (float)windowWidth / (float)windowHeight;
    float сleft = -2.0f * aspect;
    float сright = 2.0f * aspect;
    float сbottom = -2.0f;
    float сtop = 2.0f;
    float zNear = -10.0f;
    float zFar = 10.0f;
	// преобразование проекции
    glOrtho(сleft, сright, сbottom, сtop, zNear, zFar);
  }
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:
		glutDestroyWindow(glutGetWindow());
			exit(0);
      break;
    case 'w':
      up = true;
      break;
    case 's':
      down = true;
      break;
    case 'a':
      left = true;
      break;
    case 'd':
      right = true;
      break;
    case 'f':
      toggle();
      break;
    default:
      break;
  }

  if (up)
    cameraAngleY += 0.05;
  if (down)
    cameraAngleY -= 0.05;
  if (left)
    cameraAngleX += 0.05;
  if (right)
    cameraAngleX -= 0.05;

  if (cameraAngleY > 2 * PI) {
    cameraAngleY -= 2 * PI;
  }
  if (cameraAngleY < -2 * PI) {
    cameraAngleY += 2 * PI;
  }

  glutPostRedisplay();
}

void toggle() {
  isPerspective = !isPerspective;
  changePerspective();
}

void keyboardUp(unsigned char key, int x, int y) {
  switch (key) {
    case 27:
        glutDestroyWindow(glutGetWindow());
          exit(0);
      break;
    case 'w':
      up = false;
      break;
    case 's':
      down = false;
      break;
    case 'a':
      left = false;
      break;
    case 'd':
      right = false;
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

// принимает путь и возвращает идентификатор текстуры
GLuint im_load(const char *image_file_path) {
    GLuint texture;
	// генерация текстуры (тут 1) массив GLuint в котором будут храниться идентификаторы этих текстур
    glGenTextures(1, &texture);

	// привязываем тестуру в GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, texture);

	// устанавливаем метод фильтрации glTexParameteri, пока рисуется mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// s ~ x
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// t ~ y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		// (би)линейная фильтрация ~ приближенное значение

	// загрузка изображения с помощью stb_image, чтобы получить параметры изображения
    int width, height, nrChannels;
    unsigned char *data = stbi_load(image_file_path, &width, &height, &nrChannels, 0);
    if (!data) {
        fprintf(stderr, "не удалось загрузить изображение%s\n", image_file_path);
        exit(EXIT_FAILURE);
    }
    if (nrChannels < 3 || nrChannels > 4) {
        fprintf(stderr, "число каналов должно быть 3 или 4 для %s\n", image_file_path);
        exit(EXIT_FAILURE);
    }

	// выравниваем всё под 1 байт 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// загрузка данных изображения в текстуру
    glTexImage2D(GL_TEXTURE_2D,	// текстурная цель (не GL_TEXTURE_1D и не GL_TEXTURE_3D)
                0, // уровень мипмапа для которого мы хотим сгенерировать текстуру (генеируем через OpenGL)
                GL_RGB, // формат хранения текстуры
                width,	// получили во время загрузки изображения
                height,
                0, // всегда 0, так как устарел
                nrChannels == 3 ? GL_RGB : GL_RGBA, // формат
                GL_UNSIGNED_BYTE, // передаем значения в байтах
                data);	// собственно, данные изображения
	// GL_UNPACK_ALIGNMENT именно для распаковыания + указывает на выравнивание в 4 байта
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    stbi_image_free(data);	// освобождение участка памяти, выделенного под загруженное изображение
    glBindTexture(GL_TEXTURE_2D, 0);	// отвязка объекта от текстуры
    return texture;
}

void loadTextures() {
  chocolateTextureID = im_load("chocolate.jpg");
  glassTextureID = im_load("glass.png");
  brickTextureID = im_load("1.jpg");
  textureID = im_load("texture.jpg");
  texture2ID = im_load("texture4.jpg");
  texture3ID = im_load("texture3.jpg");

}

// g++ -o lab4 lab4.cpp -framework OpenGL -framework GLUT -I.