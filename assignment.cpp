#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

//including objects
#include "rendax.h"
#include "object.h"

//for debugging purposes
//#include "BetterCube.h"
//that done, let's get ON IT! YEAH!

struct{ //Hack because C++ is painful to work with
	Rendax renderer;
} Superstruct;

void display()
{
//	BCdisplay();
	Superstruct.renderer.DrawWorld();
}

void reshape(int w, int h)
{
//	BCreshape(w,h);
	Superstruct.renderer.RescaleWorld(w,h);
}

void keyboardInput(unsigned char c, int x, int y)
{
	Superstruct.renderer.MenuInput(c,x,y);
}

void init()
{
	
//	BCinit();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Redezem's Assignment");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardInput);
	init();
	glutMainLoop();
	return 0;
}
