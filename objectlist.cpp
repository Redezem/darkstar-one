#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

#include "objectlist.h"
#include "object.h"
//warning: Gratuitous use of the heap ahead
GraphicObjectList::GraphicObjectList()
{
	front=NULL;
	back=NULL;
	cur=NULL;
}

void GraphicObjectList::draw()
{
}

void GraphicObjectList::push(GraphicObject newObject)
{
	if(front==NULL)
	{
		front=(GraphicObject*)malloc(sizeof(GraphicObject));
		memcpy(front, newObject, sizeof(GraphicObject));
		back=front;
	}
	else
	{
		*back.next=(GraphicObject*)malloc(sizeof(GraphicObject));
		memcpy(*back.next, newObject, sizeof(GraphicObject));
		back=*back.next;
	}
}

void GraphicObjectList::ScaleAll(int scaleVal)
{
	cur=front;
	do
	{
		*cur.scaleVal=scaleVal;
		cur=*front.next;
	}while(cur!=back);
}

void GraphicObjectList::cut(int number)
{
	int i;
	cur=front;
	GraphicObject* herpaderp; //too tired. So help me.
	for(i=0;i!=number;i++) //this stops the one before the one required to be blown up
	{
		if(*cur.next!=NULL) //Avoid *ALL* the Segfaults! :D
		{
			cur=*cur.next;
		}
	}
	//cool, we're here, now to kill stuff
	if(*cur.next!=NULL && *cur.*next.next!=NULL) //Avoid *ALL* the Segfaults? D:
	{
		herpaderp=*cur.next;
		*cur.next=*herpaderp.next;
		free(herpaderp); //Fine I'll be memory friendly, sheesh.
		herpaderp=NULL;
	}else{
		printf("Oi Bruv, someone tried to segfault your system by searching beyond the end point of a list >:(");
	}
}

void GraphicObjectList::animate(int animationTick, int animationSpeed)
{
}
