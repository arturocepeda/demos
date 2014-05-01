
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingObjectsES20.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GETypes.h"
#include "Rendering/GERenderingObjects.h"

#if TARGET_OS_IPHONE
#include <GLKit/GLKit.h>
#else
#include <GLES2/gl2.h>
#endif


//
//  Custom types for specific platforms
//
#if TARGET_OS_IPHONE
typedef EAGLContext CTContext;
typedef GLKMatrix3 CTMatrix3;
typedef GLKMatrix4 CTMatrix4;
typedef UIFont CTFont;
#else
typedef void CTContext;
typedef GEMatrix3 CTMatrix3;
typedef GEMatrix4 CTMatrix4;
typedef void CTFont;
#endif


//
//  Shader uniforms
//
namespace GEUniforms
{
   enum
   {
      ModelViewProjection, 
      ModelView,
      Normal,
      ObjectColor,
      Texture0,
      AmbientLightColor,
      AmbientLightIntensity,
      PointLight1Position,
      PointLight1Color,
      PointLight1Intensity,
      
      Count
   };
}


//
//  Vertex attributes
//
struct
{
   enum
   {
      Position,
      TextureCoord0,
      Normal,
      
      Count
   };
}
GEVertexAttributes;


//
//  GEOpenGLElement
//
class GEOpenGLElement
{
protected:
    // vertex data
   float* fVertexData;   
   int iVertexStride;
   
   GEOpenGLElement();
   ~GEOpenGLElement();
    
public:
   void getModelMatrix(GEMatrix4* ModelMatrix);
};


//
//  GEMeshES20
//
class GEMeshES20 : public GEMesh, public GEOpenGLElement
{
private:
   bool bUseTexture;
   
   void unload();
   
public:
   GEMeshES20();
   ~GEMeshES20();

   void getModelMatrix(GEMatrix4* ModelMatrix);
    
   void loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals);
   void loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals, float* TextureCoordinate);
   
   void render();
};


//
//  GESpriteES20
//
class GESpriteES20 : public GESprite, public GEOpenGLElement
{
public:
   GESpriteES20();
   ~GESpriteES20();

   void getModelMatrix(GEMatrix4* ModelMatrix);
    
   void render();
    
   void setCenter(const GEVector3& Center);
   void setTextureCoordinates(float Ax, float Ay, float Bx, float By,
                              float Cx, float Cy, float Dx, float Dy);
};


//
//  GELabelES20
//
class GELabelES20 : public GELabel, public GEOpenGLElement
{
private:
   CTFont* fFont;
   char sText[128];
   unsigned int iAligment;
   
   unsigned int iWidth;
   unsigned int iHeight;
   
   float fMaxS;
   float fMaxT;
   
   unsigned int convertAlignment(GEAlignment iAlignment);
    
   void fillTexture();
   
public:
   GELabelES20(CTFont* Font, const char* Text, GEAlignment TextAligment,
               unsigned int Width, unsigned int Height);
   ~GELabelES20();

   void getModelMatrix(GEMatrix4* ModelMatrix);
    
   void render();
   
   void setText(const char* Text);
};
