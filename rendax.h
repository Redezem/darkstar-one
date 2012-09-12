#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "object.h"
#include "light.h"
#include "objectlist.h"
#include "lightinglist.h"
#include "menulist.h"
#include "animlist.h"

class Rendax //BOW BEFORE THE ALMIGHTY RendAX!
{
	double windowHeight, windowWidth, aspectRatio, currentLookAtMatrix[3][3], currentZoomFactor, currentSpeedFactor, currentXRotFactor, currentYRotFactor;
	int animationActive, animationStartTick, currentTick, pauseAll, bufferNumber;
	GraphicObjectList objectList;
	LightingObjectList lightingList;
	MenuObjectList menuList;
	AnimationSegmentList animList;

	//time for some functions!
	 public:
	Rendax();
	void DrawWorld();
	void RescaleWorld(int, int);
	void SetLookAt(double[3][3]);
	void SetZoomFactor(double);
	void IncrementZoomFactor(int);
	void SetSpeedFactor(double);
	void IncrementSpeedFactor(int);
	void SetRotFactor(double,double);
	void IncrementRotFactor(int,int);
	void Animation(int);
	void Pause(int);
	void AddObject(GraphicObject);
	void AddLight(LightObject);
	void AddMenu(MenuObject);
	void AddAnim(double[3][3],double[3][3], int);

	double GetZoomFactor();
	double GetSpeedFactor();
	double GetTick();
	double GetAnim();
	double GetPause();
	void GetRotFactor(int*, int*);
}
