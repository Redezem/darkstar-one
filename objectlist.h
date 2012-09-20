#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "object.h"

class GraphicObjectList //A list of stuff that needs to render n stuff
{
	public:
		GraphicObject* front; 
		GraphicObject* back;
		GraphicObject* cur;
		GraphicObjectList();
		void draw();
		void ScaleAll(int);
		void push(GraphicObject);
		void cut(int);
		void animate(int,int);
	}
;
