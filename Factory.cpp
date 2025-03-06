//  ========================================================================
//  COSC363: Computer Graphics (2025);  University of Canterbury.
//
//  FILE NAME: Humanoid.cpp
//  See Lab02.pdf for details
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include <cmath>
#include <algorithm>
#include <math.h>
using namespace std;

//-- Globals ---------------------------------------------------------------

// mesh data
float *x, *y, *z;					//vertex coordinates
int *nv, *t1, *t2, *t3, *t4;		//number of vertices and vertex indices of each face
int nvert, nface;		
float xmin, xmax, ymin, ymax;	

// frame rate
int frameRate = 16;
UINT32 frameCount = 0;
// Camera 
float cam_x = 0;
float cam_y = 7;	//Camera height
float cam_z = 18;
float cam_x_angle = 0;
float cam_y_angle = 0;
float cam_z_angle = 0;
float angle = 0;	//Rotation angle for viewing

// movement 
int keyPress = 0b00000000; // W A S D SPACE C NAN NAN

// processing
bool wireframe = false;
bool pause = false;


// mouse
int gMouseX = 0;
int gMouseY = 0;

float mouseSensitivity = 0.1f;
// window size
int windowWidth = 1200;
int windowHeight = 900;



void centerMouse() {
    glutWarpPointer(windowWidth/2, windowHeight/2);
}

//-- Draws a grid of lines on the floor plane ------------------------------
void drawFloor() {
	glColor3f(0., 0.5, 0.);			//Floor colour
	for (float i = -50.; i <= 50.; i++) {
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-50., 0., i);
			glVertex3f( 50., 0., i);
			glVertex3f(i, 0., -50.);
			glVertex3f(i, 0.,  50.);
		glEnd();
	}
}

//-- Loads mesh data in OFF format    --------------------------------------
void loadMeshFile(const char* fname) {
	ifstream fp_in;
	int nedge;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open()) {
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');		//ignore first line
	fp_in >> nvert >> nface >> nedge;	//read number of vertices, polygons, edges (not used)

	x = new float[nvert];	//create arrays
	y = new float[nvert];
	z = new float[nvert];

	nv = new int[nface];
	t1 = new int[nface];
	t2 = new int[nface];
	t3 = new int[nface];
	t4 = new int[nface];

	for(int i = 0; i < nvert; i++) {	//read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];
	}
	for(int i = 0; i < nface; i++) {	//read polygon list 
		fp_in >> nv[i] >> t1[i] >> t2[i] >> t3[i];
		if (nv[i] == 4) {
			fp_in >> t4[i];
		} else {
			t4[i] = -1;
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}

//-- Computes the min, max values of coordinates  --------------------------
void computeMinMax() {
	xmin = xmax = x[0];
	ymin = ymax = y[0];
	for(int i = 1; i < nvert; i++) {
		if 		(x[i] < xmin) xmin = x[i];
		else if (x[i] > xmax) xmax = x[i];
		if 		(y[i] < ymin) ymin = y[i];
		else if (y[i] > ymax) ymax = y[i];
	}
}

//-- Function to compute the normal vector of a triangle with index indx ---
void normal(int indx) {
	float x1 = x[t1[indx]], x2 = x[t2[indx]], x3 = x[t3[indx]];
	float y1 = y[t1[indx]], y2 = y[t2[indx]], y3 = y[t3[indx]];
	float z1 = z[t1[indx]], z2 = z[t2[indx]], z3 = z[t3[indx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}



//-- Function to compute the normal vector of a triangle with index tindx --
void drawConveyer() {
    glColor3f(1, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glScalef(15, 0.1, 1);
    if (wireframe) {
        glutWireCube(1);
    } else {
        glutSolidCube(1);
    }
    
    glPopMatrix();
}
void drawPcb() {
    glColor3f(0.5, 1, 0.5);
    glTranslatef(5, 1.1, 0);
    glPushMatrix();
    glTranslatef(- (frameCount * 0.016),0, 0);
    glScalef(1, 0.1, 1);
    if (wireframe) {
        glutWireCube(1);
    } else {
        glutSolidCube(1);
    }

    
    glPopMatrix();
}
void drawModel(){
    glColor3f(0.2, 0.7, 0.1);
    
    glPushMatrix();
    glTranslatef(5, 1.1, 0);
    glTranslatef(- (frameCount * 0.016),0, 0);
    glScalef(0.1, 0.1, 0.1);
    for(int indx = 0; indx < nface; indx++)	{	//draw each face
		normal(indx);
		if (nv[indx] == 3)	glBegin(GL_TRIANGLES);
		else				glBegin(GL_QUADS);
			glVertex3d(x[t1[indx]], y[t1[indx]], z[t1[indx]]);
			glVertex3d(x[t2[indx]], y[t2[indx]], z[t2[indx]]);
			glVertex3d(x[t3[indx]], y[t3[indx]], z[t3[indx]]);
			if (nv[indx] == 4) glVertex3d(x[t4[indx]], y[t4[indx]], z[t4[indx]]);
		glEnd();
	}
    glPopMatrix();
}
//-- Display: --------------------------------------------------------------
//-- This is the main display module containing function calls for generating
//-- the scene.
void display() {
	float lpos[4] = {10., 10., 10., 1.};	//light's position
    if (wireframe){
        glDisable(GL_LIGHTING);	//Disable lighting 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_LIGHTING);	
    } 
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glRotatef(cam_x_angle, 1, 0., 0.);	//Rotate the scene about the y-axis
    glRotatef(cam_y_angle, 0., 1., 0.);
	gluLookAt(cam_x, cam_y, cam_z, cam_x , cam_y , cam_z - 1, 0., 1., 0.);
	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position
    
	
    
	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
	drawFloor();

	glEnable(GL_LIGHTING);			//Enable lighting when drawing the model
	drawModel();
    drawConveyer();
    drawPcb();
	glFlush();
}
void cameraCalculator(){

 // Convert angles to radians
 float yAngleRad = cam_y_angle * M_PI / 180.0f;
 float xAngleRad = cam_x_angle * M_PI / 180.0f;
 
 // Calculate movement vectors based on camera angle
 float forwardX = sin(yAngleRad) * cos(xAngleRad);
 float forwardY = -sin(xAngleRad);
 float forwardZ = -cos(yAngleRad) * cos(xAngleRad);
 
 // Right vector (perpendicular to forward vector)
 float rightX = cos(yAngleRad);
 float rightZ = sin(yAngleRad);
 
 // Movement speed
 float speed = 0.1f;
 
 // Apply movement based on key presses
 if (keyPress & 0b10000000) { // W
     cam_x += forwardX * speed;
     cam_y += forwardY * speed;
     cam_z += forwardZ * speed;
 }
 if (keyPress & 0b00100000) { // S
     cam_x -= forwardX * speed;
     cam_y -= forwardY * speed;
     cam_z -= forwardZ * speed;
 }
 if (keyPress & 0b01000000) { // A
     cam_x -= rightX * speed;
     cam_z -= rightZ * speed;
 }
 if (keyPress & 0b00010000) { // D
     cam_x += rightX * speed;
     cam_z += rightZ * speed;
 }
 

}
void timer(int value) {
    // always do the camera calculation for paused movement
    cameraCalculator();
    if (pause) {
        glutTimerFunc(16, timer, 0);
        glutPostRedisplay();
        return;
    }

    frameCount++;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
//-- Initialize OpenGL parameters ------------------------------------------
void initialize() {
	glClearColor(1., 1., 1., 1.);	//Background colour
    loadMeshFile("../Cow.off");
	glEnable(GL_LIGHTING);			//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60., 1., 0.1, 100.);
}

// detect key press
void keyboardUp(unsigned char key, int x, int y) {
    if( 'w' == key )
    keyPress &= 0b01111111;
    if( 'a' == key )
    keyPress &= 0b10111111;
    if( 's' == key )
    keyPress &= 0b11011111;
    if( 'd' == key )
    keyPress &= 0b11101111;

    glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
    const float step = 0.1;
    if( 'w' == key )
    keyPress |= 0b10000000;
    if( 'a' == key )
    keyPress |= 0b01000000;
    if( 's' == key )
    keyPress |= 0b00100000;
    if( 'd' == key )
    keyPress |= 0b00010000;
    if (' ' == key)
    pause = !pause;

    if ('q' == key) {
        wireframe = !wireframe;
    }
    glutPostRedisplay();
}

//-- Special key event callback --------------------------------------------
//-- To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y) {
	if 		(key == GLUT_KEY_LEFT)  cam_x_angle--;
    else if (key == GLUT_KEY_RIGHT) cam_x_angle++;
    else if (key == GLUT_KEY_UP) cam_y_angle++;
    else if (key == GLUT_KEY_DOWN) cam_y_angle--;
    if (key == GLUT_KEY_F1) exit(0);
    
    
	glutPostRedisplay();
}

// Replace the mouseHandeller function
void mouseHandeller(int x, int y) {
    float deltaX = (x - windowWidth/2) * mouseSensitivity;
    float deltaY = (y - windowHeight/2) * mouseSensitivity;

    cam_y_angle += deltaX;
    cam_x_angle += deltaY;

    // Clamp vertical rotation to prevent over-rotation
    if(cam_x_angle > 89.0f)
        cam_x_angle = 89.0f;
    if(cam_x_angle < -89.0f)
        cam_x_angle = -89.0f;
    centerMouse();
    
    glutPostRedisplay();
}

//-- Main: Initialize glut window and register call backs ------------------
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Humanoid");
	initialize();
    glutTimerFunc(frameRate, timer, 0); // FPS 
	glutDisplayFunc(display); // Display update
	glutSpecialFunc(special); // Special key event
    glutKeyboardFunc(keyboard); // Keyboard event
    glutKeyboardUpFunc(keyboardUp); // key up
    glutPassiveMotionFunc(mouseHandeller);
	glutMainLoop();
	return 0;
}
