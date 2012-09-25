#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

struct CubeDef
{
	double height, width;
	GLfloat vertexes[8][3];
	GLfloat normals[6][3];
	GLint faces[6][4];
};

struct CubeDef cube;

GLfloat light_diffuse[] = {1.0,0.0,0.0,1.0};
GLfloat light_position[] = {1.0,1.0,1.0,0.0};

void drawBox()
{
	int i;
akeLights()
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	
	for (i=0;i<6;i++)
	{
		glBegin(GL_QUADS);
		glNormal3fv(&cube.normals[i][0]);
		glVertex3fv(&cube.vertexes[cube.faces[i][0]][0]);
		glVertex3fv(&cube.vertexes[cube.faces[i][1]][0]);
		glVertex3fv(&cube.vertexes[cube.faces[i][2]][0]);
		glVertex3fv(&cube.vertexes[cube.faces[i][3]][0]);
		glEnd();
	}
}

void BCdisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawBox();
	glutSwapBuffers();
}

void SetBoxSegments()
{
int i=0;
int j=0;

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
		cube.normals[i][j]=n[i][j];
	}
	for(j=0;j<4;j++)
	{
		cube.faces[i][j]=faces[i][j];
	}
}
	
}

void BuildBox()
{
	cube.vertexes[0][0] = cube.vertexes[1][0] = cube.vertexes[2][0] = cube.vertexes[3][0] = -1;
 	cube.vertexes[4][0] = cube.vertexes[5][0] = cube.vertexes[6][0] = cube.vertexes[7][0] = 1;
 	cube.vertexes[0][1] = cube.vertexes[1][1] = cube.vertexes[4][1] = cube.vertexes[5][1] = -1;
	cube.vertexes[2][1] = cube.vertexes[3][1] = cube.vertexes[6][1] = cube.vertexes[7][1] = 1;
 	cube.vertexes[0][2] = cube.vertexes[3][2] = cube.vertexes[4][2] = cube.vertexes[7][2] = 1;
 	cube.vertexes[1][2] = cube.vertexes[2][2] = cube.vertexes[5][2] = cube.vertexes[6][2] = -1;

}

void MakeLights()
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}

void CubeRotate()
{
	glTranslatef(0.0,0.0,-5.0);
	glRotatef(60,1.0,0.0,0.0);
	glRotatef(-20,0.0,0.0,1.0);
}

void BCinit()
{
	SetBoxSegments();
	BuildBox();
	MakeLights();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0,cube.width/cube.height,1.0,10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(	0.0,0.0,0.5,
			0.0,0.0,0.0,
			0.0,1.0,0.0);
	CubeRotate();
}

void BCreshape(int w, int h)
{
	int i;
	glViewport(0,0,(GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	cube.width=w;
	cube.height=h;
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	gluPerspective(40.0,cube.width/cube.height,1.0,10.0*cube.height/500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	0.0,0.0,0.5,
			0.0,0.0,0.0,
			0.0,1.0,0.0);
	glScalef(cube.height/500,cube.height/500,cube.height/500);
	for(i=0;i<4;i++)
	{
		light_diffuse[i]=light_diffuse[i]*(cube.height/500);
		light_position[i]=light_position[i]*(cube.height/500);
	}
	MakeLights();
	CubeRotate();
}


int BCmain(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("A Far Superior Cube");
	glutMainLoop();
	return 0;
}
