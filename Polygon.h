#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class PolyObject //A Polygon. What else?
{
			public:
		PolyObject();
		PolyObject* next;
		int scaleVal;

		float normal[3];
		float vertexes[4][3];

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
		void copyAll(PolyObject);
		void setTexture(char* inTex);
};


class PolyList //A list of stuff that needs to render n stuff
{
	public:
		PolyObject* front; 
		PolyObject* back;
		PolyObject* cur;
		PolyList();
		void draw();
		void ScaleAll(int);
		void push(PolyObject);
		void cut(int);
		void animate(int,int);
	}
;
