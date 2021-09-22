#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"

// Drawing routine.

#define PI 3.14159265358979324

// Globals.

/*static float R = 40.0; // Radius of circle.
static float X = 50.0; // X-coordinate of center of circle.
static float Y = 50.0; // Y-coordinate of center of circle.
static int numVertices = 360; // Number of vertices on circle
*/
static float _angle = 0;

void draw_connection_joints(int x) 
{
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glEnd();
}

void draw_ant_legs(int x, int y) 
{
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(x, y, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(x, -y, 0);
	glEnd();
}

void draw_ant() 
{
	//first sphere
	glLoadIdentity();
	glTranslatef(150, 0, -200);
	glutWireSphere(25, 25, 25);
	draw_ant_legs(15, 55);
	draw_connection_joints(35);

	//second sphere
	glLoadIdentity();
	glTranslatef(210, 0, -200);
	glutWireSphere(25, 25, 25);
	draw_ant_legs(0, 55);
	draw_connection_joints(35);

	//third sphere
	glLoadIdentity();
	glTranslatef(270, 0, -200);
	glutWireSphere(25, 25, 25);
	draw_ant_legs(15, 55);
}

void draw_circle(float R, float X, float Y, int numVertices)
{
	float t = 0; // Angle parameter.
	int i;
	// Draw a line loop with vertices at equal angles apart on a circle
	// with center at (X, Y) and radius R, The vertices are colored randomly.
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < numVertices; ++i)
	{
		glVertex3f(X + R * cos(t), Y + R * sin(t), 0.0);
		t += 2 * PI / numVertices;
	}
	glEnd();
}

void draw_food(int x, int y, int z, int size)
{
	glLoadIdentity();
	glTranslatef(x, y, z);
	glutWireCube(size);
}

void writeStrokeString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}

}

void display_character(int x, int y, int z, float size) 
{
	char food[] = "YUM";
	glLoadIdentity();
	glTranslatef(x, y, z);
	glScalef(size, size, 0);
	writeStrokeString(GLUT_STROKE_MONO_ROMAN, food);
}

void update(int value)
{
	_angle += 3.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
}

void display_func(void) {
	glClearColor(0.7, 1.0, 0.9, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	draw_food(-210, 0, -200, 40);
	display_character(-210, 0, -200, 0.05);
	draw_ant();

	glLoadIdentity();
	glTranslatef(200, 220, -200);
	draw_circle(40.0, 50.0, 50.0, 360);

	glLoadIdentity();
	glBegin(GL_LINES);
	glVertex3f(250, 300, -200);
	glVertex3f(250, 270, -200);
	glEnd();

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
	//glutTimerFunc(250, update, 0);
	glutMainLoop();
	return 0;
}