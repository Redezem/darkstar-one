#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "object.h"
#include "rendax.h"
#include "objectlist.h"
//#include "lightinglist.h"
//#include "menulist.h"
//#include "animlist.h"

Rendax::Rendax()
{
	windowHeight=windowWidth=500.0;
	aspectRatio=1.0;
	SetStockLookAtMatrix();
	currentZoomFactor=1.0;
	currentSpeedFactor=0.1;
	currentXRotFactor=currentYRotFactor=0.0;
	animationActive=animationStartTick=currentTick=0;
	pauseAll=1;
	bufferNumber=1;
	objectList=*new GraphicObjectList;
	//lightingList=new LightingObjectList;
	//menuList=new MenuObjectList;
	//animList=new AnimationSegmentList;
}

void Rendax::DrawWorld()
{
	currentTick+(int)(currentSpeedFactor*10.0);
	if(pauseAll==0)
	{
		if(animationActive==1)
		{
			ComputeAnimation();
		}
		if(currentXRotFactor!=0.0)
		{
			ComputeXRot();
		}
		if(currentYRotFactor!=0.0)
		{
			ComputeYRot();
		}
		//animList.draw();
		objectList.draw();
		//lightingList.draw();
		//menuList.draw();
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

void Rendax::RescaleWorld(int width, int height)
{
	double scalerRatio=1.0;
	if(height<=width)
	{
		scalerRatio=height/500.0;
	}
	else
	{
		scalerRatio=width/500.0;
	}
	glViewport(0,0,(GLsizei)width, (GLsizei)height);
	windowHeight=height;
	windowWidth=width;
	aspectRatio=width/height;
	//lightingList.killLights();
	ResizePerspectiveMatrix();
	SetLookAt(currentLookAtMatrix);
	objectList.ScaleAll(scalerRatio);
	//lightingList.ScaleAll(scalerRatio);
	//animList.ScaleAll(scalerRatio);
	//lightingList.enableLights();
}

void Rendax::SetLookAt(double newLookAtMatrix[3][3])
{
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			currentLookAtMatrix[i][j]=newLookAtMatrix[i][j];
		}
	}
	gluLookAt(	currentLookAtMatrix[0][0],currentLookAtMatrix[0][1],currentLookAtMatrix[0][2],
			currentLookAtMatrix[1][0],currentLookAtMatrix[1][1],currentLookAtMatrix[1][2],
			currentLookAtMatrix[2][0],currentLookAtMatrix[2][1],currentLookAtMatrix[2][2]);
}

void Rendax::SetZoomFactor(double newFac)
{
	currentZoomFactor=newFac;
}

void Rendax::IncrementZoomFactor(int incre)
{
	if(incre>0)
	{
		currentZoomFactor=currentZoomFactor+0.1;
	}
	else if(incre<0)
	{
		currentZoomFactor=currentZoomFactor-0.1;
	}
	else
	{
		//Do nothing I suppose....
	}
}

void Rendax::SetSpeedFactor(double newFac)
{
	currentSpeedFactor=newFac;
}

void Rendax::IncrementSpeedFactor(int incre)
{
	if(incre>0)
	{
		currentSpeedFactor=currentSpeedFactor+0.1;
	}
	else if(incre<0)
	{
		currentSpeedFactor=currentSpeedFactor-0.1;
	}
	else
	{
		//Do nothing I suppose....
	}
}

void Rendax::SetRotFactor(double newXFac,double newYFac)
{
	currentXRotFactor=newXFac;
	currentYRotFactor=newYFac;
}

void Rendax::IncrementRotFactor(int Xinc, int Yinc)
{
	if(Xinc!=0 && currentXRotFactor==0 && xRotStartTick==0)
	{
		xRotStartTick=currentTick;
	}
	if(Yinc!=0 && currentYRotFactor==0 && yRotStartTick==0)
	{
		yRotStartTick=currentTick;
	}
	if(Xinc>0)
	{
		currentXRotFactor=currentXRotFactor+0.1;
	}
	else if(Xinc<0)
	{
		currentXRotFactor=currentXRotFactor-0.1;
	}
	else if(Xinc==0)
	{
		//Do nothing I suppose....
	}
	
	if(Yinc>0)
	{
		currentYRotFactor=currentYRotFactor+0.1;
	}
	else if(Yinc<0)
	{
		currentYRotFactor=currentYRotFactor-0.1;
	}
	else if(Yinc==0)
	{
		//Do nothing for now...
	}
}

void Rendax::Animation(int state)
{
	animationActive=state;
	if(state!=0)
	{
		animationStartTick=currentTick;
	}
}

void Rendax::Pause(int state)
{
	pauseAll=state;
}

void Rendax::AddObject(GraphicObject newObject)
{
	objectList.push(newObject);
}
/*
void Rendax::AddLight(LightingObject newLight)
{
	lightingList.push(newLight);
}

void Rendax::AddMenu(MenuObject newOption)
{
	menuList.push(newOption);
}

void Rendax::AddAnim(double startAt[3][3],double endAt[3][3],int frames)
{
	double atomChange[3][3],frameBuffer[3][3];
	int i,j,k;
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			atomChange[i][j]=(endAt[i][j]-startAt[i][j])/frames;
			frameBuffer[i][j]=startAt[i][j];
		}
	}
	animList.push(frameBuffer);
	for(k=1;k<frames;k++)
	{
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				frameBuffer[i][j]=frameBuffer[i][j]+atomChange[i][j];
			}
		}
		animList.push(frameBuffer);
	}
}
*/
double Rendax::GetZoomFactor()
{
	return currentZoomFactor;
}

double Rendax::GetSpeedFactor()
{
	return currentSpeedFactor;
}

int Rendax::GetTick()
{
	return currentTick;
}

int Rendax::GetAnim()
{
	return animationActive;
}

int Rendax::GetPause()
{
	return pauseAll;
}

void Rendax::GetRotFactor(int* XReturn, int* YReturn)
{
	*XReturn=currentXRotFactor;
	*YReturn=currentYRotFactor;
}

//Private Function Time!

void Rendax::SetStockLookAtMatrix()
{
	ResizePerspectiveMatrix();
	gluLookAt(	0.0,0.0,0.5,
			0.0,0.0,0.0,
			0.0,1.0,0.0);
	currentLookAtMatrix[0][0]=currentLookAtMatrix[0][1]=currentLookAtMatrix[1][0]=currentLookAtMatrix[1][1]=currentLookAtMatrix[1][2]=currentLookAtMatrix[2][0]=currentLookAtMatrix[2][2]=0.0;
	currentLookAtMatrix[0][2]=0.5;
	currentLookAtMatrix[2][1]=1.0;
}

void Rendax::ComputeAnimation()
{
	int animTick=currentTick-animationStartTick;
	//animList.animate(animTick,(int)currentSpeedFactor*10.0);
	objectList.animate(animTick,(int)currentSpeedFactor*10.0);
	//lightingList.animate(animTick,(int)currentSpeedFactor*10.0);
	//that *should* do...
}

void Rendax::ComputeXRot()
{
	int xRotTick=currentTick-xRotStartTick;
	//animList.XRot(xRotTick,currentXRotFactor);
}

void Rendax::ComputeYRot()
{
	int yRotTick=currentTick-yRotStartTick;
	//animList.YRot(yRotTick,currentXRotFactor);
}

void Rendax::ResizePerspectiveMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective((40.0/currentZoomFactor),aspectRatio,1.0,100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
