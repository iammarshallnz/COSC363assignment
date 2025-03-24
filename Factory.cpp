//  ========================================================================
//  COSC363: Computer Graphics (2025);  MARSHALL INCH.
//
//  FILE NAME: Factory.cpp
//  Note: assignment
//  ========================================================================

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include <cmath>
#include <algorithm>
#include <math.h>
#include <list>
#include "loadTGA.h"
#include "loadBMP.h"
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F // Not defined in my version of OpenGL 

struct particle	{	//A particle 
	int t;			//Life time  (0 - 200)
	float col;		//Color  (0 - 1)
	float size;		//Size   (5 - 25)
	float pos[3];	//Position
	float vel[3];	//Velocity
};


// Partciles
list<particle> parList;	//List of particles

// Board list

list<int> boardList = {0};

//-- Globals ---------------------------------------------------------------


GLuint txId[9]; // Texture ids

// mesh data
float *x, *y, *z;            // vertex coordinates
int *nv, *t1, *t2, *t3, *t4; // number of vertices and vertex indices of each face
int nvert, nface;
float xmin, xmax, ymin, ymax;

// frame rate
int frameRate = 16;
UINT32 frameCount = 0;
// Camera
float cam_x = 0;
float cam_y = 7; // Camera height
float cam_z = 18;
float cam_x_angle = 0;
float cam_y_angle = 0;
float cam_z_angle = 0;
float angle = 0; // Rotation angle for viewing

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

// colours
const float white[4] = {1, 1, 1, 1};
const float black[4] = {0, 0, 0, 1};
const float red[4] = {1, 0.1, 0.1, 1};
// MAHCINE
    float gantZ = 0;
    float gantX = 0;

    float head1 = 0;
    float head2 = 0;



// conveyors
int conv4Count = 0;
int conv3Count = 0;
int conv2Count = 0;
int conv1Count = 0;

bool conv4Pause = true;
bool conv3Pause = true;
bool conv2Pause = true;
bool conv1Pause = true;





void loadTexture()
{
    glGenTextures(9, txId); // Create 8 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]); //

    loadTGA("../Floor.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[1]); // 

    loadTGA("../Capture.tga"); // https://www.surfacemountprocess.com/solder-paste-printing-process.html SOLDER PASTE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D, txId[2]); // 

    loadTGA("../skybox3/Left.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[3]); //

    loadTGA("../skybox3/Front.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[4]); // 

    loadTGA("../skybox3/Right.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[5]); // 

    loadTGA("../skybox3/Back.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[6]); //

    loadTGA("../skybox3/Up.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[7]); // 

    loadTGA("../skybox3/Down.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    
	glBindTexture(GL_TEXTURE_2D, txId[8]);
	loadBMP("../Glow.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glDisable(GL_TEXTURE_2D);
}


// from lab
void drawParticle(float col, float size, float px, float py, float pz) {
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glColor3f(col, col, col);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[8]);

	glPushMatrix();
		glTranslatef(px, py, pz);
		glScalef(size, size, size);

		glBegin(GL_QUADS);
			//A quad on the xy-plane
			glTexCoord2f(0, 0);
			glVertex3f(-0.5, -0.5, 0);
			glTexCoord2f(1, 0);
			glVertex3f(0.5, -0.5, 0);
			glTexCoord2f(1, 1);
			glVertex3f(0.5, 0.5, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-0.5, 0.5, 0);

			//A quad on the yz-plane
			glTexCoord2f(0, 0);
			glVertex3f(0, -0.5, -0.5);
			glTexCoord2f(1, 0);
			glVertex3f(0, 0.5, -0.5);
			glTexCoord2f(1, 1);
			glVertex3f(0, 0.5, 0.5);
			glTexCoord2f(0, 1);
			glVertex3f(0, -0.5, 0.5);
		glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}
// from lab
void newParticle() {
	particle p = { 0 };

	p.pos[0] = 0.5 * (rand() / (float)RAND_MAX) - 0.25 - 5;
	p.pos[1] = 2;	//This point is at the top end of the smoke stack
	p.pos[2] = 0.5 * (rand() / (float)RAND_MAX) - 0.25;

	p.vel[0] = 0.5 * (rand() / (float)RAND_MAX) - 0.25;
	p.vel[1] = 0.0080;
	p.vel[2] = 0.5 * (rand() / (float)RAND_MAX) - 0.25;

	p.col = 1;
	p.size = 0.2;

	parList.push_back(p);
}

//from lab
void updateQueue() {
	const int LIFETIME = 600;
	list<particle>::iterator it;
	particle p;
	int tval;
	float delta;

	//Remove particles that have passed lifetime
	if (!parList.empty()) {
		p = parList.front();
		if (p.t > LIFETIME) parList.pop_front();
	}

	for (it = parList.begin(); it != parList.end(); it++) {
		tval = it->t;
		it->t = tval + 1;
		delta = (float)tval / (float)LIFETIME;
		
		// for (int i = 0; i < 3; i++){
		// 	(it->pos[i]) += it->vel[i];
		
		// }
        (it->pos[0]) =it->pos[1] * 0.1 * sin(it->pos[1]* M_PI * 1) + it->vel[0] - 5;
        (it->pos[1]) += it->vel[1];
        (it->pos[2]) =it->pos[1] * 0.1 * cos(it->pos[1]* M_PI * 1) + it->vel[2];

		it->size = delta * 0.5 + 0.2;	// 
		it->col = 1 - delta;		// 
	}

	   //Create a new particle every sec.
}
//  black box at start
void startingWall()
{
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(9, 0, 0);
    glScalef(2, 1, 1);
    if (wireframe)
    {
        glutWireCube(1);
    }
    else
    {
        glutSolidCube(1);
    }

    glPopMatrix();
}
// black box at end 
void endingWall()
{
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-14.5, 0, 0);
    glScalef(2, 1, 1);
    if (wireframe)
    {
        glutWireCube(1);
    }
    else
    {
        glutSolidCube(1);
    }

    glPopMatrix();
}

// conveyor 
void conveyor(float speed, int convCount)
{
    // 
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glEnable(GL_TEXTURE_2D);
    glNormal3f(0, 1, 0);// up
    glPushMatrix();
    
    glBegin(GL_QUAD_STRIP);

    for (int i = 0; i <= 5; i++) // 1 extra
    {
        glTexCoord2f(0, (float)i / 2.5 + ((float)convCount * speed / 120)); ///
        glVertex3f(i, 0, 1);
        glTexCoord2f(1, (float)i / 2.5 + ((float)convCount * speed / 120)); ///
        glVertex3f(i, 0, 0);
    }
    for (int x = 0; x < 21; x++)
    {
        // right side
        glTexCoord2f(0, (float)(x * 0.1 * M_PI_2) / 10 + ((float)convCount * speed / 120));
        glVertex3f(5 + 0.1 * sin((x * 0.05) * M_PI), 0.1 * cos((x * 0.05) * M_PI) - 0.1, 1);
        glTexCoord2f(1, (float)(x * 0.1 * M_PI_2) / 10 + ((float)convCount * speed / 120));
        glVertex3f(5 + 0.1 * sin((x * 0.05) * M_PI), 0.1 * cos((x * 0.05) * M_PI) - 0.1, 0);
        // Radius is .1, diam is .2
    }
    for (int i = 0; i <= 5; i++) // 1 extra
    {
        glTexCoord2f(0, (float)i / 2.5 + (0.2 * M_PI_2) + ((float)convCount * speed / 120));
        glVertex3f(5 - i, -0.2, 1);
        glTexCoord2f(1, (float)i / 2.5 + (0.2 * M_PI_2) + ((float)convCount * speed / 120));
        glVertex3f(5 - i, -0.2, 0);
    }

    for (int x = 0; x < 21; x++)
    {
        // left side
        glTexCoord2f(0, (float)((x * 0.1 * M_PI_2)) / 10 - (0.2 * M_PI_2) + ((float)convCount * speed / 120));
        glVertex3f(-0.1 * sin((x * 0.05) * M_PI), -0.1 * cos((x * 0.05) * M_PI) - 0.1, 1);
        glTexCoord2f(1, (float)(x * 0.1 * M_PI_2) / 10 - (0.2 * M_PI_2) + ((float)convCount * speed / 120));
        glVertex3f(-0.1 * sin((x * 0.05) * M_PI), -0.1 * cos((x * 0.05) * M_PI) - 0.1, 0);
        // Radius is 1, diam is 2
    }

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
float dynam(float x){
    // goes from -12.5 to -15.5 in oven
    // 0- 2
    if (x < 12.5) return 0;
    else if (x < 15.5) return (float)(x-12.5) * 2/3;
    else return 2;

}

void paste(float pcbX)
{
    // an paste object 
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(0, 0.05, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glPushMatrix();
    // QUADS 
    // left side 
    
    for (int y = 0; y < 5; y++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int x = 0; x < 5; x++)
    {
        
        glNormal3f(0, 0, 1);
        glTexCoord2f(x/5.0, (y/ 5.0));
        glVertex3f(x, y *(1 + 0.5 * dynam(-pcbX + (x /4.0))) * 0.1, 0);
        glNormal3f(0, 0, 1);
        glTexCoord2f(x/5.0, ((y + 1)/ 5.0));
        glVertex3f(x, (y + 1) * (1 + 0.5 * dynam(-pcbX + (x /4.0))) * 0.1, 0);
    }
    glEnd();
    }
    // right side 
    
    for (int y = 0; y < 5; y++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int x = 0; x < 5; x++)
    {
        
        glNormal3f(0, 0, -1);
        glTexCoord2f(x/5.0, (y/ 5.0));
        glVertex3f(x, y *(1 + 0.5 * dynam(-pcbX + (x /4.0))) * 0.1, -1);
        glNormal3f(0, 0, -1);
        glTexCoord2f(x/5.0, ((y + 1)/ 5.0));
        glVertex3f(x, (y + 1) * (1 + 0.5 * dynam(-pcbX + (x /4.0))) * 0.1, -1);
    }
    glEnd();
    }
    // front side 
    
    for (int y = 0; y < 5; y++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int z = 0; z < 2; z++)
    {   
        glNormal3f(-1, 0, 0);
        glTexCoord2f(y/5.0, z);
        glVertex3f(0, y *(1 + 0.5 * dynam(-pcbX))* 0.1, z- 1);
        glNormal3f(-1, 0, 0);
        glTexCoord2f((y + 1.0)/5.0, z);
        glVertex3f(0, (y + 1) * (1 + 0.5 * dynam(-pcbX))* 0.1, z- 1);
    }
    glEnd();
    }
    // back side 
    
    for (int y = 0; y < 5; y++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int z = 0; z < 2; z++)
    {
        glNormal3f(1, 0, 0);
        glTexCoord2f(y/5.0, z);
        glVertex3f(4, y *(1 + 0.5 * dynam(-pcbX + 1))* 0.1, z- 1);

        glNormal3f(1, 0, 0);
        glTexCoord2f((y + 1)/5.0, z);
        glVertex3f(4, (y + 1) * (1 + 0.5 * dynam(-pcbX + 1))* 0.1, z- 1);
    }
    glEnd();
    }
    // ROOF side 
    
    for (int z = 0; z < 1; z++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int x = 0; x < 5; x++)
    {
        glNormal3f(0, 1, 0); 
        glTexCoord2f(x/ 5.0, z);
        glVertex3f(x, 5 *(1 + 0.5 * dynam(-pcbX + (x /4.0)))* 0.1, z);
        glNormal3f(0, 1, 0);
        glTexCoord2f(x / 5.0, z - 1);
        glVertex3f(x, (5) * (1 + 0.5 * dynam(-pcbX + (x /4.0)))* 0.1, z- 1);
    }
    glEnd();
    }

    glPopMatrix();

    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void drawPcb(float pcbX)
{
    // pcb is a cube scalled down
    // create each paste object and give ofsets 
    glPushMatrix();

    glTranslatef(9 + pcbX, 0.05, 0); // past cover starting

    glPushMatrix();

    glTranslatef(-0.166, 0.05, -0.18); // line up bottom cap paste
    glRotatef(-90, 0, 1, 0);
    glScalef(.02, 0.02, .033);
    paste(pcbX-0.166);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(-0.134, 0.05, 0.18); // line up TOP cap paste
    glRotatef(90, 0, 1, 0);
    glScalef(.02, 0.02, .033);
    paste(pcbX-0.134);
    glPopMatrix();


    glPushMatrix();

    glTranslatef(0, 0.05, -0.199); // line up bot resistor paste
    //glRotatef(90, 0, 1, 0);
    glScalef(0.02, 0.02, .105);
    paste(pcbX);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.3, 0.05, -0.301); // line up top resistor paste
    glRotatef(180, 0, 1, 0);
    glScalef(0.02, 0.02, .105);
    paste(pcbX + 0.3);
    glPopMatrix();


    glColor3f(0.5, 1, 0.5);

    glScalef(1, 0.099, 0.8);
    if (wireframe)
    {
        glutWireCube(1);
    }
    else
    {
        glutSolidCube(1);
    }

    glPopMatrix();
}
void machineRails()
{   
    // solid objects 
    // left rail
    glPushMatrix();
    glColor3f(0.3, 0.7, 0.5);
    glTranslatef(0, 1, 0);
    glScalef(1, 0.5, 4.5);
    if (wireframe)
        glutWireCube(0.5);
    else
        glutSolidCube(0.5);
    glPopMatrix();
    // left support
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0.2, 1.1, 0);
    glScalef(0.999, 1, 21.999);
    if (wireframe)
        glutWireCube(0.1);
    else
        glutSolidCube(0.1);
    glPopMatrix();

    // right rail
    glPushMatrix();
    glColor3f(0.3, 0.7, 0.5);
    glTranslatef(3, 1, 0);
    glScalef(1, 0.5, 4.5);
    if (wireframe)
        glutWireCube(0.5);
    else
        glutSolidCube(0.5);
    glPopMatrix();

    // rigth support
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(2.8, 1.1, 0);
    glScalef(0.999, 1, 21.999);
    if (wireframe)
        glutWireCube(0.1);
    else
        glutSolidCube(0.1);
    glPopMatrix(); // left rail
    glPushMatrix();
    glColor3f(0.3, 0.7, 0.5);
    glTranslatef(0, 1, 0);
    glScalef(1, 0.5, 4.5);
    if (wireframe)
        glutWireCube(0.5);
    else
        glutSolidCube(0.5);
    glPopMatrix();
    // left support
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0.2, 1.1, 0);
    glScalef(0.999, 1, 21.999);
    if (wireframe)
        glutWireCube(0.1);
    else
        glutSolidCube(0.1);
    glPopMatrix();

    // right rail
    glPushMatrix();
    glColor3f(0.3, 0.7, 0.5);
    glTranslatef(3, 1, 0);
    glScalef(1, 0.5, 4.5);
    if (wireframe)
        glutWireCube(0.5);
    else
        glutSolidCube(0.5);
    glPopMatrix();

    // rigth support
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(2.8, 1.1, 0);
    glScalef(0.999, 1, 21.999);
    if (wireframe)
        glutWireCube(0.1);
    else
        glutSolidCube(0.1);
    glPopMatrix();
}
void machineCenter(){
    // centre bar movement (z movement)s
    // global 
    glPushMatrix();
    glColor3f(0.4, 0.1, 0.4);
    glTranslatef(1.5, 1.2, 0 + gantZ);

    glScalef(6, 0.2, 0.2);

    if (wireframe)
        glutWireCube(0.5);
    else
        glutSolidCube(0.5);

    glPopMatrix();

    glPushMatrix(); // gantry
    glColor3f(0.4, 0.1, 0.1);
    glTranslatef(1.5 + gantX, 1.2, gantZ);
    if (wireframe)
        glutWireCube(0.5);
    else
        glutSolidCube(0.5);

    glPopMatrix();

    glPushMatrix(); // head 1
    glColor3f(0.9, 0.9, 0.9);
    glTranslatef(1.5 - 0.15 + gantX, 1.4499 + head1, gantZ);
    glRotatef(90, 1, 0, 0);
    if (wireframe)
        glutWireCylinder(0.05, 0.75, 20, 20);
    else
        glutSolidCylinder(0.05, 0.75, 20, 20);

    glPopMatrix();

    glPushMatrix(); // head 2
    glColor3f(0.9, 0.9, 0.9);
    glTranslatef(1.5 + 0.15 + gantX, 1.4499 + head2, gantZ);
    glRotatef(90, 1, 0, 0);
    if (wireframe)
        glutWireCylinder(0.05, 0.75, 20, 20);
    else
        glutSolidCylinder(0.05, 0.75, 20, 20);

    glPopMatrix();



}

void resistor(float capX, float capY, float capZ, float rotateRes)
{
    // hidden box
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(1.7, -0.21, 0.75); // stop clashing

    glScalef(0.3, 1, 0.3);

    if (wireframe)
        glutWireCube(1);
    else
        glutSolidCube(1);

    glPopMatrix();

    glPushMatrix(); // global

    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(1.7 + capX, 0.51 + capY, 0.75 + capZ); // position
    glRotatef(rotateRes, 0, 1, 0);
    glPushMatrix(); //  cap middle
    glTranslatef(0, -0.2, 0);
    glScalef(1, 0.2, 1.2);
    if (wireframe)
        glutWireCube(0.1);
    else
        glutSolidCube(0.1);

    glPopMatrix(); // cap middle

    glPushMatrix(); //  cap front
    glTranslatef(0, -0.2, 0.075);
    glScalef(1, 0.2, 0.3);
    glColor3f(0.8, 0.8, 0.8);
    if (wireframe)
        glutWireCube(0.1);
    else
        glutSolidCube(0.1);

    glPopMatrix(); // cap front

    glPushMatrix(); //  cap back
    glTranslatef(0, -0.2, -0.075);
    glScalef(1, 0.2, 0.3);
    glColor3f(0.8, 0.8, 0.8);
    if (wireframe)
        glutWireCube(0.1);
    else
        glutSolidCube(0.1);

    glPopMatrix(); // cap back

    glPopMatrix(); // global
}

void cappacator(float capX, float capY, float capZ)
{

    // hidden box
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(1.2, -0.21, 0.75); // stop clashing
    glScalef(0.3, 1, 0.3);

    if (wireframe)
        glutWireCube(1);
    else
        glutSolidCube(1);

    glPopMatrix();

    glPushMatrix(); // CAPP 1

    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(1.2 + capX, 0.505 + capY, 0.75 + capZ); // position
    glPushMatrix();                                      // top of cap
    glRotatef(90, 1, 0, 0);
    if (wireframe)
        glutWireCylinder(0.1, 0.2, 20, 20);
    else
        glutSolidCylinder(0.1, 0.2, 20, 5);
    glPopMatrix();
    glPushMatrix(); // bottom of cap
    glColor3f(0, 0.3, 0.3);
    glTranslatef(0, -0.2, 0.02);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    glVertex3f(0.1, 0, 0.1);
    glVertex3f(-0.1, 0, 0.1);
    glVertex3f(-0.15, 0, 0.05);
    glVertex3f(-0.15, 0, -0.15);
    glVertex3f(0.15, 0, -0.15);
    glVertex3f(0.15, 0, 0.05);
    glVertex3f(0.1, 0, 0.1);

    glEnd();
    // MAKE LEGS
    // front leg
    glPushMatrix();
    glColor3f(0.8, 0.8, 0.8);
    glTranslatef(0, 0, 0.075);
    glScalef(0.03, 0.005, 0.1);
    if (wireframe)
        glutWireCube(1);
    else
        glutSolidCube(1);

    glPopMatrix();
    // back leg
    glPushMatrix();
    glColor3f(0.8, 0.8, 0.8);
    glTranslatef(0, 0, -0.12);
    glScalef(0.03, 0.005, 0.1);
    if (wireframe)
        glutWireCube(1);
    else
        glutSolidCube(1);

    glPopMatrix();

    glPopMatrix();

    glPopMatrix(); // cap 1glPushMatrix(); // CAPP 1
}
void ceilingLight()
{
    const int N = 25;                 // Number of vertices on base curve
    float toRadians = M_PI / 180.0;   // Conversion from degrees to radians
    float angStep = 10.0 * toRadians; // Rotate base curve in 10 deg steps
    int nSlices = 36;                 // 36 slices at 10 deg intervals

    float vx[N], vy[N], vz[N]; // vertex positions
    float wx[N], wy[N], wz[N];
    float nx[N], ny[N], nz[N]; // normal vectors
    float mx[N], my[N], mz[N];

    for (int i = 0; i < N; i++)
    { // Initialize data everytime the frame is refreshed

        float x = i / 50.f; // 25/50 
        float y = -2 * (x * x); /// negatve parabloid 
        float dx = -x; // derivative for normal calculation
        float dy = 2.0f;
        vx[i] = x;
        vy[i] = y;
        vz[i] = 0;
        nx[i] = dx;
        ny[i] = dy;
        nz[i] = 0;
    }

    glPushMatrix();      
                   // global
    glRotatef(frameCount / 0.5f, 0, 1, 0); // Rotate the light fixture
    glTranslatef(0, 6, 0);                // move up 6
    glRotatef(10, 1, 0, 0);               // rotate 10 deg
    glTranslatef(0, -3.0, 0);             // move down 3

    glPushMatrix(); // parabolic

    glColor3f(0.8, 0.8, 0.8); // gray colour

    for (int j = 0; j < nSlices; j++)
    {
        for (int i = 0; i < N; i++)
        {
            wx[i] = cos(angStep) * vx[i] + sin(angStep) * vz[i];
            wy[i] = vy[i];
            wz[i] = -sin(angStep) * vx[i] + cos(angStep) * vz[i];

            mx[i] = cos(angStep) * nx[i] + sin(angStep) * nz[i];
            my[i] = ny[i];
            mz[i] = -sin(angStep) * nx[i] + cos(angStep) * nz[i];
        }

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i < N; i++)
        {
            glNormal3f(nx[i], ny[i], nz[i]);
            
            glVertex3f(vx[i], vy[i], vz[i]);
            glNormal3f(mx[i], my[i], mz[i]);
            
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        glEnd();

        for (int i = 0; i < N; i++)
        { // Update vertices
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
            nx[i] = mx[i];
            ny[i] = my[i];
            nz[i] = mz[i];
        }
    }

    glPopMatrix(); // parbolic

    float emis[3] = {0.8, 0.8, 0};
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);

    glPushMatrix(); // light bulb
    GLfloat spotCutoff[] = {30.0f};
    GLfloat spotExponent[] = {50.0f};
    float spotDirection[] = {0, -1, 0};
    float lightPos[] = {0, 0, 0, 1};
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spotCutoff);
    glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, spotExponent);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
    glColor3f(1, 1, 0);       // Yellow color
    glTranslatef(0, -0.2, 0); // Position the light bulb
    if (wireframe)
        glutWireSphere(0.2, 20, 20);
    else
        glutSolidSphere(0.2, 20, 20);

    glPopMatrix(); // light bulb
    glMaterialfv(GL_FRONT, GL_EMISSION, black);
    glPushMatrix();           // cord
    glColor3f(0.3, 0.3, 0.3); // Dark gray color
    glTranslatef(0, -0, 0);   // Position the cord
    glRotatef(-90, 1, 0, 0);  // Rotate the cord
    if (wireframe)
        glutWireCylinder(0.01, 3, 20, 5);
    else
        glutSolidCylinder(0.01, 3, 20, 5);

    glPopMatrix(); // cord

    glPopMatrix(); // global
}

void oven()
{
    // BASE
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    glTranslatef(-5, 1.5, 0);
    glScalef(4, 0.5, 1);

    if (wireframe)
        glutWireCube(1);
    else
        glutSolidCube(1);

    glPopMatrix();

    glPushMatrix(); // exhaust
    glColor3f(0.2, 0.2, 0.2);
    glTranslatef(-5, 2, 0);
    glRotatef(90, 1, 0, 0);

    if (wireframe)
        glutWireCylinder(0.5, 0.2, 20, 4);
    else
        glutSolidCylinder(0.5, 0.2, 20, 4);

    glPopMatrix();





    // ELEMENTS
    glPushMatrix();
    GLfloat spotCutoff[] = {30.0f};
    GLfloat spotExponent[] = {20.0f};
    float spotDirection[] = {0, -1, 0};
    float lightPos2[4] = {-5.5, 1.2, 0, 1};
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
    glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutoff);
    glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);

    float lightPos3[4] = {-4, 1.2, 0, 1};
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
    glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, spotCutoff);
    glLightfv(GL_LIGHT3, GL_SPOT_EXPONENT, spotExponent);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);

    float emis[3] = {0.8, 0, 0};
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);

    glColor3f(0.8, 0.1, 0.1);
    glTranslatef(-6.5, 1.25, 0.25);

    glRotatef(90, 0, 1, 0);
    if (wireframe)
        glutWireCylinder(0.2, 3, 20, 20);
    else
        glutSolidCylinder(0.2, 3, 20, 20);

    glPopMatrix();
    glPushMatrix();
    // chimney 
    glColor3f(0.8, 0.1, 0.1);
    glTranslatef(-6.5, 1.25, -0.25);
    glRotatef(90, 0, 1, 0);
    if (wireframe)
        glutWireCylinder(0.2, 3, 20, 20);
    else
        glutSolidCylinder(0.2, 3, 20, 20);

    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_EMISSION, black);
}
void smtMachine(int adjust)
{
    // called by each board 
    // large if statements that are called at certain frames 
    // basically a state machine.
    // every thing in here is replicated per board e.g re generate reistor but not the actual machine
    // which uses adjustable globals 
    float conv4Speed = 1.9;
    float conv3Speed = 0.8;
    float conv2Speed = 1.3;
    float conv1Speed = 1.9;

    float pcbX = 0;

    gantZ = 0;
    gantX = 0;

    head1 = 0;
    head2 = 0;

    float capY = 0;
    float capX = 0;
    float capZ = 0;

    float resY = 0;
    float resX = 0;
    float resZ = 0;

    float offset = adjust;
    float rotateRes = 0;

    if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 360 + 60)
    { // MOVE TO DEATH BOX 
        float moveX = 7.5;

        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 360 + 60;
        if (frameCount < offset + 210)
        {

            conv4Pause = false;
        }else conv4Pause = true;
        if (frameCount < offset + 60)
        {

            conv3Pause = false;
        } else conv3Pause = true;
        pcbX = -10.5 - 5.5 - moveX * (std::clamp((float)(frameCount - offset) / 240.0f, 0.0f, 1.0f));
        head1 = 0;
        head2 = 0;
        rotateRes = 90;
        resX = 0 - 0.05 - 1.5 - 5.5 - moveX * (std::clamp((float)(frameCount - offset) / 240.0f, 0.0f, 1.0f));
        resY = 0 - 0.2;
        resZ = -1;
        capX = 0.15 - 1.5 - 5.5 - moveX * (std::clamp((float)(frameCount - offset) / 240.0f, 0.0f, 1.0f));
        capY = -0.19;
        capZ = -0.75;
        gantZ = 0;
        gantX = 0;
        
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60)
    { // PCB MOVE into oven untill end 360 frames in oven,
        float moveX = 5.5;

        if (frameCount % 5 == 0) newParticle();// generAte smoke in oven


        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60;
        if (frameCount < offset + 360)
        {

            conv3Pause = false;
        }else conv3Pause = true;
        if (frameCount < offset + 60)
        {

            conv2Pause = false;
        } else conv2Pause = true;
        pcbX = -10.5 - moveX * (std::clamp((float)(frameCount - offset) / 360.0f, 0.0f, 1.0f));
        head1 = 0;
        head2 = 0;
        rotateRes = 90;
        resX = 0 - 0.05 - 1.5 - moveX * (std::clamp((float)(frameCount - offset) / 360.0f, 0.0f, 1.0f));
        resY = 0 - 0.2;
        resZ = -1;
        capX = 0.15 - 1.5 - moveX * (std::clamp((float)(frameCount - offset) / 360.0f, 0.0f, 1.0f));
        capY = -0.19;
        capZ = -0.75;
        gantZ = 0;
        gantX = 0;
        
    }
    else if (frameCount- adjust >= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60)
    { // PCB MOVE to machine exit

        float moveX = 1.5;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60;

        if (frameCount < offset + 60)
        {

            conv2Pause = false;
            conv2Speed = 1.3;
        } else conv2Pause = true;

        pcbX = -9 - moveX * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        head1 = 0;
        head2 = 0;
        rotateRes = 90;
        resX = 0 - 0.05 - moveX * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        resY = 0 - 0.2;
        resZ = -1;
        capX = 0.15 - moveX * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        capY = -0.19;
        capZ = -0.75;
        gantZ = 0;
        gantX = 0;
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60)
    { // MOVE CENTer
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60;
        head1 = 0;
        head2 = 0;
        rotateRes = 90;
        resX = 0 - 0.05;
        resY = 0 - 0.2;
        resZ = -1;
        capX = 0.15;
        capY = -0.19;
        capZ = -0.75;
        gantZ = -0.25 + 0.25 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        gantX = 0;
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60)
    { // DROP  resistor
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60;
        head1 = 0;
        head2 = -0.4 + 0.4 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        rotateRes = 90;
        resX = 0 - 0.05;
        resY = 0 - 0.2 * (std::clamp((float)(frameCount - offset) * 3 / 60.0f, 0.0f, 1.0f)); // speed up falling to look more natural
        resZ = -1;
        capX = 0.15;
        capY = -0.19;
        capZ = -0.75;
        gantZ = -0.25;
        gantX = 0;
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60)
    { // place resistor
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60;
        head1 = 0;
        head2 = -0.4 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        rotateRes = 90;
        resX = 0 - 0.05;
        resY = 0.4 - 0.4 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        resZ = -1;
        capX = 0.15;
        capY = -0.19;
        capZ = -0.75;
        gantZ = -0.25;
        gantX = 0;
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60)
    { // move gantry spin resistor, cap placed
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60;
        head1 = 0;
        head2 = 0;
        rotateRes = std::clamp((float)(frameCount - offset) * 1.5f, .0f, 90.f);
        resX = 0 - 0.05;
        resY = 0.4;
        resZ = -0.75 - 0.25 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        capX = 0.15;
        capY = -0.19;
        capZ = -0.75;
        gantZ = -0.25 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        gantX = 0;
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60)
    { // head 1 up
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60;
        head1 = -0.4 + 0.4 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        head2 = 0;

        resX = 0 - 0.05;
        resY = 0.4;
        resZ = -0.75;
        capX = 0.15;
        capY = -0.19;
        capZ = -0.75;
        gantZ = 0;
        gantX = 0;
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60)
    { // head 1 down place cap
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60;
        head1 = -0.39 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        head2 = 0;

        resX = 0 - 0.05;
        resY = 0.4;
        resZ = -0.75;
        capX = 0.15;
        capY = 0.2 - 0.39 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        capZ = -0.75;
        gantZ = 0;
        gantX = 0;
    }

    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60)
    { // gantery to mid
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60;
        head1 = 0;
        head2 = 0;
        resX = 0 - 0.05 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f);
        resY = 0.4;
        resZ = 0 - 0.75 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f);
        capX = 0.2 - (0.05 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f));
        capY = 0.2;
        capZ = 0 - 0.75 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f);
        ;
        gantZ = (0.75) - 0.75 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f);
        gantX = (0.05) - 0.05 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f);
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60)
    { // head 2 up
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60;
        head2 = (-0.4 * (1 - std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f)));
        resY = 0.4 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f);
        capX = 0.2;
        capY = 0.2;
        capZ = 0;
        gantZ = (0.75);
        gantX = (0.05);
    }
    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120 + 60)
    { // head2 down
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120 + 60;

        capX = 0.2;
        capY = 0.2;
        capZ = 0;
        resY = 0;
        head2 = (-0.4 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f)));
        gantZ = (0.75);
        gantX = (0.05);
    }

    else if (frameCount - adjust>= 120 + 60 + 60 + 120 + 120 + 120)
    { // move centre to next part
        pcbX = -9;
        offset += 120 + 60 + 60 + 120 + 120 + 120;
        head1 = -0.001; // hide clashing

        gantZ = (0.75);
       
        gantX = (-0.15) + (0.2 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f)));
        
        capX = (0.2 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f)));

        capY = 0.2;
        capZ = 0;
        
    }
    else if (frameCount - adjust>= 120 + 60 + 120 + 120 + 120)
    { // head up
        pcbX = -9;
        offset += 120 + 60 + 120 + 120 + 120;
        head1 = (-0.2 * (1 - std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f)));
        capY = 0.2 * std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f);
        gantZ = (0.75);
        gantX = (-0.15);
    }
    else if (frameCount- adjust >= 120 + 120 + 120 + 120)
    { // head down
        offset += 120 + 120 + 120 + 120;
        capY = 0;
        pcbX = -9;
        head1 = (-0.2 * (std::clamp((float)(frameCount - offset) / 60.0f, 0.0f, 1.0f)));
        gantZ = (0.75);
        gantX = (-0.15);
    }
    else if (frameCount- adjust >= 120 + 120 + 120)
    { // move to part
        offset += 120 + 120 + 120;
        head1 = 0;
        capY = 0;
        pcbX = -9;
        gantZ = (0.75 * (std::clamp((float)(frameCount - offset) / 120.0f, 0.0f, 1.0f)));
        gantX = (-0.15 * (std::clamp((float)(frameCount - offset) / 120.0f, 0.0f, 1.0f)));
        conv2Pause = true;
    }
    else if (frameCount- adjust >= 120 + 60)
    {
        // 9 - 9 pcbX = -9
        offset += 120 + 60;
        if (frameCount - adjust< 120 + 120)
        {
            conv1Pause = false;

            conv2Pause = false;
            
        }
        else if (frameCount - adjust< 120 + 120 + 60)
        {
            conv2Pause = false;
            conv1Pause = true;
        }

        pcbX = -5.5 - 3.5 * (std::clamp((float)(frameCount - offset) / 120, 0.0f, 1.0f));
    }

    else if (frameCount - adjust >= 150)
    {
        conv1Pause = true;
        pcbX = -5.5;
    }
    else
    { // wait for board
        conv1Pause = false;
        
        head1 = 0;
        head2 = 0;
        pcbX = -5.5 * (std::clamp((float)(frameCount - offset) / 150, 0.0f, 1.0f));
        capY = -0.3 + (0.3 * (std::clamp((float)(frameCount - offset) / 120.0f, 0.0f, 1.0f)));
        resY = -0.3 + (0.3 * (std::clamp((float)(frameCount - offset) / 120.0f, 0.0f, 1.0f)));
        gantZ = 0;
        gantX = 0;
    }

    // CONVEYORS
    glPushMatrix(); // conveyer 4
    glTranslatef(-13, 0, -0.5);
    conveyor(conv4Speed, conv4Count);
    glPopMatrix();
    glPushMatrix(); // conveyer 3
    glTranslatef(-7.75, 0, -0.5);
    conveyor(conv3Speed, conv3Count);
    glPopMatrix();

    glPushMatrix(); // conveyer 2
    glTranslatef(-2.5, 0, -0.5);
    conveyor(conv2Speed, conv2Count);
    glPopMatrix();
    glPushMatrix(); // conveyer 1
    glTranslatef(2.75, 0, -0.5);
    conveyor(conv1Speed, conv1Count);
    glPopMatrix();

    // OVEN
    oven();

    /// PCB draw func

    drawPcb(pcbX);

    glPushMatrix(); // global

    glTranslatef(-1.5, -0, 0);

    machineRails(); // fuc to hide consts

    

    cappacator(capX, capY, capZ);

    resistor(resX, resY, resZ, rotateRes);

    glPopMatrix(); // final
}

void skybox() {
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    
    // Up face (y = 1.0f) INVERTED NORMALS 
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f,  1.0f);
    glEnd();

    // Bottom face (y = -1.0f)
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();

    // Front face (z = 1.0f)
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f);
    glEnd();

    // Back face (z = -1.0f)
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glEnd();

    // Left face (x = -1.0f)
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();

    // Right face (x = 1.0f)
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}


void display()
{
    float lpos[4] = {0., 100., 0., 1.}; // light's position
    if (wireframe)
    {
        glDisable(GL_LIGHTING); // Disable lighting
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_LIGHTING);
    }
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE = Wireframe;   GL_FILL = Solid

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // roate every thing 
    glRotatef(cam_x_angle, 1, 0., 0.); 
    glRotatef(cam_y_angle, 0., 1., 0.);
    gluLookAt(cam_x, cam_y, cam_z, cam_x, cam_y, cam_z - 1, 0., 1., 0.); // look in front. 
    
    float gray[]  = {0.4,0.4,0.4, 1};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);
    glLightfv(GL_LIGHT0, GL_POSITION, lpos); // Set light position
    glLightfv(GL_LIGHT4, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT4, GL_POSITION, lpos); // Set light position

    glEnable(GL_LIGHT4); // lighting for skybox
    // sky box 
    glPushMatrix();
    glTranslatef(cam_x, cam_y, cam_z); 
    glScalef(10000, 10000, 10000);     
    skybox();
    glPopMatrix();

    glDisable(GL_LIGHT4);

    
    startingWall();
    endingWall();
    ceilingLight();
    glPushMatrix();
    glTranslatef(-1.5, -0, 0);
    
    machineCenter();    
    glPopMatrix();
    // BOARD LIST
    if (frameCount - boardList.front() > 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 360 + 360)
    {
        boardList.pop_front();
    }
    
    if (frameCount - boardList.back() > 120 + 60 + 60 + 120 + 120 + 120 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60 + 60){
        boardList.push_back(frameCount);
    }
    for (int i : boardList)
    {   
        smtMachine(i);
    }
    
    // PARTICLE LIST 
    list<particle>::iterator it;
	for (it = parList.begin(); it != parList.end(); it++) {
		drawParticle(it->col, it->size, it->pos[0], it->pos[1], it->pos[2]);
	}
    
    glutSwapBuffers(); 
}
void centerMouse()
{
    glutWarpPointer(windowWidth / 2, windowHeight / 2);
}
void cameraCalculator()
{

    
    float yAngleRad = cam_y_angle * M_PI / 180.0f;
    float xAngleRad = cam_x_angle * M_PI / 180.0f;

    
    float forwardX = sin(yAngleRad) * cos(xAngleRad);
    float forwardY = -sin(xAngleRad);
    float forwardZ = -cos(yAngleRad) * cos(xAngleRad);

   
    float X = cos(yAngleRad);
    float Z = sin(yAngleRad);

    // Movement speed
    float speed = 0.1f;

    // Apply movement based on key presses
    if (keyPress & 0b10000000)
    { // W
        cam_x += forwardX * speed;
        cam_y += forwardY * speed;
        cam_z += forwardZ * speed;
    }
    if (keyPress & 0b00100000)
    { // S
        cam_x -= forwardX * speed;
        cam_y -= forwardY * speed;
        cam_z -= forwardZ * speed;
    }
    if (keyPress & 0b01000000)
    { // A
        cam_x -= X * speed;
        cam_z -= Z * speed;
    }
    if (keyPress & 0b00010000)
    { // D
        cam_x += X * speed;
        cam_z += Z * speed;
    }
}
void timer(int value)
{
    // always do the camera calculation for paused movement
    cameraCalculator();
    if (pause)
    {   

        glutTimerFunc(16, timer, 0);
        glutPostRedisplay();
        return;
    }

    frameCount++;

    

    if (!conv1Pause)
        conv1Count++;
    if (!conv2Pause)
        conv2Count++;
    if (!conv3Pause)
        conv3Count++;
    if (!conv4Pause)
        conv4Count++;
    updateQueue(); // PArticle 
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
//-- Initialize OpenGL parameters ------------------------------------------
void initialize()
{
    glClearColor(1., 1., 1., 1.); // Background colour

    glEnable(GL_LIGHTING); // Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    float yellowish[] = {0.8, 0.8, 0.1, 1};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowish);
    glLightfv(GL_LIGHT1, GL_SPECULAR, yellowish);
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, red);
    glLightfv(GL_LIGHT2, GL_SPECULAR, red);
    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, red);
    glLightfv(GL_LIGHT3, GL_SPECULAR, red);
    
    glLightfv(GL_LIGHT4, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT4, GL_SPECULAR, white);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    loadTexture();
    gluPerspective(80., 1.6, 0.01, 20000.); // Increased far clipping plane
}

// detect key press
void keyboardUp(unsigned char key, int x, int y)
{ // key up
    // using bits register key press
    if ('w' == key)
        keyPress &= 0b01111111;
    if ('a' == key)
        keyPress &= 0b10111111;
    if ('s' == key)
        keyPress &= 0b11011111;
    if ('d' == key)
        keyPress &= 0b11101111;

    glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{ // "key down"
    // using bits register key press
    // bit shifting thing
    const float step = 0.1;
    if ('w' == key)
        keyPress |= 0b10000000;
    if ('a' == key)
        keyPress |= 0b01000000;
    if ('s' == key)
        keyPress |= 0b00100000;
    if ('d' == key)
        keyPress |= 0b00010000;
    if (' ' == key)
        pause = !pause; // pause the movement
    if ('f' == key)
    {
        frameCount += 10;
    }
    
    if ('q' == key)
    {
        wireframe = !wireframe;
    }
    if (27 == key)
        exit(0);
    glutPostRedisplay();
}

//-- Special key event callback --------------------------------------------
//-- To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{ // special key event
    if (key == GLUT_KEY_LEFT)
        cam_y_angle--;
    else if (key == GLUT_KEY_RIGHT)
        cam_y_angle++;
    else if (key == GLUT_KEY_UP)
        cam_x_angle--;
    else if (key == GLUT_KEY_DOWN)
        cam_x_angle++;
    if (key == GLUT_KEY_F1)
        exit(0);

    glutPostRedisplay();
}

// does mouise handeling 
void mouseHandeller(int x, int y)
{

    
    float deltaX = (x - windowWidth / 2) * mouseSensitivity;// mouse pos change 
    float deltaY = (y - windowHeight / 2) * mouseSensitivity;// 
    
    // increment globals
    cam_y_angle += deltaX;
    cam_x_angle += deltaY;

    cam_x_angle = std::clamp((float)cam_x_angle,-89.0f,89.0f); // clamp 
    
   
    centerMouse(); // center 

    glutPostRedisplay();
}

//-- Main: Initialize glut window and register call backs ------------------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Assignment");
    initialize();
    glutTimerFunc(frameRate, timer, 0); 
    glutDisplayFunc(display);           
    glutSpecialFunc(special);           
    glutKeyboardFunc(keyboard);         
    glutKeyboardUpFunc(keyboardUp);     
    glutPassiveMotionFunc(mouseHandeller);
    glutMainLoop();
    return 0;
}
