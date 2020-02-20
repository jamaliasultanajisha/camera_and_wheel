#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <bits/stdc++.h>
//#include <GL/glut.h>
#ifdef __linux
#include <GL/glut.h>
#else
#include <windows.h>
#include <glut.h>
#endif

using namespace std;

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double expand;
double max_expand;

struct point
{
	double x,y,z;
};
struct point pos,u,r,l;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawCube() 
{
	glColor3f(1, 1, 1);
	for (int i = 0; i<25; i++)
	{
		glPushMatrix();
	}

	glTranslated(0, 0, max_expand);
	drawSquare(expand);

	glPopMatrix();
	glTranslated(0, 0, -max_expand);
	drawSquare(expand);

	glPopMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(0, 0, max_expand);
	drawSquare(expand);

	glPopMatrix();
	glRotated(-90, 1, 0, 0);
	glTranslated(0, 0, max_expand);
	drawSquare(expand);

	glPopMatrix();
	glRotated(90, 0, 1, 0);
	glTranslated(0, 0, max_expand);
	drawSquare(expand);

	glPopMatrix();
	glRotated(-90, 0, 1, 0);
	glTranslated(0, 0, max_expand);
	drawSquare(expand);
}

void drawCylinder(double radius,double height, int slices, int stacks)
{
	struct point points[100][100];
	int i, j;
	double h, r;
	glColor3f(0, 1, 0);
	//generate points
	for (i = 0; i <= stacks; i++)
	{
		r = radius;
		h = height * sin(((double)i / (double)stacks)*(pi / 2));
		for (j = 0; j <= slices; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices)*.5*pi);
			points[i][j].y = r * sin(((double)j / (double)slices)*.5*pi);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i<stacks; i++)
	{
		for (j = 0; j<slices; j++)
		{
			glBegin(GL_QUADS); {
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				//lower hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
			}glEnd();
		}
	}
}

void drawfullCylinder()
{
	double r = max_expand - expand;
	double d = expand;

	glPushMatrix();
	{
		glTranslatef(d, d, -d - r);
		drawCylinder(r, d, 50, 50);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-d, d, -d - r);
		glRotated(90, 0, 0, 1);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-d, -d, -d - r);
		glRotated(180, 0, 0, 1);
		drawCylinder(r, d, 50, 50);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(d, -d, -d - r);
		glRotated(-90, 0, 0, 1);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, d, -r);
		glRotated(-90, 0, 1, 0);
		drawCylinder(r, d, 50, 50);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0, d, -d - d - r);
		glRotated(90, 0, 1, 0);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, -d, -d - d - r);
		glRotated(-90, 1, 0, 0);
		glRotated(90, 0, 1, 0);
		drawCylinder(r, d, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, -d, d - d - r);
		glRotated(90, 1, 0, 0);
		glRotated(-90, 0, 1, 0);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(d, 0, d - d - r);
		glRotated(90, 1, 0, 0);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(d, 0, -d - d - r);
		glRotated(-90, 1, 0, 0);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-d, 0, d - d - r);
		glRotated(-90, 0, 1, 0);
		glRotated(90, 1, 0, 0);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-d, 0, -d - d - r);
		glRotated(90, 0, 1, 0);
		glRotated(-90, 1, 0, 0);
		drawCylinder(r, d, 50, 50); 
	}
	glPopMatrix();
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1.0,0.0,0.0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
			}glEnd();
		}
	}
}

void drawfullSphere()
{
	double slices = 50; 
	double stacks = 50;
	double d = expand;
	double r = max_expand - expand;
	glPushMatrix();
	{
		glTranslatef(d, d, d);
		drawSphere(r, slices, stacks); //x y z
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-d, d, d);
		glRotated(90, 0, 0, 1);
		drawSphere(r, slices, stacks);  //-x y z
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-d, -d, d);
		glRotated(180, 0, 0, 1);
		drawSphere(r, slices, stacks);  //-x -y z
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(d, -d, d);
		glRotated(-90, 0, 0, 1);
		drawSphere(r, slices, stacks);  //x -y z
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(d, d, -d);
		glRotated(180, 1, 1, 0);
		drawSphere(r, slices, stacks);  //x y -z
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-d, d, -d);
		glRotated(90, 0, 0, 1);
		glRotated(180, 1, 1, 0);
		drawSphere(r, slices, stacks);  //-x y -z
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-d, -d, -d);
		glRotated(180, 0, 0, 1);
		glRotated(180, 1, 1, 0);
		drawSphere(r, slices, stacks);  //-x -y -z
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(d, -d, -d);
		glRotated(-90, 0, 0, 1);
		glRotated(180, 1, 1, 0);
		drawSphere(r, slices, stacks);  //x y -z
	}
	glPopMatrix();
}

void drawfullPicture()
{
	glPushMatrix();
	{
		drawfullSphere();
		drawCube();
		drawfullCylinder();
	}
	glPopMatrix();
}

struct point myRotate(struct point p, struct point ref, double s)
{
	struct point inter, ret;
	double theta = 0.01 * s;

	inter.x = ref.y*p.z - ref.z*p.y;
	inter.y = ref.z*p.x - ref.x*p.z;
	inter.z = ref.x*p.y - ref.y*p.x;

	ret.x = p.x * cos(theta) + inter.x * sin(theta);
	ret.y = p.y * cos(theta) + inter.y * sin(theta);
	ret.z = p.z * cos(theta) + inter.z * sin(theta);

	return ret;
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			r = myRotate(r, u, 1);
			l = myRotate(l, u, 1);
			break;
		case '2':
			r = myRotate(r, u, -1);
			l = myRotate(l, u, -1);
			break;
		case '3':
			u = myRotate(u, r, 1);
			l = myRotate(l, r, 1);
			break;
		case '4':
			u = myRotate(u, r, -1);
			l = myRotate(l, r, -1);
			break;
		case '5':
			r = myRotate(r, l, 1);
			u = myRotate(u, l, 1);
			break;
		case '6':
			r = myRotate(r, l, -1);
			u = myRotate(u, l, -1);
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	double factor = 3.0;
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos.x = (pos.x - l.x * 3.0);
			pos.y = (pos.y - l.y * 3.0);
			pos.z = (pos.z - l.z * 3.0);
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos.x = (pos.x + l.x * 3.0);
			pos.y = (pos.y + l.y * 3.0);
			pos.z = (pos.z + l.z * 3.0);
			break;

		case GLUT_KEY_RIGHT:
			pos.x = (pos.x + r.x * 3.0);
			pos.y = (pos.y + r.y * 3.0);
			pos.z = (pos.z + r.z * 3.0);
			break;
		case GLUT_KEY_LEFT:
			pos.x = (pos.x - r.x * 3.0);
			pos.y = (pos.y - r.y * 3.0);
			pos.z = (pos.z - r.z * 3.0);
			break;

		case GLUT_KEY_PAGE_UP:	
			pos.x = (pos.x + u.x * 3.0);
			pos.y = (pos.y + u.y * 3.0);
			pos.z = (pos.z + u.z * 3.0);
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.x = (pos.x - u.x * 3.0);
			pos.y = (pos.y - u.y * 3.0);
			pos.z = (pos.z - u.z * 3.0);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			expand -= 0.2;
			expand = fmax(expand, 0.0);
			break;
		case GLUT_KEY_END:
			expand += 0.2;
			expand = fmin(expand, max_expand);
			break;

		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
//	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z,		pos.x+l.x,pos.y+l.y,pos.z+l.z,		u.x,u.y,u.z);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
//	drawGrid();
    //glColor3f(1,0,0);

	drawfullPicture();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
//	cameraHeight=150.0;
	pos.x = 100.0; pos.y = 100.0; pos.z = 0.0;
	cameraAngle=0;
	u.x = 0.0; u.y = 0.0; u.z = 1.0;
	r.x = -1.0/sqrt(2.0); r.y = 1.0/sqrt(2.0); r.z = 0.0;
	l.x = -1.0/sqrt(2.0); l.y = -1.0/sqrt(2.0); l.z = 0.0;
	expand = 15.0;
	max_expand = 30.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Offline1 Simulation");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}