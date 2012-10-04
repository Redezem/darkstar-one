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

		GLuint texture[1];
		float texSRepeat;
		float texTRepeat;

		int blend;
		float blendFactor;

		int animationOn;
		int animationTick;
		float animationRadius, animationTheta, animationPhi, animationDeltaTheta, animationDeltaPhi, animationSpeedFactor;
	
		float positionMatrix[16];//this is cols rows not rows cols
// should look like:	float betterMatrix[16]={1,0,0,0,
//						0,1,0,0,
//						0,0,1,0,
//						0,0,-5,1};
 

		void draw();
		void animate(int);
		void copyAll(CubeObject);
		void setTexture(char* inTex);
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
