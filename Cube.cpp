#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "Cube.h"

//warning: Gratuitous use of the heap ahead
CubeList::CubeList()
{
	front=NULL;
	back=NULL;
	cur=NULL;
}

void CubeList::draw()
{
	if(front!=NULL)
	{
		cur=front;
		while(cur->next!=NULL)
		{
			cur->draw();
			cur=cur->next;
		}
		cur->draw();
	}
}

void CubeList::push(CubeObject newObject)
{
	if(front==NULL)
	{
		//class interrogation would be good
		front=new CubeObject;
		front->copyAll(newObject);
		back=front;
	}
	else
	{
		//need to put class interrogation here
		back->next=new CubeObject;
		back->next->copyAll(newObject);
		back=back->next;
	}
}

void CubeList::ScaleAll(int scaleVal)
{
	cur=front;
	do
	{
		cur->scaleVal=scaleVal;
		cur=cur->next;
	}while(cur!=back);
}

void CubeList::cut(int number)
{
	int i;
	cur=front;
	CubeObject* herpaderp; //too tired. So help me.
	for(i=0;i!=number;i++) //this stops the one before the one required to be blown up
	{
		if(cur->next!=NULL) //Avoid *ALL* the Segfaults! :D
		{
			cur=cur->next;
		}
	}
	//cool, we're here, now to kill stuff
	if(cur->next!=NULL && cur->next->next!=NULL) //Avoid *ALL* the Segfaults? D:
	{
		herpaderp=cur->next;
		cur->next=herpaderp->next;
		free(herpaderp); //Fine I'll be memory friendly, sheesh.
		herpaderp=NULL;
	}else{
		printf("Oi Bruv, someone tried to segfault your system by searching beyond the end point of a list >:(");
	}
}

void CubeList::animate(int animationTick, int animationSpeed)
{
	int correctedAnimationTick;
	correctedAnimationTick=animationTick*animationSpeed;
	cur=front;
	do
	{
		cur->animate(correctedAnimationTick);
		cur=cur->next;
	}while(cur!=back);
	
}

CubeObject::CubeObject()
{
	next=NULL;
	scaleVal=1;
}

void CubeObject::animate(int inboundTick)
{
	int tempAnim, animDiff, i;
	tempAnim=animationTick;
	animationTick=animationTick+(inboundTick/animationSpeedFactor);
	animDiff=animationTick-tempAnim;
	for(i=0;i<animDiff;i++)
	{
		animationTheta=animationTheta+animationDeltaTheta;
		animationPhi=animationPhi+animationDeltaPhi;
	}
}

void CubeObject::draw()
{
	
}