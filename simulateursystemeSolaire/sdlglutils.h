#ifndef SDLGLUTILS_H
#define SDLGLUTILS_H

#include <gl.h>
#include <SDL.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

GLuint loadTexture(const char * filename,bool useMipMap = true);
int takeScreenshot(const char * filename);
void drawAxis(double scale = 1);


#endif //SDLGLUTILS_H
