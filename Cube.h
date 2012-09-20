#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class CubeObject //A Cube. What else?
{
			public:
		CubeObject();
		CubeObject* next;
		int scaleVal;

		GLfloat vertexes[8][3];
		GLfloat normals[6][3];
		GLint faces[6][4];

		int animationOn;
		int animationTick;
		float animationRadius, animationTheta, animationPhi, animationDeltaTheta, animationDeltaPhi, animationSpeedFactor;

		void draw();
		void animate(int);
		void copyAll(CubeObject);
};


class CubeList //A list of stuff that needs to render n stuff
{
	public:
		CubeObject* front; 
		CubeObject* back;
		CubeObject* cur;
		CubeList();
		void draw();
		void ScaleAll(int);
		void push(CubeObject);
		void cut(int);
		void animate(int,int);
	}
;
