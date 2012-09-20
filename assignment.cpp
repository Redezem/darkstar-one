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

//cubelist
//spherelist
//polygonlist


void ComputeAnimation()
{
	int animTick=currentTick-animationStartTick;
	CubeList.animate(animTick,(int)currentSpeedFactor*10.0);
	//that *should* do...
}

void display()
{
//	BCdisplay();
	//Superstruct.renderer->DrawWorld();
	 //from Rendax
	currentTick+(int)(currentSpeedFactor*10.0);
	if(pauseAll==0)
	{
		if(animationActive==1)
		{
			ComputeAnimation();
		}
		if(currentXRotFactor!=0.0)
		{
			//ComputeXRot();
		}
		if(currentYRotFactor!=0.0)
		{
			//ComputeYRot();
		}
		//Put the things that need to be drawn here
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
	
//	BCinit();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Redezem's Assignment");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardInput);
	init();
	glutMainLoop();
	return 0;
}
