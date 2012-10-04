#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#ifdef WIN32
#include <SOIL.h>
#else
#include "SOIL/SOIL.h"
#endif

void TextureLoader(GLuint*, char*);