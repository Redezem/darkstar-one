
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

enum ObjectTypes
{
	None,
	Cube,
	Sphere,
	Polygon
};

class GraphicObject
{
	public:
		GraphicObject();
		GraphicObject* next;
		int scaleVal;
		ObjectTypes ObjectType;
		virtual void draw();
		virtual void animate(int);
		virtual void copyAll(GraphicObject);
};

class CubeObject : public GraphicObject
{
	public:
		CubeObject();
		GraphicObject* next;
		int scaleVal;
		ObjectTypes ObjectType;

		GLfloat vertexes[8][3];
		GLfloat normals[6][3];
		GLint faces[6][4];

		void draw();
		void animate(int);
		void copyAll(CubeObject);
};
