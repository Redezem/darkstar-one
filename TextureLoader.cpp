#include "TextureLoader.h"
void TextureLoader(GLuint* herpaderp, char* fileName) //So what? I'm a horrible person. Good for you.
{
	herpaderp[0]=SOIL_load_OGL_texture(fileName,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

	if(herpaderp[0] == 0)
	{
		printf("TextureWarning: %s could not load", fileName);
		return;
	}
		printf("%d\n",herpaderp[0]);
	glBindTexture(GL_TEXTURE_2D, herpaderp[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}