#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "object.h"

class GraphicObjectList //A list of stuff that needs to render n stuff
{
	GraphicObject* front, back, cur;
	public:
		GraphicObjectList();
		void draw();
		void ScaleAll(int);
		void push(GraphicObject);
		void animate(int,int);
	private:
		int isLast();
		void next();
		void reset();
}
