
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  iOS Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingObjects.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GETypes.h"

#include <GLKit/GLKit.h>

#define ZNEAR     0.1f
#define ZFAR      1000.0f

#define PI        3.14159f
#define DEG2RAD   57.2958f    // (180/PI)


//
//  GELight
//
struct GELight
{
   GEVector3 Position;
   GEColor Color;
   float Intensity;
};


//
//  GETextureSize
//
struct GETextureSize
{
   unsigned int Width;
   unsigned int Height;
};


//
//  Set of lights
//
struct
{
   enum
   {
      PointLight1,
      
      Count
   };
} 
GELights;


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
//  Shader programs
//
struct 
{
   enum
   {
      HUD,
      Text,
      MeshColor,
      MeshTexture,
      
      Count
   };
}
GEPrograms;


//
//  GERenderingObject
//
class GERenderingObject
{
protected:
   GEVector3 vPosition;
   GEVector3 vRotation;
   GEVector3 vScale;
   
public:
   void move(float DX, float DY, float DZ = 0.0f);
   void move(const GEVector3& Move);
   void scale(float SX, float SY, float SZ = 1.0f);
   void scale(const GEVector3& Scale);
   void rotate(float RX, float RY, float RZ);
   void rotate(const GEVector3& Rotate);

   void getPosition(GEVector3* Position);
   void getRotation(GEVector3* Rotation);
   void getScale(GEVector3* Scale);

   void setPosition(float X, float Y, float Z = 0.0f);
   void setPosition(const GEVector3& Position);
   void setScale(float X, float Y, float Z = 1.0f);
   void setScale(const GEVector3& Scale);
   void setRotation(float X, float Y, float Z);
   void setRotation(const GEVector3& Rotation);
};


//
//  GECamera
//
class GECamera : public GERenderingObject
{
private:
   GEVector3 vLookAt;
   
public:
   GECamera();
   ~GECamera();
   
   void lookAt(float X, float Y, float Z);
   void lookAt(GEVector3& LookAt);
   
   void getLookAt(GEVector3* LookAt);
};


//
//  GERenderingObjectVisible
//
class GERenderingObjectVisible : public GERenderingObject
{
protected:
   unsigned int iNumVertices;
   
   float fOpacity;
   bool bVisible;
   GEColor cColor;
   GLuint iTexture;
   
   // vertex attributes pointers
   float* fVertex;
   float* fNormals;
   float* fTextureCoordinate;
   
   // vertex array and vertex buffers
   GLuint iVertexArray;
   GLuint iVertexBuffers[GEVertexAttributes.Count];
   
public:
   void show();
   void hide();
   
   virtual void render() = 0;
   
   void getModelMatrix(GLKMatrix4* ModelMatrix);
   void getColor(GEColor* Color);
   float getOpacity();
   bool getVisible();
   GLuint getTexture();
      
   void setColor(float R, float G, float B);
   void setColor(const GEColor& Color);
   void setOpacity(float Opacity);
   void setVisible(bool Visible);
   void setTexture(GLuint Texture);
};


//
//  GEMesh
//
class GEMesh : public GERenderingObjectVisible
{
public:
   GEMesh();
   ~GEMesh();

   void loadFromHeader(unsigned int NumVertices, float* Vertex, float* Normals);
   void loadFromHeader(unsigned int NumVertices, float* Vertex, float* Normals, float* TextureCoordinate);
   void unload();
   
   void render();
};


//
//  GESprite
//
class GESprite : public GERenderingObjectVisible
{
public:
   GESprite();
   ~GESprite();

   void loadFromFile(const char* Filename);
   void unload();

   void render();
   
   void setCenter(float X, float Y, float Z);
   void setTextureCoordinates(float Ax, float Ay, float Bx, float By,
                              float Cx, float Cy, float Dx, float Dy);
};


//
//  GELabel
//
class GELabel : public GERenderingObjectVisible
{
private:
   NSString* sText;
   NSString* sFont;
   UITextAlignment tAligment;
   float fFontSize;
   
   unsigned int iWidth;
   unsigned int iHeight;
   
   float fMaxS;
   float fMaxT;
   
   void createTexture();
   void releaseTexture();
   
public:
   GELabel(NSString* Text, NSString* FontName, float FontSize, UITextAlignment TextAligment,
           unsigned int Width, unsigned int Height);
   ~GELabel();
   
   void render();
   
   void setText(NSString* Text);
};
