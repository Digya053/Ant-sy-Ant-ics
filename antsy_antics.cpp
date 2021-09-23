#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"
#include <chrono>

// Drawing routine.

#define PI 3.14159265358979324

// Globals.
static float _angle = 0;
static int view_state = 1; //Ortho view = 1, Perspective = 0

static int horizontal_move = 0;
static int vertical_move = 0;

static int left = 0;
static int right = 0;
static int up = 0;
static int down = 0;

static float ant_color_r = 0.0;
static float ant_color_g = 0.0;
static float ant_color_b = 0.0;

static float eyeY = 0.0;

static int isAnimate = 0;
static int animationPeriod = 60;
static int key = 1;

// initialize gateOpenDelay and flag  to zero(0)

int gateOpenDelay = 0;
int flag = 0;

// initialize the gateOpenDelayTime to 5 secs, and startTime to zero(0)
long gateOpenDelayTime = 5000;
long startTime = 0;

char food[] = "YUM";
char finish[] = "YOU WIN";

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

void draw_ant() 
{
	//first sphere
	glLoadIdentity();
	glTranslatef(150 + horizontal_move, vertical_move, -200);
	glutWireSphere(25, 25, 25);
	draw_ant_legs(15, 55);
	draw_connection_joints(35);
	//glPopMatrix();

	//second sphere
	glLoadIdentity();
	glTranslatef(210 + horizontal_move, vertical_move, -200);
	glutWireSphere(25, 25, 25);
	draw_ant_legs(0, 55);
	draw_connection_joints(35);

	//third sphere
	glLoadIdentity();
	glTranslatef(270 + horizontal_move, vertical_move, -200);
	glutWireSphere(25, 25, 25);
	draw_ant_legs(15, 55);
	

	//glFlush();
	//glPopMatrix();
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
	glLoadIdentity();
	glTranslatef(x, y, z);
	glutWireCube(size);
}

void writeStrokeString(void *font, char *string)
{
	//char *c;
	int l, i;
	l = strlen(string);
	for (i = 0; i < l; i++)
	{
		glutStrokeCharacter(font, string[i]);
	}
	//for (c = string; *c != '\0'; c++) {
		//glutBitmapCharacter(font, *c);
	//}

}

void display_character(int x, int y, int z, float size, char *character) 
{
	glLoadIdentity();
	glTranslatef(x, y, -200);
	glScalef(size, size, 0);
	//glRasterPos3i(x, y, z);
	writeStrokeString(GLUT_STROKE_ROMAN,character);
}

void update(int value)
{
	_angle += 3.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
}

void setup() {
	if (view_state == 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(116.0, 1.0, 1.0, 640.0);
		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

	}
	else {
		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();
		glOrtho(-320.0, 320.0, -320.0, 320.0, 0, 640.0);

		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
	}
}

void background(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.0);
	draw_sugar_cube(-210, 0, -200, 40);
	display_character(-213, -3, -200, 0.07, food);
	//display_character(-200, -3, -200, 0.7, finish);

	glColor3f(0.0, 0.0, 0.0);
	glLoadIdentity();
	glTranslatef(190, 220, -200);
	draw_circle(40.0, 50.0, 50.0, 360);

	glLoadIdentity();
	glBegin(GL_LINES);
	glVertex3f(240, 300, -200);
	glVertex3f(240, 270, -200);
	glEnd();
}

void display_func_end(void) {
	glClearColor(0.5, 1.0, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 0, 0);
		display_character(-200, -3, -200, 0.7, finish);
		glFlush();
	}

void display_func(void) {
	if (210 + horizontal_move == -210 && vertical_move == 0) {
		display_func_end();
		glutKeyboardFunc(NULL);
		
	} else{
		background();
		glColor3f(ant_color_r, ant_color_g, ant_color_b);
		draw_ant();
		glFlush();
		glFinish();

	}
}


long double millis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		).count();
}

void gate_delay() {
	if (flag == 0) {
		startTime = millis();
		flag = 1;
	}
	// If the timer is finished, then set the flag gateOpenDelay  = 1

	if (millis() - startTime >= gateOpenDelayTime) {
		gateOpenDelay = 1;
	}
}
void timer_func(int val) {
	
	if (isAnimate == 1) {
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
		
		glutTimerFunc(60, timer_func, 1);
		//glutKeyboardFunc(NULL);

		glutPostRedisplay();
		isAnimate = 0;
		key = 1;
		//glutKeyboardFunc(keyboard_handler);
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
		setup();
		glutPostRedisplay();
		break;
		//h
	case 72: case 104:
		left = 1;
		right = 0;
		up = 0;
		down = 0;
		//horizontal_move -= 5;
		//gate_delay();
		//glutKeyboardFunc(NULL);
		isAnimate = 1;
		glutTimerFunc(0, timer_func, 1);
		break;
		//j
	case 74: case 106:
		left = 0;
		right = 1;
		up = 0;
		down = 0;
		//horizontal_move += 5;
		isAnimate = 1;
		glutTimerFunc(0, timer_func, 1);
		break;
		//u
	case 85: case 117:
		left = 0;
		right = 0;
		up = 1;
		down = 0;
		//vertical_move += 5;
		isAnimate = 1;
		glutTimerFunc(0, timer_func, 1);
		break;
		//n
	case 78: case 110:
		left = 0;
		right = 0;
		up = 0;
		down = 1;
		//vertical_move -= 5;
		isAnimate = 1;
		glutTimerFunc(60, timer_func, 1);
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
		eyeY = 25;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}


// Sets width and height of canvas to 480 by 480.
#define canvas_Width 640
#define canvas_Height 640
char canvas_Name[] = "Ant-sy Ant-ics";


int main(int argc, char ** argv) {
	std::cout << "Any Key Click Will Start Animation.." << std::endl;
	glutInit(&argc, argv);
	my_setup(canvas_Width, canvas_Height, canvas_Name);
	
	//gluLookAt(0, eyeY, -0.5, 0, 0, -200, 0, 1, 1);
	setup();


	glClearColor(0.5, 1.0, 0.5, 1.0);
	//whole_scene();

	glutDisplayFunc(display_func);
	if (key == 0) {
		glutKeyboardFunc(NULL);
	}
	else if (key == 1) {
		glutKeyboardFunc(keyboard_handler);
	}
	
	//glutTimerFunc(250, timer_func, 1);
	glutMainLoop();
	return 0;
}