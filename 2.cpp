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
#define wheelRadius 20.0

double cameraHeight;
double cameraAngle;
int drawgrid;
double angle;
double wheelAngle,wheelDistance;

struct point
{
	double x,y,z;
};

struct point pos,u,r,l,wheelPos;

void drawGrid()
{
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(int i=-20;i<=20;i++){

				//lines parallel to Y-axis
				glVertex3f(i*10, -200, 0);
				glVertex3f(i*10,  200, 0);

				//lines parallel to X-axis
				glVertex3f(-200, i*10, 0);
				glVertex3f( 200, i*10, 0);
			}
		}glEnd();
	}
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawRectangle(double width,double height)
{
    glPushMatrix();
    {
        double a=width/2;
        double b=height/2;
        glBegin(GL_QUADS);
        {
            glVertex3f(a,b,0);
            glVertex3f(a,-b,0);
            glVertex3f(-a,-b,0);
            glVertex3f(-a,b,0);
        }
        glEnd();
    }
    glPopMatrix();
}

void drawWheel()
{
    int segments=64;
    double perimeter=2*pi*wheelRadius;
    double rotateAngle = 360*wheelDistance/perimeter;
    double lenth=perimeter/segments;
    glPushMatrix();
    {
        glTranslatef(0,0,wheelRadius);
        glTranslatef(wheelPos.x,wheelPos.y,wheelPos.z);
        glRotatef(wheelAngle,0,0,1);
        glRotatef(rotateAngle,0,1,0);
        double color;

        struct point point[100];

        for(int i=0;i<=segments;i++) {
            point[i].x=wheelRadius*cos(2*pi*(double)i/(double)segments); //radian circulate
            point[i].y=0;
            point[i].z=wheelRadius*sin(2*pi*(double)i/(double)segments);
    }

    for(int i=0;i<segments;i++) {
        if(i<segments/2)   
        { 
            color=2*((double)i/(double)segments);
        }
        else    
        {
            color=2*(1-(double)i/(double)segments);
        }
        double circleAngle=360.0*(double)i/(double)segments;    //calculated in degree
        glPushMatrix(); {
            glColor3f(color,color,color);

            glRotatef(circleAngle,0,1,0);
            glTranslatef(wheelRadius,0,0);
            glRotatef(90,0,1,0);
            drawRectangle(lenth,4*lenth);
        }
        glPopMatrix();
    }
    glColor3f(.8,.8,.8);
    drawRectangle(2*wheelRadius,lenth);
    glColor3f(.6,.6,.6);
    glRotatef(90,0,1,0);
    drawRectangle(2*wheelRadius,lenth);
    }
    glPopMatrix();
}

/***Keyboard and mouse listeners***/
void keyboardListener(unsigned char key, int x,int y)
{
    switch(key){
        case 'w':
            wheelPos.x -=3*cos(wheelAngle*pi/180.0);   
            wheelPos.y -=3*sin(wheelAngle*pi/180.0); 
            wheelPos.z = 0; 
            wheelDistance-=3;
            break;
        case 's':
            wheelPos.x +=3*cos(wheelAngle*pi/180.0);  
            wheelPos.y +=3*sin(wheelAngle*pi/180.0);   
            wheelPos.z = 0;
            wheelDistance +=3;
            break;
        case 'a':
            wheelAngle+=3;
            break;
        case 'd':
            wheelAngle-=3;
            break;
        default:
            break;
    }
}

void specialKeyListener(int key, int x,int y)
{
    switch(key){
        case GLUT_KEY_DOWN:		//down arrow key
            cameraHeight -= 3.0;
            break;
        case GLUT_KEY_UP:		// up arrow key
            cameraHeight += 3.0;
            break;

        case GLUT_KEY_RIGHT:
            cameraAngle += 0.03;
            break;
        case GLUT_KEY_LEFT:
            cameraAngle -= 0.03;
            break;

        case GLUT_KEY_PAGE_UP:
            break;
        case GLUT_KEY_PAGE_DOWN:
            break;

        case GLUT_KEY_INSERT:
            break;

        case GLUT_KEY_HOME:
            break;
        case GLUT_KEY_END:
            break;

        default:
            break;
    }
}

void mouseListener(int button, int state, int x, int y)
{	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
                //..........
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

        case 3:             //scroll up
            cameraHeight--;
            break;
        case 4:             //scroll down
            cameraHeight++;
            break;
		default:
			break;
	}
}


/***main display function***/
void display()
{

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
	gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

//	gluLookAt(0,200,cameraHeight,	0,0,0,	0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	drawGrid();

    drawWheel();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate()
{
    angle += 0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init()
{
	//codes for initialization

	/** Camera initialization **/
	drawgrid=1;
	cameraHeight=50.0;
	cameraAngle=1.0;

    wheelPos.x=0;wheelPos.y=0;wheelPos.z=0;
    wheelAngle= 0;
    wheelDistance=0;

    angle = 0;

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

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Offline2 Simulation");

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