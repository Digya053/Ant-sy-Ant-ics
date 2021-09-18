#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"


void display_func(void) {
	glClearColor(0.7, 1.0, 0.9, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// Position the box for viewing.

	gluLookAt(0.0, 0.0, -0.5, 0.0, 0.0, -200.0, 0.0, 1.0, -1.0);
	glColor3f(0.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(-210, 0, -180);
	glutWireCube(40);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(150, 0, -200);
	glutWireSphere(25, 25, 25);

	// Draw legs
	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(-15, 55, -200);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(-15, -55, -200);
	glEnd();

	// Draw connection line
	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(35, 0, -200);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(210, 0, -200);
	glutWireSphere(25, 25, 25);
	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(0, 55, -200);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(0, -55, -200);
	glEnd();

	// Draw connection line
	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(35, 0, -200);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(270, 0, -200);
	glutWireSphere(25, 25, 25);
	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(15, 55, -200);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, -200);
	glVertex3f(15, -55, -200);
	glEnd();
	glPopMatrix();	

	glFlush();
}


// Sets width and height of canvas to 480 by 480.
#define canvas_Width 640
#define canvas_Height 640
char canvas_Name[] = "Ant-sy Ant-ics";


int main(int argc, char ** argv) {
	std::cout << "Any Key Click Will Start Animation.." << std::endl;
	glutInit(&argc, argv);
	my_setup(canvas_Width, canvas_Height, canvas_Name);

	glutDisplayFunc(display_func);
	glutMainLoop();
	return 0;
}