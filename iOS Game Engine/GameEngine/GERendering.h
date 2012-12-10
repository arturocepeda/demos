
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  iOS Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERendering.h ---
//
//////////////////////////////////////////////////////////////////


#ifndef _GERENDERING_H_
#define _GERENDERING_H_

#include "GERenderingObjects.h"

#define TEXTURES 256

class GERendering
{
private:
   EAGLContext* glContext;
   
   GLuint tTextures[TEXTURES];
   GETextureSize tTextureSize[TEXTURES];
   
   GEColor cBackgroundColor;
   float fOrientationAngle;
   unsigned int iNumLights;

public:
	GERendering(EAGLContext* Context);
	~GERendering();

   // textures
   void loadTexture(unsigned int TextureIndex, NSString* Name);
   GLuint getTexture(unsigned int TextureIndex);
   GETextureSize& getTextureSize(unsigned int TextureIndex);
   
   // lighting (TODO)
   void switchLighting(bool On);
   
   void setAmbientLight(unsigned char R, unsigned char G, unsigned char B);

   unsigned int createDirectionalLight(float R, float G, float B, float A, float Range,
                                       float DirX, float DirY, float DirZ);
   unsigned int createPointLight(float R, float G, float B, float A, float Range, float Attenuation,
                                 float PosX, float PosY, float PosZ);
   unsigned int createSpotLight(float R, float G, float B, float A, float Range, float Attenuation,
                                float PosX, float PosY, float PosZ, 
                                float DirX, float DirY, float DirZ,
                                float Theta, float Phi, float Falloff);

   void switchLight(unsigned int Light, bool On);
   void moveLight(unsigned int Light, float DX, float DY, float DZ);
   void setLightColor(unsigned int Light, float R, float G, float B, float A);
   void setLightRange(unsigned int Light, float Range);
   void setLightPosition(unsigned int Light, float PosX, float PosY, float PosZ);
   void setLightDirection(unsigned int Light, float DirX, float DirY, float DirZ);
   
   // device orientation
   void setOrientationAngle(float Angle);
   
   // background
   void setBackgroundColor(float R, float G, float B);
   
   // camera
   void useCamera(GECamera* Camera);

   // rendering
   void renderBegin();
   void renderMesh(GEMesh* Mesh);
   void renderSprite(GESprite* Sprite);
   void renderLabel(GELabel* Label);
   void renderEnd();
   
   // rendering mode
   void set2D(bool Portrait = true);
   void set3D();
};

#endif
