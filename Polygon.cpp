#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
#include <SOIL.h>
#else
#include "SOIL/SOIL.h"
#endif

#include "TextureLoader.h"

#include "Polygon.h"

//warning: Gratuitous use of the heap ahead
PolyList::PolyList()
{
	front=NULL;
	back=NULL;
	cur=NULL;
}

void PolyList::draw()
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

void PolyList::push(PolyObject newObject)
{
	if(front==NULL)
	{
		//class interrogation would be good
		front=new PolyObject;
		front->copyAll(newObject);
		back=front;
	}
	else
	{
		//need to put class interrogation here
		back->next=new PolyObject;
		back->next->copyAll(newObject);
		back=back->next;
	}
}

void PolyList::ScaleAll(int scaleVal)
{
	cur=front;
	do
	{
		cur->scaleVal=scaleVal;
		cur=cur->next;
	}while(cur!=back);
}

void PolyList::cut(int number)
{
	int i;
	cur=front;
	PolyObject* herpaderp; //too tired. So help me.
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

void PolyList::animate(int animationTick, int animationSpeed)
{
	int correctedAnimationTick;
	correctedAnimationTick=animationTick;
	cur=front;
	while(cur!=back)
	{
		cur->animate(correctedAnimationTick);
		cur=cur->next;
	}
	cur->animate(correctedAnimationTick);
	
}

PolyObject::PolyObject()
{

	next=NULL;
	scaleVal=1;
	texture[0]=0;
	texSRepeat=texTRepeat=1.0;
	blend=0;
	blendFactor=1.0;

}

void PolyObject::animate(int inboundTick)
{
/*	int tempAnim, animDiff, i;
	tempAnim=animationTick;
	animationTick=animationTick+(inboundTick/animationSpeedFactor);
	animDiff=animationTick-tempAnim;
	for(i=0;i<animDiff;i++)
	{*/
		animationTheta=animationTheta+animationDeltaTheta;
		animationPhi=animationPhi+animationDeltaPhi;
		
		//printf("Thet:%f Phi:%f\n", animationTheta,animationPhi);
//	}
}

void PolyObject::draw()
{
//	float betterMatrix[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,-5,1};

	glPushMatrix();
	glRotatef(animationPhi,0.0,1,0);
	glMultMatrixf(&positionMatrix[0]);
//	glRotatef(60,1.0,0.0,0.0);
//	glRotatef(-20,0.0,0.0,1.0);

	glRotatef(animationTheta,1.0,0,0);

	if(blend!=0)
	{
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0,1.0,1.0,blendFactor);
	}

	if(texture!=0){
	glBindTexture(GL_TEXTURE_2D, texture[0]); 
	glBegin(GL_QUADS);
	glNormal3fv(&normal[0]);
	glTexCoord2f(0.0,1.0*texTRepeat); glVertex3fv(&vertexes[0][0]);
	glTexCoord2f(0.0,0.0); glVertex3fv(&vertexes[1][0]);
	glTexCoord2f(1.0*texSRepeat,0.0); glVertex3fv(&vertexes[2][0]);
	glTexCoord2f(1.0*texSRepeat,1.0*texTRepeat); glVertex3fv(&vertexes[3][0]);
	glEnd();
	}else{
	glBegin(GL_QUADS);
	glNormal3fv(&normal[0]);
	glVertex3fv(&vertexes[0][0]);
	glVertex3fv(&vertexes[1][0]);
	glVertex3fv(&vertexes[2][0]);
	glVertex3fv(&vertexes[3][0]);
	glEnd();
	}
	if(blend!=0)
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glColor4f(1.0,1.0,1.0,1.0);
	}
	glPopMatrix();
}

void PolyObject::copyAll(PolyObject newCube)
{
	int i,j;
	scaleVal=newCube.scaleVal;
	animationOn=newCube.animationOn;
	animationTick=newCube.animationTick;
	animationRadius=newCube.animationRadius;
	animationTheta=newCube.animationTheta;
	animationPhi=newCube.animationPhi;
	animationDeltaTheta=newCube.animationDeltaTheta;
	animationDeltaPhi=newCube.animationDeltaPhi;
	animationSpeedFactor=newCube.animationSpeedFactor;

	texSRepeat=newCube.texSRepeat;
	texTRepeat=newCube.texTRepeat;

	blend=newCube.blend;
	blendFactor=newCube.blendFactor;

	texture[0]=newCube.texture[0];

	for(i=0;i<4;i++)
	{
		for(j=0;j<3;j++)
		{
			vertexes[i][j]=newCube.vertexes[i][j];
			if(i==0){normal[j]=newCube.normal[j];}
		}
	}
	
	for(i=0;i<16;i++)
	{
		positionMatrix[i]=newCube.positionMatrix[i];
	}
}

void PolyObject::setTexture(char* inTex)
{
	TextureLoader(&texture[0], inTex);

	printf("%d\n",texture[0]);
}