
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  iOS Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingObjectsES20.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GETypes.h"
#include "Rendering/GERenderingObjects.h"
#include <GLKit/GLKit.h>


//
//  Shader uniforms
//
struct
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
GEUniforms;


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
    // vertex data pointers
    float* fVertex;
    float* fTextureCoordinate;
    float* fNormals;
    
    // vertex array and vertex buffers
    GLuint iVertexArray;
    GLuint iVertexBuffers[GEVertexAttributes.Count];
    
public:
    void getModelMatrix(GEMatrix4* ModelMatrix);
};


//
//  GEMeshES20
//
class GEMeshES20 : public GEMesh, public GEOpenGLElement
{
public:
   GEMeshES20();
   ~GEMeshES20();

   void getModelMatrix(GEMatrix4* ModelMatrix);
    
   void loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals);
   void loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals, float* TextureCoordinate);
   void unload();
   
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
   UIFont* fFont;
   char sText[128];
   NSTextAlignment tAligment;
   
   unsigned int iWidth;
   unsigned int iHeight;
   
   float fMaxS;
   float fMaxT;
    
   float* fVertex;
   float* fTextureCoordinate;
   
   NSTextAlignment convertAlignment(GEAlignment iAlignment);
    
   void fillTexture();
   
public:
   GELabelES20(UIFont* Font, const char* Text, GEAlignment TextAligment,
               unsigned int Width, unsigned int Height);
   ~GELabelES20();

   void getModelMatrix(GEMatrix4* ModelMatrix);
    
   void render();
   
   void setText(const char* Text);
};
