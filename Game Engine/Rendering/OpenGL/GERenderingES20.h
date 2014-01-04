
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  iOS Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingES20.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Rendering/GERendering.h"
#include "GERenderingObjectsES20.h"
#include "GERenderingShadersES20.h"
#include "Core/GEUtils.h"

#define FONTS 4

class GERenderingES20 : public GERendering
{
private:
   EAGLContext* glContext;
   GEColor cColor;
   
   // matrices
   GLKMatrix4 matProjection;
   GLKMatrix4 matView;
   GLKMatrix4 matModel;
   
   GLKMatrix3 matNormal;
   GLKMatrix4 matModelView;
   GLKMatrix4 matModelViewProjection;

   // camera
   GEVector3 vCameraPosition;
   GEVector3 vCameraRotation;
    
   // fonts
   UIFont* fFonts[FONTS];
   
   // shaders
   GEShaderProgram sPrograms[GEShaderPrograms::Count];
   GLuint iUniforms[GEShaderPrograms::Count][GEUniforms::Count];
   unsigned int iActiveProgram;
   
   void setupProjectionMatrix();
   void clearBuffers();
    
   void loadShaders();   
   void attachShaders(unsigned int iProgramIndex, GEVertexShader& cVertexShader, GEFragmentShader& cFragmentShader);
   void linkProgram(unsigned int iProgramIndex);
   bool checkProgram(unsigned int iProgramIndex);
   void getUniformsLocation(unsigned int iProgramIndex);

public:
   GERenderingES20(EAGLContext* Context);
   ~GERenderingES20();
    
   // textures
   void loadTexture(unsigned int TextureIndex, const char* Name);
   void loadTextureCompressed(unsigned int TextureIndex, const char* Name,
                              unsigned int Size, unsigned int BPP, bool Alpha = false);
   
   // camera
   void useCamera(GECamera* Camera);
   
   // rendering mode
   void set2D(bool Portrait = true);
   void set3D(bool Portrait = true);
   void useShaderProgram(unsigned int iProgramIndex);
    
   // rendering objects factory
   void createMesh(GEMesh** Mesh);
   void createSprite(GESprite** Sprite);
   void createLabel(GELabel** Label, unsigned int Font, GEAlignment Alignment,
                    unsigned int Width, unsigned int Height, const char* Text = "");
   void createCamera(GECamera** Camera);
   
   // lighting
   unsigned int createDirectionalLight(const GEColor& Color, float Range,
                                       const GEVector3& Direction) { return 0; }
   unsigned int createPointLight(const GEColor& Color, float Range, float Attenuation,
                                 const GEVector3& Position) { return 0; }
   unsigned int createSpotLight(const GEColor& Color, float Range, float Attenuation,
                                const GEVector3& Position, const GEVector3& Direction,
                                float Theta, float Phi, float Falloff) { return 0; }
    
   // fonts
   void defineFont(unsigned int Font, const char* FontName, float Size,
                   bool Bold = false, bool Italic = false);
   void releaseFont(unsigned int Font);
    
   // transformations
   void worldToScreen(const GEVector3* PositionWorld, GEVector3* PositionScreen) {}
   void screenToWorld(const GEVector3* PositionScreen, GEVector3* WorldPointNear, GEVector3* WorldPointFar) {}
    
   // rendering
   void renderBegin();
   void renderMesh(GEMesh* Mesh);
   void renderSprite(GESprite* Sprite);
   void renderLabel(GELabel* Label);
   void renderEnd();
};
