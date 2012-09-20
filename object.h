#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class GraphicObject
{
	public:
		GraphicObject next;
		int scaleVal;
		virtual void draw()=0;
		virtual void animate(int)=0;
}

class CubeObject : public GraphicObject
{
	public:
		GraphicObject next;
		int scaleVal;
		

		void draw();
		void animate(int);

}
