/*CS 445/545 Prog 2 for Digya Acharya*/
/***********************************************************************************************
antsy_antics.cpp
EXTRA CREDIT: All three extra credit features have been implemented, i.e, supporting toggling of an ant
color, supporting toggling of the camera center without changing the camera heading 25 units in the 
y-direction and adding clock in the upper right corner; ending game as it reaches the starting position.

Software Architecture Statement: This program illustrates the movement of an ant to reach a sugar cube
before the clock runs out of time by registering three callback functions: display_func() which is a 
display callback handler, timer_func() which is a timer callback handler and keyboard_handler() which is
a keyboard callback handler. The program uses double buffered display mode and uses glutSwapBuffers() at
the end of display_func to swap the next scene generated in the back buffer to the content of the front 
buffer. glutWireSphere() and glutWireCube() have been used to draw ant and cube, & series of 
glPushMatrix(), translate, draw, glPopMatrix() is used to place them, instead of glLoadIdentity() so as
to change the camera position using gluLookAt conveniently.
Major Functions => draw_new_scene() draws the complete scene which includes an ant, clock and a sugar 
cube; display_func() calls the draw_new_scene(), is also responsible for redrawing the screen on each 
new frame and displaying "YOU WIN" screen at the end; timer_func() handles movement of an ant and the motion of a
clock; keyboard_handler() handles the key input to cause horizontal and vertical motion.
Major global variables => horizontal_move and vertical_move are the major integer global variables used
in animation which is responsible for causing horizontal and vertical move of an ant; left,right,up, 
down are the major global integer flags for determining direction of motion of an ant; animationPeriod 
is a global integer variable used for timing two subsequent movement in a single key press; view_state 
for tracking the type of view volume in display.
Keyboard Interactions => Pressing 'H' or 'h' causes  the left motion, 'J' or 'j' causes the right
motion, 'U' or 'u' causes the up motion, 'N' or 'n' causes the down motion, 'C' or 'c' toggles the ant
color, 'T' or 't' toggles the camera position and 'P' or 'p' toggles the view volume.
************************************************************************************************/
//CHANGE IN OpenGL445Setup.h => The glut display mode has been set to double buffered window replacing
//the mode to GLUT_DOUBLE in the "OpenGL445Setup.h" file with instructor's approval.

#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup.h"

// Globals variables
static int view_state; // Flag to keep track of the type of view volume currently in display 
						// [Ortho view = 1, Perspective = 0]

// For animation
static int horizontal_move; // Changes position in the horizontal direction (left/right)
static int vertical_move; // Changes position in the vertical direction (up/down)

static int left; // Set the direction of move to left
static int right; // Set the direction of move to right
static int up; // Set the direction of move to up
static int down; // Set the direction of move to down

static int animationPeriod = 60; // Set the time between two consecutive movements on a single
								// key press as 60 ms
// For toggling ant color
static float ant_color_r;		// Stores the red, green and blue values of ant color
static float ant_color_g;
static float ant_color_b;

// For changing camera orientation
static float eyeY;				// Stores the current y-position of the camera

// For moving 
static int sweepHand;			// Used for rotating the sweep hand of a clock

char food[] = "YUM";			// Text on the sugar cube
char finish[] = "YOU WIN!";		// Text displayed after the center of ant aligns with center of cube 
								// before the time is out
char canvas_Name[] = "Ant-sy Ant-ics"; // Name at the top of canvas

// Sets width and height of canvas to 480 by 480.
#define canvas_Width 640
#define canvas_Height 640

#define PI 3.14159265358979324	// For drawing circle

// Function Prototypes
void keyboard_handler(unsigned char key, int x, int y);
void timer_func(int val);

void init(void){
	/* This function sets the background or clearning color of the canvas. Also set the initial state
	of all the global variables declared. */
	glClearColor(0.5, 1.0, 0.5, 1.0);				// the background color
	view_state = 1;									// Set the view volume to orthographic

	// Set the inital value of variables to move ant to 0
	horizontal_move = 0;							
	vertical_move = 0;

	// Set the initial direction variables to 0
	left = 0;
	right = 0;
	up = 0;
	down = 0;

	// Set ant color to black
	ant_color_r = 0.0;
	ant_color_g = 0.0;
	ant_color_b = 0.0;

	// As the camera position is at the origin in the beginning, set eyeY (position of camera y) to 0
	eyeY = 0.0;

	// The sweep hand at the initial position
	sweepHand = 0;
}

void draw_connection_joints(int x){
	/* 
	This function joins the sphers of an ant.

	Parameters
	----------
		x: Integer
			x coordinate to keep a distance of 10 units with next sphere
	*/
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glEnd();
}

void draw_legs(int x, int y){
	/*
	This function draws the legs of an ant.

	Parameters
	----------
		x: Integer
			x coordinate to draw leg 30 units above the sphere
		y: Integer
			y coordinate to draw leg 30 units above the sphere
	*/
	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(x, -y, 0);
	glEnd();

}

void draw_sphere(int translate_x, int leg_x, int leg_y, int joint_x){
	/*
	This function draws the a wiresphere with connection joint and leg in the translated position.

	Parameters
	----------
		translate_x: Integer
			x coordinate to draw the sphere
		leg_x: Integer
			x coordinate to draw leg 30 units above the sphere
		leg_y: Integer
			y coordinate to draw leg 30 units above the sphere
		joint_x: Integer
			x coordinate to keep a distnace of 10 units with next sphere
	*/
	glPushMatrix();
	glTranslatef(translate_x, vertical_move, -200);
	glutWireSphere(25, 16, 16);
	draw_legs(leg_x, leg_y);
	draw_connection_joints(joint_x);
	glPopMatrix();
}

void draw_ant(void){
	/*
	This function draws the complete ant making use draw sphere, draw legs and draw connection joints
	function. Each start coordinate of sphere is separated with other by 60 units: 50 units to draw a 
	wiresphere and 10 units for connection joint.
	*/
	draw_sphere(150 + horizontal_move, 15, 55, 35);
	draw_sphere(210 + horizontal_move, 0, 55, 35);
	draw_sphere(270 + horizontal_move, 15, 55, 0);
}

void draw_circle(float R, float X, float Y, int numVertices){
	/*
	This function draws the circle of a clock. The parametric equations implemented are:
	x = X + RCos(t), y = Y + RSin(t), z = 0 whre range of t is [0, 2*PI] 
	
	Parameters
	----------
		R: float
			The radius of the circle
		X: float 
			x-coordinate of the center of the circle
		Y: float
			y-coordinate of the center of the circle
		numVertices: Integer
			Number of points to draw
	*/
	float t = 0; // Angle parameter.
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < numVertices; ++i)
	{
		glVertex3f(X + R * cos(t), Y + R * sin(t), 0.0);
		t += 2 * PI / numVertices;
	}
	glEnd();
}

void draw_sugar_cube(int x, int y, int z, int size){
	/*
	This function draws a sugar cube of particular size at given (x, y, z) position
	
	Parameters:
	-----------
		x: Integer
			x-coordinate where a cube is to be placed
		y: Integer
			y-coordinate where a cube is to be placed
		z: Integer
			z-coordinate where a cube is to be placed
		size: Integer
			size of the cube
	*/
	glPushMatrix();
	glTranslatef(x, y, z);
	glutWireCube(size);
	glPopMatrix();
}

/****** BORROWED CODE FROM GUHA - fonts.cpp ******/
void writeBitmapString(void *font, char *string){
	/*
	This function writes a bitmap text, one character at a time.
	
	Parameters:
	----------
		font: void pointer
			The font to use to write a text.
		string: char pointer
			The text to write.
	*/
	int l, i;
	l = strlen(string);
	for (i = 0; i < l; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

void display_character(int x, int y, int z, float size, char *character){
	/*
	This function displays a bitmap text at (x, y, z) position which uses a 
	GLUT_BITMAP_HELVETICA_12 font.
	
	Parameters:
	-----------
		x: Integer
			x-coordinate where text is to be placed.
		y: Integer
			y-coordinate where text is to be placed.
		z: Integer
			z-coordinate where text is to be placed.
		size: Float
			size of a text
		character: char pointer
			The text to be written
	*/
	glPushMatrix();
	//glRasterPos3i has been used to place text at a preferred position
	glRasterPos3i(x, y, z);           
	writeBitmapString(GLUT_BITMAP_HELVETICA_12, character);
	glPopMatrix();
}

void view_setup(void) {
	/*
	This function sets up the view volume.
	*/

	// Set the current matrix mode to projection
	glMatrixMode(GL_PROJECTION);
	// Load identity matrix
	glLoadIdentity();
	// Sets view volume to perspective if view state is 0. The aspect ratio of perspective projection
	//is computed as 116.
	if (view_state == 0)
	{
		gluPerspective(116.0, canvas_Width / canvas_Height, 1.0, 640.0);
	}
	// Sets view volume to orthographic if view state is 1. 
	else {
		glOrtho(-320.0, 320.0, -320.0, 320.0, 0.5, 640.0);
	}
	// Sets the current matrix mode to modelview to apply the subsequent operations on modelview mode.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_clock(float x1, float y1, float x2, float y2, float z, int length) {
	/*
	This function draws a complete clock. The clock is drawn using circle and line.

	Parameters:
	-----------
		x1: Float
			x-coordinate of the center of the circle
		y1: Float
			y-coordinate of the center of the circle
		x2: Float
			x-position where sweep hand is to be placed
		y2: Float
			y-position where sweep hand is to be placed
		z: Float
			z coordinate of both circle and line
		length: Integer
			Length of the sweep hand
	*/
	// draw a circle
	glPushMatrix();
	glTranslatef(x1, y1, z);
	draw_circle(35.0, 50.0, 50.0, 200);
	glPopMatrix();

	// draw sweep hand
	glPushMatrix();
	glTranslatef(x2, y2, z);
	glRotated(sweepHand, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);
	glEnd();
	glPopMatrix();
}

void draw_new_scene(void) {
	/*
	This function draws all objects of the scene: sugar cube with a text, ant and clock. 
	The sugar cube is yellow in color, the color of ant is toggled in between black and orange so, 
	is used as a global variable, clock is displayed in black color.
	This function is called by display callback handler, i.e, display_func on each new frame 
	generation.
	*/
	glColor3f(1.0, 1.0, 0.0);

	// draw sugar cube
	draw_sugar_cube(-210, 0, -200, 40);
	display_character(-225, -3, -180, 0.07, food);

	// draw clock
	glColor3f(0.0, 0.0, 0.0);
	draw_clock(190, 185, 240, 236, -200, 34);

	// draw ant
	glColor3f(ant_color_r, ant_color_g, ant_color_b);
	draw_ant();
}

void end_scene(void) {
	/*
	This function is called by display callback handler, i.e, display_func when the center of the 
	ant aligns with the center of the cube before the end of time.

	This function writes a text indicating a win on a cleared canvas.
	*/
	glClearColor(0.5, 1.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0, 0);
	display_character(-15, 0, -200, 1, finish);
	// Exit after 1000 ms of displaying "YOU WIN" screen
	glutTimerFunc(1000, timer_func, 2);
}

void display_func(void) {
	/* 
	This is a glut display callback handler which is called whenever a window needs to be displayed 
	or redisplayed. It clears the canvas screen and reloads all the objects of the scene when called.
	This function also detects whether the center of the ant aligns with the center of the cube, 
	and after it aligns, this function calls end_scene function to display the game win text.
	*/

	// Detect whether the x and y coordinate of center sphere of ant is equal to the x and y 
	// coordinate of the cube.
	if ((210 + horizontal_move <= -210) && (210 + horizontal_move >= -213) && vertical_move == 0) {
		// Prevent ant from moving beyond the win position
		glutKeyboardFunc(NULL);
		// After moving to the win position, end the program
		end_scene();
		
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		// Draws all objects of the scene
		draw_new_scene();
	}
	// At the end of each display callback, swap the back buffer drawn with the new position of the 
	// objects with the front buffer
	glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int x, int y)
{
	/*
	This function is a glut keyboard callback handler. Whenever a key is pressed, it gets stored in a key
	variable. The switch condition matches the corresponding key condition and perform necessary
	computation.

	Parameters:
	-----------
		key: unsigned char
			The key input received
		x: Integer
			Mouse x-coordinate
		y: Integer
			Mouse y-coordinate
	*/
	switch (key)
	{
	case 80: case 112:						// 'p' or 'P' key is pressed
		view_state = abs(view_state - 1);	// Change the view volume state
		view_setup();						// Set up the new projection and modelview matrix stack
		glutPostRedisplay();				// Queue a display event
		break;
	case 72: case 104:						// 'h' or 'H' key is pressed
		glutKeyboardFunc(NULL);				// Disable keyboard callback
		left = 1;							// The next move is a left move
		right = 0;							// Set the flag of other direction to 0
		up = 0;
		down = 0;
		glutTimerFunc(60, timer_func, 10);  // Queue timer for moving to the left position
		break;
	case 74: case 106:					    // 'j' or 'J' key is pressed
		glutKeyboardFunc(NULL);				// Disable keyboard callback
		left = 0;
		right = 1;							// The next move is a right move
		up = 0;
		down = 0;
		glutTimerFunc(60, timer_func, 10);	// Queue timer for moving to the right position
		break;
	case 85: case 117:						// 'u' or 'U' key is pressed
		glutKeyboardFunc(NULL);				// Disable keyboard callback
		left = 0;
		right = 0;
		up = 1;								// The next move is a up move
		down = 0;
		glutTimerFunc(60, timer_func, 10);	// Queue timer for moving to the up position
		break;
	case 78: case 110:						// 'n' or 'N' key is pressed
		glutKeyboardFunc(NULL);				// Disable keyboard callback
		left = 0;
		right = 0;
		up = 0;
		down = 1;							// The next move is a down move
		glutTimerFunc(60, timer_func, 10);	// Queue timer event for moving to the down position
		break;
	case 67: case 99:						// 'c' or 'C' key is pressed
		// Change the ant color to orange if black and vice versa
		if (ant_color_r == 0 and ant_color_g == 0) {
			ant_color_r = 1.0;
			ant_color_g = 0.647;
		}
		else {
			ant_color_r = 0.0;
			ant_color_g = 0.0;
		}
		glutPostRedisplay();				// Queue a display event
		break;
	case 84: case 116:						// 't' or 'T' key is pressed
		// Change the y-position to 0 if 25 and vice versa
		if (eyeY == 0) {					
			eyeY = 25;
			view_setup();			// Set up the projection and modelview matrix stack for new position
			gluLookAt(0.0, eyeY, 0.0, 0.0, 0.0, -200.0, 0.0, 1.0, 0.0); //Reposition the camera
		}
		else if (eyeY == 25) {
			eyeY = 0;
			view_setup();			// Set up the projection and modelview matrix stack for new position
		}
		glutPostRedisplay();				// Queue a display event
		break;
	default:
		break;
	}
}

void move_position(void) {
	/*
	This function checks the motion flag and moves to the position where the flag is set on.
	*/

	if (left == 1) {
		horizontal_move -= 5;				// Move 5 units in the left direction
	}
	else if (right == 1) {
		horizontal_move += 5;				// Move 5 units in the right direction
	}
	else if (up == 1) {
		vertical_move += 5;					// Move 5 units towards up direction
	}
	else if (down == 1) {
		vertical_move -= 5;					// Move 5 units towards down direction
	}
}

void timer_func(int val) {
	/*
	This function is a glut timer callback function which repositions the sweepHand of the clock every
	250 ms, enable 10 units move in any direction with the time interval of 60 ms between two 5 units
	move.

	Parameters:
	-----------
		val: Integer
			This is the timerID.
	*/
	switch (val)
	{
	case 1:									// value or timer ID is set to 1
		sweepHand -= 3;						// Rotate 3 degree
		// Exit if sweep hand goes back to the initial position
		if (sweepHand == -360)
		{
			exit(0);
		}
		glutPostRedisplay();				// Queue a display event
		glutTimerFunc(250, timer_func, 1);	// Queue timer event for rotating each 250 ms
		break;
	case 2:									// value or timer ID is set to 2
		exit(0);							// exit
	case 10:								// value or timer ID is set to 10
		move_position();					// Move position of the ant
		glutPostRedisplay();				// display event for generating new frame with ant movement
		glutTimerFunc(60, timer_func, 20);	// timer event for second move in 60 ms
		break;
	case 20:								// value or timer ID is set to 20
		move_position();					// Move position of the ant
		glutPostRedisplay();				// display event for generating new frame with ant movement
		glutTimerFunc(0, timer_func, 30);	// timer event for reenabling keyboard callbacks
		break;
	case 30:								// value or timer ID is set to 30
		glutKeyboardFunc(keyboard_handler);	// reenable keyboard callback
		break;
	}
}

void print_interactions(void) {
	/* 
	This function prints the key interactions on terminal for convenience.
	*/
	std::cout << "==========Keyboard Interactions:==========" << std::endl;
	std::cout << "Press 'H' or 'h' for left motion" << std::endl;
	std::cout << "Press'J' or 'j' for right motion" << std::endl;
	std::cout << "Press 'U' or 'u' for up motion" << std::endl;
	std::cout << "Press 'N' or 'n' for down motion" << std::endl;
	std::cout << "Press 'C' or 'c' to toggle the ant color" << std::endl;
	std::cout << "Press 'T' or 't' to toggle the camera position" << std::endl;
	std::cout << "Press 'P' or 'p'to toggle the view volume" << std::endl;
}

/************** MAIN FUNCTION **************/
int main(int argc, char ** argv) {
	// Setup preliminaries
	glutInit(&argc, argv);
	my_setup(canvas_Width, canvas_Height, canvas_Name);
	// Print keyboard interactions for convenience
	print_interactions();

	// Register timer callback handler for running clock
	glutTimerFunc(250, timer_func, 1);
	// Register display callback handler
	glutDisplayFunc(display_func);
	// Register keyboard callback handler
	glutKeyboardFunc(keyboard_handler);

	// Initialize all the global variables
	init();
	// Event processing loop
	glutMainLoop();
	return 0;
}
