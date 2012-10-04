#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "Cube.h"
#include "Sphere.h"
#include "Polygon.h"
//including objects
//#include "rendax.h"
//#include "object.h"
//Ripped from Rendax
double windowHeight, windowWidth, aspectRatio, currentLookAtMatrix[3][3], currentZoomFactor, currentSpeedFactor, currentXRotFactor, currentYRotFactor;
int animationActive, animationStartTick, xRotStartTick, yRotStartTick, currentTick, pauseAll, bufferNumber, capmode,fullscreen;

CubeList* cuboids=new CubeList;
SphereList* spheroids=new SphereList;
PolyList* polys=new PolyList;

GLfloat light_diffuse[] = {1.0,1.0,1.0,1.0};
GLfloat light_position[] = {1.0,1.0,1.0,0.0};
GLfloat light_ambient[] = {1.0,1.0,1.0,1.0};
GLfloat light_specular[] = {1.0,1.0,1.0,1.0};
GLfloat specular_position[] = {-1.0,1.0,1.0,0.0};

float specReflection[] = { 0.8, 0.8, 0.8, 1.0 };
float specReflectionforSphere[] = { 1.0, 1.0, 1.0, 1.0};


void idle(void)
{
	if(capmode!=0)
	{
	#ifdef WIN32
	Sleep(17);
	#endif
	}
	glutPostRedisplay();
}

void MatMult4(float* AIN, float* BIN, float* OUTAGE)
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

void ResizePerspectiveMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective((40.0/currentZoomFactor),aspectRatio,1.0,200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetStockLookAtMatrix()
{
	ResizePerspectiveMatrix();
	gluLookAt(	0.0,0.0,30.0,
			0.0,0.0,0.0,
			0.0,1.0,0.0);
	currentLookAtMatrix[0][0]=currentLookAtMatrix[0][1]=currentLookAtMatrix[1][0]=currentLookAtMatrix[1][1]=currentLookAtMatrix[1][2]=currentLookAtMatrix[2][0]=currentLookAtMatrix[2][2]=0.0;
	currentLookAtMatrix[0][2]=30;
	currentLookAtMatrix[2][1]=1.0;
}

void RegenerateLookAtMatrix()
{
	ResizePerspectiveMatrix();
	gluLookAt(	currentLookAtMatrix[0][0],currentLookAtMatrix[0][1],currentLookAtMatrix[0][2],
				currentLookAtMatrix[1][0],currentLookAtMatrix[1][1],currentLookAtMatrix[1][2],
				currentLookAtMatrix[2][0],currentLookAtMatrix[2][1],currentLookAtMatrix[2][2]);
}


void ComputeAnimation()
{
	int animTick=currentTick-animationStartTick;
	//that *should* do...
}
void MakeLights()
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


void display()
{
//	BCdisplay();
	//Superstruct.renderer->DrawWorld();
	 //from Rendax
	currentTick=currentTick+(int)(currentSpeedFactor*10.0);
	if(pauseAll==0)
	{
		if(animationActive==1)
		{
			ComputeAnimation();
		}
		if(currentXRotFactor!=0.0)
		{
			//ComputeXRot();
		}
		if(currentYRotFactor!=0.0)
		{
			//ComputeYRot();
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

	cuboids->animate(1,1);
		//cube->draw();
		cuboids->draw();
		polys->draw();
		cuboids->draw();
		glMaterialfv(GL_FRONT, GL_SPECULAR, specReflectionforSphere);
		glMateriali(GL_FRONT, GL_SHININESS, 100);
		spheroids->draw();
		glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
		glMateriali(GL_FRONT, GL_SHININESS, 56);
		//Put the things that need to be drawn here
//	printf("ran\n");

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
		RegenerateLookAtMatrix();
	}
}

void keyboardInput(unsigned char c, int x, int y)
{
	switch(c)
	{
	case ('c'):
		SwitchCapMode();
		printf("Switch!\n");
		break;
	case ('w'):
		SizeChange();
		break;
	case ('f'):
		FullScreen16by9();
		break;
	case ('q'):
		exit(0);
		break;
	}
	


	//Superstruct.renderer->MenuInput(c,x,y);
}



void init()
{
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
	currentSpeedFactor=0.1;
	currentXRotFactor=currentYRotFactor=0.0;
	animationActive=animationStartTick=currentTick=0;
	pauseAll=0;
	animationActive=1;
	fullscreen=0;
	capmode=0;
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	SetStockLookAtMatrix();	

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
	cube->blend=1;
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
	sphere->animationDeltaTheta=0;
	sphere->animationPhi=0;
	sphere->animationDeltaPhi=0;
	sphere->positionMatrix[5]=0.0;
	sphere->positionMatrix[6]=-1.0;
	sphere->positionMatrix[9]=1.0;
	sphere->positionMatrix[10]=0.0;
	sphere->setTexture("world.jpg");
	sphere->blend=1;
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
	poly->positionMatrix[13]=-15.0;
	poly->texSRepeat=20.0;
	poly->texTRepeat=20.0;
	poly->setTexture("floor.png");
	polys->push(*poly);
	poly->texture[0]=0;
	poly->setTexture("Betterfloor.jpg");
	poly->positionMatrix[13]=15.0;
	poly->positionMatrix[5]=-1.0;
	polys->push(*poly);
	poly->texSRepeat=40.0;
	poly->texTRepeat=40.0;
	poly->setTexture("Betterwall.jpg");
	poly->positionMatrix[5]=0.0;
	poly->positionMatrix[13]=0.0;
	poly->positionMatrix[6]=1.0;
	poly->positionMatrix[9]=-1.0;
	poly->positionMatrix[10]=0.0;
	poly->positionMatrix[14]=-40.0;
	polys->push(*poly);
	poly->positionMatrix[14]=40.0;
	polys->push(*poly);
	for(i=0;i<16;i++)
	{
		poly->positionMatrix[i]=ident[i];
	}
	poly->positionMatrix[1]=1;
	poly->positionMatrix[0]=0;
	poly->positionMatrix[4]=-1;
	poly->positionMatrix[5]=0;
	poly->positionMatrix[12]=40;
	polys->push(*poly);
	poly->positionMatrix[1]=-1;
	poly->positionMatrix[12]=-40;
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
	putenv( (char *) "__GL_SYNC_TO_VBLANK=1" );
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
