#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"

// Globals.
static float _angle;
static int view_state; //Ortho view = 1, Perspective = 0

static int horizontal_move;
static int vertical_move;

static int left;
static int right;
static int up;
static int down;

static float ant_color_r;
static float ant_color_g;
static float ant_color_b;

static float eyeY;

static int isAnimate = 0;
static int animationPeriod = 60;

static int secondHand;

char food[] = "YUM";
char finish[] = "YOU WIN!";
char canvas_Name[] = "Ant-sy Ant-ics";

// Sets width and height of canvas to 480 by 480.
#define canvas_Width 640
#define canvas_Height 640

#define PI 3.14159265358979324

void keyboard_handler(unsigned char key, int x, int y);
void timer_func(int val);

void init() {
	glClearColor(0.5, 1.0, 0.5, 1.0);

	_angle = 0;
	view_state = 1; //Ortho view = 1, Perspective = 0

	horizontal_move = 0;
	vertical_move = 0;

	left = 0;
	right = 0;
	up = 0;
	down = 0;

	ant_color_r = 0.0;
	ant_color_g = 0.0;
	ant_color_b = 0.0;

	eyeY = 0.0;

	secondHand = 0;

}

void draw_connection_joints(int x)
{
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glEnd();
}

void draw_ant_legs(int x, int y)
{

	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(x, -y, 0);
	glEnd();

}

void draw_sphere(int translate_x, int ant_x, int ant_y, int connect_x) {
	glPushMatrix();
	glTranslatef(translate_x, vertical_move, -200);
	glutWireSphere(25, 16, 16);
	draw_ant_legs(ant_x, ant_y);
	draw_connection_joints(connect_x);
	glPopMatrix();
}

void draw_ant()
{
	draw_sphere(150 + horizontal_move, 15, 55, 35);
	draw_sphere(210 + horizontal_move, 0, 55, 35);
	draw_sphere(270 + horizontal_move, 15, 55, 0);
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

void draw_sugar_cube(int x, int y, int z, int size)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glutWireCube(size);
	glPopMatrix();
}

void writeBitmapString(void *font, char *string)
{
	int l, i;
	l = strlen(string);
	for (i = 0; i < l; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

}

void display_character(int x, int y, int z, float size, char *character)
{
	glPushMatrix();
	glRasterPos3i(x, y, z);
	writeBitmapString(GLUT_BITMAP_HELVETICA_12, character);
	glPopMatrix();
}

void view_setup() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (view_state == 0)
	{
		gluPerspective(116.0, canvas_Width / canvas_Height, 1.0, 640.0);
	}
	else {
		glOrtho(-320.0, 320.0, -320.0, 320.0, 0, 640.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, -0.5, 0.0, eyeY, -200.0, 0.0, 1.0, 0.0);
}

void draw_clock(void) {
	glPushMatrix();
	glTranslatef(190, 220, -200);

	draw_circle(40.0, 50.0, 50.0, 200);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(240, 270, -200);
	glRotated(secondHand, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 40, 0);

	glEnd();
	glPopMatrix();
}

void draw_scene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.0);
	view_setup();

	draw_sugar_cube(-210, 0, -200, 40);
	display_character(-225, -3, -180, 0.07, food);

	glColor3f(0.0, 0.0, 0.0);
	draw_clock();

	glColor3f(ant_color_r, ant_color_g, ant_color_b);
	draw_ant();
}

void end_scene(void) {
	glClearColor(0.5, 1.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0, 0);
	display_character(-15, 0, -200, 1, finish);
}

void display_func(void) {

	if ((210 + horizontal_move <= -210) && (210 + horizontal_move >= -213) && vertical_move == 0) {
		glutKeyboardFunc(NULL);
		end_scene();
		
	}
	else {
		draw_scene();
	}
}

void keyboard_handler(unsigned char key, int x, int y)
{
	switch (key)
	{
		//p
	case 80: case 112:
		//exit(0);
		view_state = abs(view_state - 1);
		view_setup();
		glutPostRedisplay();
		break;
		//h
	case 72: case 104:
		glutKeyboardFunc(NULL);
		left = 1;
		right = 0;
		up = 0;
		down = 0;
		isAnimate = 1;
		glutTimerFunc(60, timer_func, 10);
		break;
		//j
	case 74: case 106:
		left = 0;
		right = 1;
		up = 0;
		down = 0;
		isAnimate = 1;
		glutTimerFunc(60, timer_func, 10);
		break;
		//u
	case 85: case 117:
		left = 0;
		right = 0;
		up = 1;
		down = 0;
		isAnimate = 1;
		glutTimerFunc(60, timer_func, 10);
		break;
		//n
	case 78: case 110:
		left = 0;
		right = 0;
		up = 0;
		down = 1;
		isAnimate = 1;
		glutTimerFunc(60, timer_func, 10);
		break;
	case 67: case 99:
		if (ant_color_r == 0 and ant_color_g == 0) {
			ant_color_r = 1.0;
			ant_color_g = 0.647;
		}
		else {
			ant_color_r = 0.0;
			ant_color_g = 0.0;
		}
		glutPostRedisplay();
		break;
	case 84: case 116:
		if (eyeY == 0) {
			eyeY = 25;
		}
		else if (eyeY == 25) {
			eyeY = 0;
		}
		view_setup();
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void move_position(void) {
	if (left == 1) {
		horizontal_move -= 5;
	}
	else if (right == 1) {
		horizontal_move += 5;
	}
	else if (up == 1) {
		vertical_move += 5;
	}
	else if (down == 1) {
		vertical_move -= 5;
	}
}

void timer_func(int val) {
	switch (val)
	{
	case 1:
		secondHand -= 3;
		if (secondHand == -360)
		{
			exit(0);
		}
		glutSwapBuffers();
		glutPostRedisplay();
		glutTimerFunc(250, timer_func, 1);
		break;
	case 10:
		if (isAnimate != 0) {
			move_position();
			glutPostRedisplay();
			glutTimerFunc(60, timer_func, 20);

		}
		break;
	case 20:
		if (isAnimate != 0) {
			move_position();
			glutPostRedisplay();
			glutTimerFunc(0, timer_func, 30);
		}
		break;
	case 30:
		isAnimate = 0;
		glutKeyboardFunc(keyboard_handler);
		break;
	
	}
}

int main(int argc, char ** argv) {
	std::cout << "Any Key Click Will Start Animation.." << std::endl;
	glutInit(&argc, argv);
	my_setup(canvas_Width, canvas_Height, canvas_Name);
	//view_setup();

	glutTimerFunc(250, timer_func, 1);
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_handler);

	init();
	glutMainLoop();
	return 0;
}
