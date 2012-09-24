#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "Cube.h"
//including objects
//#include "rendax.h"
//#include "object.h"
//Ripped from Rendax
double windowHeight, windowWidth, aspectRatio, currentLookAtMatrix[3][3], currentZoomFactor, currentSpeedFactor, currentXRotFactor, currentYRotFactor;
int animationActive, animationStartTick, xRotStartTick, yRotStartTick, currentTick, pauseAll, bufferNumber;

CubeObject* cube=new CubeObject;
//spherelist
//polygonlist

GLfloat light_diffuse[] = {1.0,0.0,0.0,1.0};
GLfloat light_position[] = {1.0,1.0,1.0,0.0};

void ResizePerspectiveMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective((40.0/currentZoomFactor),aspectRatio,1.0,10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetStockLookAtMatrix()
{
	ResizePerspectiveMatrix();
	gluLookAt(	0.0,0.0,0.5,
			0.0,0.0,0.0,
			0.0,1.0,0.0);
	currentLookAtMatrix[0][0]=currentLookAtMatrix[0][1]=currentLookAtMatrix[1][0]=currentLookAtMatrix[1][1]=currentLookAtMatrix[1][2]=currentLookAtMatrix[2][0]=currentLookAtMatrix[2][2]=0.0;
	currentLookAtMatrix[0][2]=0.5;
	currentLookAtMatrix[2][1]=1.0;
}


void ComputeAnimation()
{
	int animTick=currentTick-animationStartTick;
	//that *should* do...
}
void MakeLights()
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}


void display()
{
//	BCdisplay();
	//Superstruct.renderer->DrawWorld();
	 //from Rendax
	currentTick=currentTick+(int)(currentSpeedFactor*10.0);
	if(pauseAll==0)
	{
		if(animationActive==1)
		{
		///	ComputeAnimation();
		}
		if(currentXRotFactor!=0.0)
		{
			//ComputeXRot();
		}
		if(currentYRotFactor!=0.0)
		{
			//ComputeYRot();
		}
	}
	if(animationStartTick!=0 && (pauseAll!=0 || animationActive==0))
	{
		animationStartTick++;
	}
	if(xRotStartTick!=0 && (pauseAll!=0 || currentXRotFactor==0.0))
	{
		xRotStartTick++;
	}
	if(yRotStartTick!=0 && (pauseAll!=0 || currentYRotFactor==0.0))
	{
		yRotStartTick++;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glutSolidSphere(1, 20, 20);
		cube->draw();
		//Put the things that need to be drawn here
	glutSwapBuffers();

}

void reshape(int w, int h)
{
//	BCreshape(w,h);
	//Superstruct.renderer->RescaleWorld(w,h);
}

void keyboardInput(unsigned char c, int x, int y)
{
	//Superstruct.renderer->MenuInput(c,x,y);
}



void init()
{
	windowHeight=windowWidth=500.0;
	aspectRatio=1.0;
	currentZoomFactor=1.0;
	currentSpeedFactor=0.1;
	currentXRotFactor=currentYRotFactor=0.0;
	animationActive=animationStartTick=currentTick=0;
	pauseAll=1;
	glEnable(GL_DEPTH_TEST);
	SetStockLookAtMatrix();

	glTranslatef(0.0,0.0,-5.0);
	glRotatef(60,1.0,0.0,0.0);
	glRotatef(-20,0.0,0.0,1.0);

	
	int i=0;
int j=0;

	GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
	  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
	GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
	  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
	  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
for(i=0;i<6;i++)
{
	for(j=0;j<3;j++)
	{
		cube->normals[i][j]=n[i][j];
	}
	for(j=0;j<4;j++)
	{
		cube->faces[i][j]=faces[i][j];
	}
}

	cube->vertexes[0][0] = cube->vertexes[1][0] = cube->vertexes[2][0] = cube->vertexes[3][0] = -1;
 	cube->vertexes[4][0] = cube->vertexes[5][0] = cube->vertexes[6][0] = cube->vertexes[7][0] = 1;
 	cube->vertexes[0][1] = cube->vertexes[1][1] = cube->vertexes[4][1] = cube->vertexes[5][1] = -1;
	cube->vertexes[2][1] = cube->vertexes[3][1] = cube->vertexes[6][1] = cube->vertexes[7][1] = 1;
 	cube->vertexes[0][2] = cube->vertexes[3][2] = cube->vertexes[4][2] = cube->vertexes[7][2] = 1;
 	cube->vertexes[1][2] = cube->vertexes[2][2] = cube->vertexes[5][2] = cube->vertexes[6][2] = -1;
	
	
	MakeLights();
	
//	BCinit();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Redezem's Assignment");
	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(keyboardInput);
	init();
	glutMainLoop();
	return 0;
}
