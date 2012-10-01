#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class SphereObject //A Sphere. What else?
{
			public:
		SphereObject();
		SphereObject* next;
		int scaleVal;

		double radius;
		int slices;
		int squares;


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
		void copyAll(SphereObject);
};


class SphereList //A list of stuff that needs to render n stuff
{
	public:
		SphereObject* front; 
		SphereObject* back;
		SphereObject* cur;
		SphereList();
		void draw();
		void ScaleAll(int);
		void push(SphereObject);
		void cut(int);
		void animate(int,int);
	}
;