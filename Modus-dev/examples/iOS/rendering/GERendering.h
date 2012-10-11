
/*
   Arturo Cepeda PŽrez

	Rendering Engine (OpenGL)

   --- GERendering.h ---
*/

#ifndef _GERENDERING_H_
#define _GERENDERING_H_

#include "GERenderingObjects.h"

#define TEXTURES 65536

class GERendering
{
private:
   EAGLContext* glContext;
   GLuint glViewFrameBuffer;
   GLuint glViewRenderBuffer;

   GLuint tTextures[TEXTURES];
   GETextureSize tTextureSize[TEXTURES];
   
   GEColor cBackground;
   unsigned int iNumLights;

public:
	GERendering(EAGLContext* Context, GLuint ViewFrameBuffer, GLuint ViewRenderBuffer);
	~GERendering();

   // textures
   void loadTexture(unsigned int TextureIndex, NSString* Name);
   GLuint getTexture(unsigned int TextureIndex);
   GETextureSize& getTextureSize(unsigned int TextureIndex);
   
   // background
   void setBackgroundColor(float R, float G, float B);

   // rendering
   void renderBegin();
   void renderSprite(GESprite* Sprite);
   void renderLabel(GELabel* Label);
   void renderEnd();
};

#endif
