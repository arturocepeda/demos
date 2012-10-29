
/*
   Arturo Cepeda PŽrez

	Rendering Engine (OpenGL)
   
   --- GERendering.mm ---
*/

#include "GERendering.h"


//
//  GERendering
//
GERendering::GERendering(EAGLContext* Context, GLuint ViewFrameBuffer, GLuint ViewRenderBuffer)
{
   glContext = Context;
   glViewFrameBuffer = ViewFrameBuffer;
   glViewRenderBuffer = ViewRenderBuffer;
   
   bOrientation180 = false;
   cBackground.set(0.0f, 0.0f, 0.0f);
   iNumLights = 0;
   
   // set OpenGL context
   [EAGLContext setCurrentContext:glContext];
   
	// enable OpenGL texturing
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
   glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
   // generate textures
   glGenTextures(TEXTURES, tTextures);
   memset(tTextureSize, 0, sizeof(GETextureSize) * TEXTURES);
   
   // set 2D mode
   glDisable(GL_DEPTH_TEST);
   setProjectionMatrix();
}

GERendering::~GERendering()
{
   // release textures
   glDeleteTextures(TEXTURES, tTextures);
}

void GERendering::setProjectionMatrix()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   if(bOrientation180)
      glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
   
   glOrthof(-1.0f, 1.0f, -GEDevice::getAspectRatio(), GEDevice::getAspectRatio(), -1.0f, 1.0f);   
   glMatrixMode(GL_MODELVIEW);
}

void GERendering::setOrientation180(bool Orientation180)
{
   if(bOrientation180 != Orientation180)
   {
      bOrientation180 = Orientation180;
      setProjectionMatrix();
   }
}

void GERendering::loadTexture(unsigned int TextureIndex, NSString* Name)
{
   if(TextureIndex >= TEXTURES)
      return;
   
	CGImageRef cgImage = [UIImage imageNamed:Name].CGImage;
	CGContextRef cgContext;
	GLubyte* pData = nil;	
	size_t iWidth, iHeight;
	
	if(!cgImage)
      return;
   
   glBindTexture(GL_TEXTURE_2D, tTextures[TextureIndex]);
   
	iWidth = CGImageGetWidth(cgImage);
	iHeight = CGImageGetHeight(cgImage);
   
   tTextureSize[TextureIndex].Width = iWidth;
   tTextureSize[TextureIndex].Height = iHeight;
	
	pData = (GLubyte*)calloc(iWidth * iHeight * 4, sizeof(GLubyte));
   
	// Uses the bitmap creation function provided by the Core Graphics framework. 
	cgContext = CGBitmapContextCreate(pData, iWidth, iHeight, 8, iWidth * 4, 
                                     CGImageGetColorSpace(cgImage), kCGImageAlphaPremultipliedLast);
   
	// After you create the context, you can draw the image to the context.
	CGContextDrawImage(cgContext, CGRectMake(0.0f, 0.0f, (CGFloat)iWidth, (CGFloat)iHeight), 
                      cgImage);
   
	// You don't need the context at this point, so you need to release it to avoid memory leaks.
	CGContextRelease(cgContext);
   
	// setup texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	free(pData);
   
   glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint GERendering::getTexture(unsigned int TextureIndex)
{
   return tTextures[TextureIndex];
}

GETextureSize& GERendering::getTextureSize(unsigned int TextureIndex)
{
   return tTextureSize[TextureIndex];
}


//
//  Background
//
void GERendering::setBackgroundColor(float R, float G, float B)
{
   cBackground.R = R;
   cBackground.G = G;
   cBackground.B = B;
}


//
//  Rendering
//
void GERendering::renderBegin()
{
   glBindFramebufferOES(GL_FRAMEBUFFER_OES, glViewFrameBuffer);             
   glClearColor(cBackground.R, cBackground.G, cBackground.B, 1.0f);
	glClearDepthf(1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GERendering::renderSprite(GESprite* Sprite)
{
   Sprite->render();
}

void GERendering::renderLabel(GELabel* Label)
{
   Label->render();
}

void GERendering::renderEnd()
{
	glBindTexture(GL_TEXTURE_2D, 0);
   glBindRenderbufferOES(GL_RENDERBUFFER_OES, glViewRenderBuffer);
   [glContext presentRenderbuffer: GL_RENDERBUFFER_OES]; 
}
