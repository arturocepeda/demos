
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
#include "GERenderingShaders.h"
#include "../../Core/GEUtils.h"

#define TEXTURES 256

class GERendering
{
private:
   EAGLContext* glContext;
   GEColor cBackgroundColor;
   GEColor cColor;
    
   // coordinates
   GELine* cPixelToScreenX;
   GELine* cPixelToScreenY;
   
   // matrices
   GLKMatrix4 matProjection;
   GLKMatrix4 matView;
   GLKMatrix4 matModel;
   
   GLKMatrix3 matNormal;
   GLKMatrix4 matModelView;
   GLKMatrix4 matModelViewProjection;

   // textures
   GLuint tTextures[TEXTURES];
   GETextureSize tTextureSize[TEXTURES];

   // camera
   GEVector3 vCameraPosition;
   GEVector3 vCameraRotation;
   
   // lighting
   unsigned int iNumberOfActiveLights;
   GELight sAmbientLight;
   GELight sLights[GELights.Count];
   
   // shaders
   GEProgram sPrograms[GEPrograms.Count];
   GLuint iUniforms[GEPrograms.Count][GEUniforms.Count];
   unsigned int iActiveProgram;
   
   void loadShaders();   
   void attachShaders(unsigned int iProgramIndex, GEVertexShader& cVertexShader, GEFragmentShader& cFragmentShader);
   void linkProgram(unsigned int iProgramIndex);
   bool checkProgram(unsigned int iProgramIndex);
   void getUniformsLocation(unsigned int iProgramIndex);

public:
	GERendering(EAGLContext* Context);
	~GERendering();

   // screen
   GEVector2 pixelToScreen(const GEVector2& PixelPosition);
    
   // textures
   void loadTexture(unsigned int TextureIndex, NSString* Name);
   void loadTextureCompressed(unsigned int TextureIndex, NSString* Name,
                              unsigned int Size, unsigned int BPP, bool Alpha = false);
   GLuint getTexture(unsigned int TextureIndex);
   GETextureSize& getTextureSize(unsigned int TextureIndex);
   
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
   void set3D(bool Portrait = true);
   void useProgram(unsigned int iProgramIndex);
   
   // lighting
   void setAmbientLightColor(float R, float G, float B);
   void setAmbientLightColor(const GEColor& Color);
   void setAmbientLightIntensity(float Intensity);
   
   void setNumberOfActiveLights(unsigned int N);
   void moveLight(unsigned int LightIndex, float DX, float DY, float DZ);
   void moveLight(unsigned int LightIndex, const GEVector3& D);
   void setLightPosition(unsigned int LightIndex, float PosX, float PosY, float PosZ);
   void setLightPosition(unsigned int LightIndex, const GEVector3& Position);
   void setLightColor(unsigned int LightIndex, float R, float G, float B);
   void setLightColor(unsigned int LightIndex, const GEColor& Color);
   void setLightIntensity(unsigned int LightIndex, float Intensity);
};

#endif
