#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <GL/freeglut.h>
#include <windows.h>
#include <stdio.h>

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();

	glutSwapBuffers();
}

void keyboard() {

}

void update() {
	if (GetAsyncKeyState('W') & 1) {
		wprintf(L"%s\n", L"приветики\n");
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF8");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("window");

	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutDisplayFunc(display);
	glutMainLoop();
}