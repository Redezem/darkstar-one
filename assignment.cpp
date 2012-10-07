//Generic includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Windows cross compatibility
#ifdef WIN32
#include <Windows.h>
#endif
//GL and GLUT
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
//Rendax Style Render Lists
#include "Cube.h"
#include "Sphere.h"
#include "Polygon.h"
#include "TextureLoader.h"
//including objects
//#include "rendax.h"
//#include "object.h"
//Ripped from Rendax
double windowHeight, windowWidth, aspectRatio, currentLookAtMatrix[3][3], currentZoomFactor, currentSpeedFactor, currentXRotFactor, currentYRotFactor,xViewAngle,yViewAngle, distanceFromViewCenter,coneAngle;
int animationActive, animationStartTick, xRotStartTick, yRotStartTick, currentTick, pauseAll, bufferNumber, capmode,fullscreen;

GLUquadricObj *cone;
GLuint conetexture[1];
//Lists
CubeList* cuboids=new CubeList;
SphereList* spheroids=new SphereList;
PolyList* polys=new PolyList;
//lighting positions
GLfloat light_diffuse[] = {1.0,1.0,1.0,1.0};
GLfloat light_position[] = {20.0,20.0,0.0,0.0};
GLfloat light_ambient[] = {1.0,1.0,1.0,1.0};
GLfloat light_specular[] = {1.0,1.0,1.0,1.0};
GLfloat specular_position[] = {20.0,-20.0,20.0,0.0};
//Specular rulings
float specReflection[] = { 0.8, 0.8, 0.8, 1.0 };
float specReflectionforSphere[] = { 1.0, 1.0, 1.0, 1.0};

void RegenerateLookAtMatrix();

void idle(void)
{
	//this segment does the frame limiting
	if(capmode!=0)
	{
	#ifdef WIN32
	Sleep(17);
	#endif
	}
	glutPostRedisplay();
}

void ComputeYRot()
{
	float xZPlaneDistance, yAxisSubtended,newXAxisSubtended,newZAxisSubtended;

	xViewAngle=xViewAngle+currentXRotFactor;
	yAxisSubtended=currentLookAtMatrix[0][1];
	xZPlaneDistance=sqrtf(((distanceFromViewCenter*distanceFromViewCenter)-(yAxisSubtended*yAxisSubtended)));
	newXAxisSubtended=xZPlaneDistance*cosf(xViewAngle);
	newZAxisSubtended=xZPlaneDistance*sinf(xViewAngle);
	currentLookAtMatrix[0][0]=newXAxisSubtended;
	currentLookAtMatrix[0][2]=newZAxisSubtended;

	RegenerateLookAtMatrix();
	
}

void ComputeXRot()
{
	float yZPlaneDistance, xAxisSubtended,newYAxisSubtended,newZAxisSubtended;

	yViewAngle=yViewAngle+currentYRotFactor;
	xAxisSubtended=currentLookAtMatrix[0][0];
	yZPlaneDistance=sqrtf(((distanceFromViewCenter*distanceFromViewCenter)-(xAxisSubtended*xAxisSubtended)));
	newZAxisSubtended=yZPlaneDistance*cosf(yViewAngle);
	newYAxisSubtended=yZPlaneDistance*sinf(yViewAngle);
	currentLookAtMatrix[0][1]=newYAxisSubtended;
	currentLookAtMatrix[0][2]=newZAxisSubtended;

	RegenerateLookAtMatrix();
	
}

void MatMult4(float* AIN, float* BIN, float* OUTAGE) //Cool function that does 4*4 matrix mulitplications
{
	const int N=4;
	float A[N][N], B[N][N], C[N][N];
	int i,j,k;
	k=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			A[i][j]=AIN[k];
//			printf("A=%f\n", A[i][j]);
			B[i][j]=BIN[k];
//			printf("B=%f\n", B[i][j]);
			k++;
		}
	}


       float sum;
       for (i = 0; i < N; i++) {
          for (j = 0; j < N; j++) {
           sum = 0;
            for (k = 0; k < N; k++) {
             sum += A[i][k] * B[k][j];
//		printf("sum=%f\n", sum);
                       }
             C[i][j] = sum;
               }
       }
	k=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			OUTAGE[k]=C[i][j];
			k++;	
		}	
	}
}

void ResizePerspectiveMatrix() //Resizes Perspectives on Demand
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective((40.0/currentZoomFactor),aspectRatio,1.0,200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetStockLookAtMatrix() //Startup look at matrix set up
{
	ResizePerspectiveMatrix();
	gluLookAt(	0.0,0.0,30.0,
			0.0,0.0,0.0,
			0.0,1.0,0.0);
	currentLookAtMatrix[0][0]=currentLookAtMatrix[0][1]=currentLookAtMatrix[1][0]=currentLookAtMatrix[1][1]=currentLookAtMatrix[1][2]=currentLookAtMatrix[2][0]=currentLookAtMatrix[2][2]=0.0;
	currentLookAtMatrix[0][2]=30;
	currentLookAtMatrix[2][1]=1.0;
	distanceFromViewCenter=30;
}

void RegenerateLookAtMatrix() //Use previous lookatmatrix to build the Look at matrix
{
	ResizePerspectiveMatrix();
	gluLookAt(	currentLookAtMatrix[0][0],currentLookAtMatrix[0][1],currentLookAtMatrix[0][2],
				currentLookAtMatrix[1][0],currentLookAtMatrix[1][1],currentLookAtMatrix[1][2],
				currentLookAtMatrix[2][0],currentLookAtMatrix[2][1],currentLookAtMatrix[2][2]);
}
void DeMakeLights()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
}
void MakeLights() //This is effectively a computerised art student, as a friend of mine so nicely put it
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT2, GL_POSITION, specular_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);

}
void ReMakeLights()
{
	DeMakeLights();
	MakeLights();
}

void DrawSomeText(char*outmsg)
{
	glTranslated(0,-100,0);
		glRasterPos2f(100,2500);
		while(*outmsg)
		{
		//	printf("%c\n",*outmsg);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*outmsg++);
			//glutStrokeCharacter(GLUT_STROKE_ROMAN,*outmsg++);
		}
}

void writeMenu()
{
	DeMakeLights();
	glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluOrtho2D(0,windowWidth,0,windowHeight);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glScalef(0.1, 0.1, 0.1);
	 glColor3d(1.0,1.0,1.0);
	DrawSomeText("--------------------------------------");
	DrawSomeText("Z: Zoom in");
	DrawSomeText("z: Zoom out");
	DrawSomeText("--------------------------------------");
	DrawSomeText("X/x: Rotate Y-wise (around X axis)");
	DrawSomeText("Y/y: Rotate X-wise (around Y axis)");
	DrawSomeText("--------------------------------------");
	DrawSomeText("A/a: Begin/Pause Animation");
	DrawSomeText("F/f: Increase Animation Speed");
	DrawSomeText("S/s: Decrease Animation Speed");
	DrawSomeText("--------------------------------------");
	DrawSomeText("T/t: Pause All");
	DrawSomeText("C/c: Resume All");
	DrawSomeText("--------------------------------------");
	DrawSomeText("p: Flat Shading");
	DrawSomeText("P: Smooth Shading");
	DrawSomeText("--------------------------------------");
	DrawSomeText("W/w: Toggle Widescreen");
	DrawSomeText("U/u: Toggle Fullscreen");
	DrawSomeText("Q/q: Exit");
	DrawSomeText("--------------------------------------");
	RegenerateLookAtMatrix();
	MakeLights();
}

/*
void ComputeAnimation() //animation computation, no longer needed
{
	int animTick=currentTick-animationStartTick;
	//that *should* do...
}
*/
void DrawCone()
{
	glPushMatrix();
	
	glEnable(GL_BLEND);
	glColor4f(0.5,0.5,1.0,0.8);
	glTranslated(0,-27,0);
	glRotated(coneAngle,0,1,0);
	glRotated(-90,1,0,0);
	glBindTexture(GL_TEXTURE_2D,conetexture[0]);
	gluCylinder(cone,0,9,20,90*currentZoomFactor,90*currentZoomFactor);
	glDisable(GL_BLEND);
	glColor4f(1.0,1.0,1.0,1.0);
	glPopMatrix();
}



void display() //Aaah the draw function
{
//	BCdisplay();
	//Superstruct.renderer->DrawWorld();
	 //from Rendax
	currentTick=currentTick+(int)(currentSpeedFactor*10.0);
	if(pauseAll==0)
	{
		if(animationActive!=0.0)
		{
		//	printf("animat\n");
			cuboids->animate(1,currentSpeedFactor);
			spheroids->animate(1,currentSpeedFactor);
			coneAngle=coneAngle-currentSpeedFactor;
		}
		if(currentXRotFactor!=0.0) //Confusing no? It's planar rotation we're doing here, not axial rotation, that's why the factors are "wrong". Factors reference Planes, everything else reference Axes.
		{
			ComputeYRot();
		}
		if(currentYRotFactor!=0.0)
		{
			ComputeXRot();
		}
	}
	if(animationStartTick!=0 && (pauseAll!=0 || animationActive==0))
	{
		animationStartTick++;
	}
	if(xRotStartTick!=0 && (pauseAll!=0 || currentXRotFactor==0.0))
	{
		xRotStartTick++;
	}
	if(yRotStartTick!=0 && (pauseAll!=0 || currentYRotFactor==0.0))
	{
		yRotStartTick++;
	}
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//glutSolidSphere(8, 90, 90);
	glPopMatrix();
		
	
	
		//cube->draw();
		cuboids->draw();
		polys->draw();
		DrawCone();
		//cuboids->draw();
		glMaterialfv(GL_FRONT, GL_SPECULAR, specReflectionforSphere);
		glMateriali(GL_FRONT, GL_SHININESS, 100);
		spheroids->draw(currentZoomFactor);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
		glMateriali(GL_FRONT, GL_SHININESS, 56);
		//Put the things that need to be drawn here
//	printf("ran\n");
	//	glPushMatrix();
		//	glEnable(GL_BLEND);
		//	glColor4f(1.0,1.0,1.0,0.5);
		writeMenu();
		//	glColor4f(1.0,1.0,1.0,1.0);
		//	glDisable(GL_BLEND);
	//	glPopMatrix();
		ReMakeLights();
		
	glutSwapBuffers();

}

void reshape(int w, int h)
{
//	BCreshape(w,h);
	//Superstruct.renderer->RescaleWorld(w,h);
}

void SwitchCapMode()
{
	if(capmode!=0)
	{
		capmode=0;
	}
	else
	{
		capmode=1;
	}
}

void SizeChange()
{
	if( aspectRatio==1)
	{
		aspectRatio=1.7;
		glutReshapeWindow(889,500);
		RegenerateLookAtMatrix();
	}
	else
	{
		aspectRatio=1;
		glutReshapeWindow(500,500);
		RegenerateLookAtMatrix();
	}
}

void FullScreen16by9()
{
	if(fullscreen==0)
	{
	aspectRatio=1.7;
	glutFullScreen();
	RegenerateLookAtMatrix();
	fullscreen=1;
	}
	else
	{
		glutReshapeWindow(889,500);
		fullscreen=0;
		RegenerateLookAtMatrix();
	}
}

void keyboardInput(unsigned char c, int x, int y)
{
	switch(c)
	{
	case('E'): //As yet only useable in Windows
	case ('e'):
		SwitchCapMode();
		printf("Switch!\n");
		break;
	case('W'):
	case ('w'):
		SizeChange();
		break;
	case('U'):
	case ('u'):
		FullScreen16by9();
		break;
	case('Q'):
	case ('q'):
		exit(0);
		break;
	case ('y'):
	case ('Y'):
		if(currentXRotFactor!=0.0)
		{
			currentXRotFactor=0.0;
		}else{
			currentXRotFactor=0.01;
		}
		break;
	case('x'):
	case('X'):
		if(currentYRotFactor!=0.0)
		{
			currentYRotFactor=0.0;
		}else{
			currentYRotFactor=0.01;
		}
		break;
	case('a'):
	case('A'):
		if(animationActive!=0)
		{
			animationActive=0;
		}else{
			animationActive=1;
		}
		break;
	case('z'):
		if(currentZoomFactor>0.5){
		currentZoomFactor=currentZoomFactor-0.1;
		}
		RegenerateLookAtMatrix();
		break;
	case('Z'):
		currentZoomFactor=currentZoomFactor+0.1;
		RegenerateLookAtMatrix();
		break;
	case('f'):
	case('F'):
		currentSpeedFactor=currentSpeedFactor*1.5;
		break;
	case('s'):
	case('S'):
		if(currentSpeedFactor>1){
		currentSpeedFactor=currentSpeedFactor/1.5;
		}
		//printf("%f\n",currentSpeedFactor);
		break;
	case('p'):
		glShadeModel(GL_FLAT);
		break;
	case('P'):
		glShadeModel(GL_SMOOTH);
		break;
	case('T'):
	case('t'):
		pauseAll=1;
		break;
	case('c'):
	case('C'):
		pauseAll=0;
		break;
	}
	


	//Superstruct.renderer->MenuInput(c,x,y);
}



void init()
{
	printf("Please wait; Loading....\n");
	float ident[16]={			1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1};
	float cubeMatrix[16]={1,0,0,0,
			      0,1,0,0,
			      0,0,1,0,
			      0,0,-10,1};
	float cubeMatrix3[16];
	float cubeMatrix2[16]={	cos(-20.0),0,sin(-20.0),0,
			       	0,1,0,0,
			       	-sin(-20.0),0,cos(-20.0),0,
				0,0,0,1};
	float cubeMatrix4[16]={	cos(-90.0),0,sin(-90.0),0,
				0,1,0,0,
				-sin(-90.0),0,cos(-90.0),0,
				0,0,0,1};
	float polymatrix[16];
	SphereObject* sphere=new SphereObject;
	CubeObject* cube=new CubeObject;
	PolyObject* poly=new PolyObject;
	windowHeight=500.0;
	windowWidth=889.0;
	aspectRatio=1.7;
	currentZoomFactor=1.0;
	currentSpeedFactor=1.0;
	currentXRotFactor=currentYRotFactor=0.0;
	animationActive=animationStartTick=currentTick=0;
	pauseAll=0;
	animationActive=0;
	fullscreen=0;
	capmode=0;
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	SetStockLookAtMatrix();	
	xViewAngle=90;
	glShadeModel(GL_SMOOTH);

	cone=gluNewQuadric();
	gluQuadricNormals(cone, GLU_SMOOTH);
	gluQuadricTexture(cone, GLU_TRUE);
	gluQuadricDrawStyle(cone, GLU_FILL);
	coneAngle=0;
	TextureLoader(&conetexture[0], "holo.jpg");
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	

	int i=0;
	int j=0;
	cubeMatrix[5]=(float)cos(60.0);
	cubeMatrix[6]=(float)(sin(60.0)*-1);
	cubeMatrix[9]=(float)sin(60.0);
	cubeMatrix[10]=(float)cos(60.0);
//	cubeMatrix[14]=(float)(cos(60.0)*-10);
//	cubeMatrix[13]=(float)(sin(60.0)*-10);
	
//	cubeMatrix[i]=cubeMatrix2[i]*cubeMatrix[i];
	MatMult4(&cubeMatrix2[0], &cubeMatrix[0], &cubeMatrix3[0]);
//	cubeMatrix[14]=0.0;
//	cubeMatrix[12]=10.0;
/*	for(i=0;i<16;i++)
	{
	printf("%f\n", cubeMatrix[i]);
	}
*/	
	glPushMatrix();
	glTranslatef(0.0,0.0,-5.0);
	glRotatef(60,1.0,0.0,0.0);
	glRotatef(-20,0.0,0.0,1.0);
	

	GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
	  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
	GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
	  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
	  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
for(i=0;i<6;i++)
{
	for(j=0;j<3;j++)
	{
		cube->normals[i][j]=n[i][j];
	}
	for(j=0;j<4;j++)
	{
		cube->faces[i][j]=faces[i][j];
	}
}

	cube->vertexes[0][0] = cube->vertexes[1][0] = cube->vertexes[2][0] = cube->vertexes[3][0] = -1;
 	cube->vertexes[4][0] = cube->vertexes[5][0] = cube->vertexes[6][0] = cube->vertexes[7][0] = 1;
 	cube->vertexes[0][1] = cube->vertexes[1][1] = cube->vertexes[4][1] = cube->vertexes[5][1] = -1;
	cube->vertexes[2][1] = cube->vertexes[3][1] = cube->vertexes[6][1] = cube->vertexes[7][1] = 1;
 	cube->vertexes[0][2] = cube->vertexes[3][2] = cube->vertexes[4][2] = cube->vertexes[7][2] = 1;
 	cube->vertexes[1][2] = cube->vertexes[2][2] = cube->vertexes[5][2] = cube->vertexes[6][2] = -1;
	cube->setTexture("borg.jpg");
//	cube->blend=1;
	cube->blendFactor=1.0;
	for(i=0;i<16;i++)
	{
		cube->positionMatrix[i]=cubeMatrix3[i];
	}
	cube->animationTheta=1;
	cube->animationDeltaTheta=0.1;
	cube->animationPhi=1;
	cube->animationDeltaPhi=0.1;
	cuboids->push(*cube);
	cube->positionMatrix[14]=0.0;
	cube->positionMatrix[12]=10.0;
	cuboids->push(*cube);
	cube->positionMatrix[12]=-10.0;
	cuboids->push(*cube);
	cube->positionMatrix[12]=0.0;
	cube->positionMatrix[14]=10.0;
	cuboids->push(*cube);
	for(i=0;i<16;i++)
	{
		sphere->positionMatrix[i]=ident[i];
		poly->positionMatrix[i]=ident[i];
//		printf("%f\n",cubeMatrix3[i]);
	}
//	printf("-------------------\n");
//	for(i=0;i<16;i++)
//	{
//		printf("%f\n",cube->positionMatrix[i]);
//	}
	sphere->radius=8;
	sphere->slices=90;
	sphere->squares=90;
	sphere->animationTheta=0;
	sphere->animationDeltaTheta=0.02;
	sphere->animationPhi=0;
	sphere->animationDeltaPhi=0.01;
	sphere->positionMatrix[5]=0.0;
	sphere->positionMatrix[6]=-1.0;
	sphere->positionMatrix[9]=1.0;
	sphere->positionMatrix[10]=0.0;
	sphere->setTexture("World.jpg");
//	sphere->blend=1;
	sphere->blendFactor=0.8;
	spheroids->push(*sphere);
	poly->animationTheta=0;
	poly->animationPhi=0;
	poly->animationDeltaTheta=0;
	poly->animationDeltaPhi=0;
	poly->normal[0]=0.0;
	poly->normal[1]=1.0;
	poly->normal[2]=0.0;
	poly->vertexes[0][0]=poly->vertexes[1][0]=-40.0;
	poly->vertexes[0][2]=poly->vertexes[3][2]=-40.0;
	poly->vertexes[1][2]=poly->vertexes[2][2]=40.0;
	poly->vertexes[2][0]=poly->vertexes[3][0]=40.0;
	poly->vertexes[0][1]=poly->vertexes[1][1]=poly->vertexes[2][1]=poly->vertexes[3][1]=0.0;
	poly->positionMatrix[13]=-30.0;
	poly->texSRepeat=20.0;
	poly->texTRepeat=20.0;
	poly->setTexture("floor.png");
	polys->push(*poly);
	poly->positionMatrix[14]=-40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=-40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=40.0;
	polys->push(*poly);
	poly->positionMatrix[14]=0.0;
	polys->push(*poly);
	poly->positionMatrix[14]=40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=0.0;
	polys->push(*poly);
	poly->positionMatrix[12]=-40.0;
	polys->push(*poly);
	poly->texture[0]=0;
	poly->setTexture("Betterfloor.jpg");
	poly->positionMatrix[13]=30.0;
	poly->positionMatrix[5]=-1.0;
	polys->push(*poly);
	poly->positionMatrix[14]=-40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=-40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=40.0;
	polys->push(*poly);
	poly->positionMatrix[14]=0.0;
	polys->push(*poly);
	poly->positionMatrix[14]=40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=0.0;
	polys->push(*poly);
	poly->positionMatrix[12]=-40.0;
	polys->push(*poly);
	poly->texSRepeat=40.0;
	poly->texTRepeat=40.0;
	poly->setTexture("Betterwall.jpg");
	poly->positionMatrix[5]=0.0;
	poly->positionMatrix[13]=0.0;
	poly->positionMatrix[6]=1.0;
	poly->positionMatrix[9]=-1.0;
	poly->positionMatrix[10]=0.0;
	poly->positionMatrix[14]=-80.0;
	polys->push(*poly);
	poly->positionMatrix[12]=-40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=40.0;
	polys->push(*poly);
	poly->positionMatrix[14]=80.0;
	polys->push(*poly);
	poly->positionMatrix[12]=-40.0;
	polys->push(*poly);
	poly->positionMatrix[12]=40.0;
	polys->push(*poly);
	for(i=0;i<16;i++)
	{
		poly->positionMatrix[i]=ident[i];
	}
	poly->positionMatrix[1]=1;
	poly->positionMatrix[0]=0;
	poly->positionMatrix[4]=-1;
	poly->positionMatrix[5]=0;
	poly->positionMatrix[12]=80;
	polys->push(*poly);
	poly->positionMatrix[14]=-40;
	polys->push(*poly);
	poly->positionMatrix[14]=40;
	polys->push(*poly);
	poly->positionMatrix[14]=0;
	poly->positionMatrix[1]=-1;
	poly->positionMatrix[12]=-40;
	polys->push(*poly);
	poly->setTexture("Door.jpg");
	poly->texSRepeat=1;
	poly->texTRepeat=1;
	poly->positionMatrix[12]=-80;
	poly->positionMatrix[14]=-40;
	polys->push(*poly);
	poly->positionMatrix[14]=40;
	polys->push(*poly);
//	cuboids->ScaleAll(2);
//	spheroids->ScaleAll(2);
	for(i=0;i<16;i++)
	{
		poly->positionMatrix[i]=ident[i];
	}
	poly->texSRepeat=1;
	poly->texTRepeat=1;
	poly->setTexture("StarTrekTNGPanel.jpg");

	poly->vertexes[0][0]=-5;
	poly->vertexes[0][1]=-1;
	poly->vertexes[0][2]=10;

	poly->vertexes[1][0]=5;
	poly->vertexes[1][1]=-1;
	poly->vertexes[1][2]=10;

	poly->vertexes[2][0]=10;
	poly->vertexes[2][1]=-6;
	poly->vertexes[2][2]=15;

	poly->vertexes[3][0]=-10;
	poly->vertexes[3][1]=-6;
	poly->vertexes[3][2]=15;

	poly->positionMatrix[13]=-24;
	polys->push(*poly);
	poly->positionMatrix[10]=-1;
	polys->push(*poly);
	poly->positionMatrix[10]=1;
	poly->positionMatrix[0]=0;
	poly->positionMatrix[2]=1;
	poly->positionMatrix[8]=1;
	poly->positionMatrix[10]=0;
	polys->push(*poly);
	poly->positionMatrix[8]=-1;
	polys->push(*poly);
	for(i=0;i<16;i++)
	{
		poly->positionMatrix[i]=ident[i];
	}
	poly->setTexture("BlackTex.jpg");
	
	poly->vertexes[0][0]=-5;
	poly->vertexes[0][1]=-1;
	poly->vertexes[0][2]=10;

	poly->vertexes[1][0]=-10;
	poly->vertexes[1][1]=-1;
	poly->vertexes[1][2]=5;

	poly->vertexes[2][0]=-15;
	poly->vertexes[2][1]=-6;
	poly->vertexes[2][2]=10;

	poly->vertexes[3][0]=-10;
	poly->vertexes[3][1]=-6;
	poly->vertexes[3][2]=15;

	poly->positionMatrix[13]=-24;
	polys->push(*poly);
	poly->positionMatrix[0]=-1;
	polys->push(*poly);
	poly->positionMatrix[10]=-1;
	polys->push(*poly);
	poly->positionMatrix[0]=1;
	polys->push(*poly);
	for(i=0;i<16;i++)
	{
		poly->positionMatrix[i]=ident[i];
	}
	poly->vertexes[0][0]=-5;
	poly->vertexes[0][1]=-1;
	poly->vertexes[0][2]=10;

	poly->vertexes[1][0]=-10;
	poly->vertexes[1][1]=-1;
	poly->vertexes[1][2]=5;

	poly->vertexes[2][0]=-10;
	poly->vertexes[2][1]=-6;
	poly->vertexes[2][2]=5;

	poly->vertexes[3][0]=-5;
	poly->vertexes[3][1]=-6;
	poly->vertexes[3][2]=10;

	poly->positionMatrix[13]=-24;
	polys->push(*poly);
	poly->positionMatrix[0]=-1;
	polys->push(*poly);
	poly->positionMatrix[10]=-1;
	polys->push(*poly);
	poly->positionMatrix[0]=1;
	polys->push(*poly);
	for(i=0;i<16;i++)
	{
		poly->positionMatrix[i]=ident[i];
	}
	poly->vertexes[0][0]=-5;
	poly->vertexes[0][1]=-1;
	poly->vertexes[0][2]=10;

	poly->vertexes[1][0]=5;
	poly->vertexes[1][1]=-1;
	poly->vertexes[1][2]=10;

	poly->vertexes[2][0]=5;
	poly->vertexes[2][1]=-6;
	poly->vertexes[2][2]=10;

	poly->vertexes[3][0]=-5;
	poly->vertexes[3][1]=-6;
	poly->vertexes[3][2]=10;

	poly->positionMatrix[13]=-24;
	polys->push(*poly);
	poly->positionMatrix[10]=-1;
	polys->push(*poly);
	poly->positionMatrix[10]=1;
	poly->positionMatrix[0]=0;
	poly->positionMatrix[2]=1;
	poly->positionMatrix[8]=1;
	poly->positionMatrix[10]=0;
	polys->push(*poly);
	poly->positionMatrix[8]=-1;
	polys->push(*poly);
	for(i=0;i<16;i++)
	{
		poly->positionMatrix[i]=ident[i];
	}
	poly->setTexture("hologram.jpg");
	poly->vertexes[0][0]=-11;
	poly->vertexes[0][1]=-1;
	poly->vertexes[0][2]=11;

	poly->vertexes[1][0]=-11;
	poly->vertexes[1][1]=-1;
	poly->vertexes[1][2]=-11;

	poly->vertexes[2][0]=11;
	poly->vertexes[2][1]=-1;
	poly->vertexes[2][2]=-11;

	poly->vertexes[3][0]=11;
	poly->vertexes[3][1]=-1;
	poly->vertexes[3][2]=11;
	poly->positionMatrix[13]=-28;
	polys->push(*poly);
	delete poly;
	delete cube;
	delete sphere;
	MakeLights();
	glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	glMateriali(GL_FRONT, GL_SHININESS, 56);
	glPopMatrix();	
//	BCinit();
}


int main(int argc, char **argv)
{
	putenv( (char *) "__GL_SYNC_TO_VBLANK=1" ); //This attempts to VSync the image, only works under certain conditions however (is hardware specific)
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(889,500);
	glutCreateWindow("Redezem's Assignment");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
//	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardInput);
	init();
	glutMainLoop();
	return 0;
}
