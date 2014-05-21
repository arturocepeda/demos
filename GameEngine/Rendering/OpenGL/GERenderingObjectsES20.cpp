
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingObjectsES20.cpp ---
//
//////////////////////////////////////////////////////////////////


#include "GERenderingObjectsES20.h"


//
//  GEOpenGLElement
//
GEOpenGLElement::GEOpenGLElement()
   : fVertexData(NULL)
{
}

GEOpenGLElement::~GEOpenGLElement()
{
   if(fVertexData)
      delete[] fVertexData;
}


//
//  GEMeshES20
//
GEMeshES20::GEMeshES20()
   : bUseTexture(false)
{
   bVisible = true;
   iNumVertices = 0;

   memset(&vPosition, 0, sizeof(GEVector3));
   memset(&vRotation, 0, sizeof(GEVector3));
   vScale.set(1.0f, 1.0f, 1.0f);
   cColor.A = 1.0f;
   iTexture = 0;
}

GEMeshES20::~GEMeshES20()
{
}

void GEMeshES20::getModelMatrix(GEMatrix4* ModelMatrix)
{
   GEMatrix4MakeTranslation(vPosition, ModelMatrix);
   GEMatrix4RotateX(ModelMatrix, vRotation.X);
   GEMatrix4RotateY(ModelMatrix, vRotation.Y);
   GEMatrix4RotateZ(ModelMatrix, vRotation.Z);
   GEMatrix4Scale(ModelMatrix, vScale);
}

void GEMeshES20::loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals)
{
   unload();
   
   iNumVertices = NumVertices;
   iVertexStride = (3 + 3) * sizeof(float);
   fVertexData = new float[iNumVertices * iVertexStride];
   bUseTexture = false;
   
   float* pCurrentVertex = fVertexData;
   float* pCurrentPosition = Vertex;
   float* pCurrentNormal = Normals;
   
   for(unsigned int i = 0; i < iNumVertices; i++)
   {
      memcpy(pCurrentVertex, pCurrentPosition, 3 * sizeof(float));
      pCurrentVertex += 3;
      pCurrentPosition += 3;
      
      memcpy(pCurrentVertex, pCurrentNormal, 3 * sizeof(float));
      pCurrentVertex += 3;
      pCurrentNormal += 3;
   }
}

void GEMeshES20::loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals, float* TextureCoordinate)
{
   unload();
   
   iNumVertices = NumVertices;
   iVertexStride = (3 + 3 + 2) * sizeof(float);
   fVertexData = new float[iNumVertices * iVertexStride];
   bUseTexture = true;
   
   float* pCurrentVertex = fVertexData;
   float* pCurrentPosition = Vertex;
   float* pCurrentNormal = Normals;
   float* pCurrentTextureCoordinate = TextureCoordinate;
   
   for(unsigned int i = 0; i < iNumVertices; i++)
   {
      memcpy(pCurrentVertex, pCurrentPosition, 3 * sizeof(float));
      pCurrentVertex += 3;
      pCurrentPosition += 3;
      
      memcpy(pCurrentVertex, pCurrentNormal, 3 * sizeof(float));
      pCurrentVertex += 3;
      pCurrentNormal += 3;
      
      memcpy(pCurrentVertex, pCurrentTextureCoordinate, 2 * sizeof(float));
      pCurrentVertex += 2;
      pCurrentTextureCoordinate += 2;
   }
}

void GEMeshES20::unload()
{
   iNumVertices = 0;
   
   if(fVertexData)
   {
      delete[] fVertexData;
      fVertexData = NULL;
   }
}

void GEMeshES20::render()
{
   if(!bVisible)
      return;

   // load vertex data into vertex buffer
   glBufferData(GL_ARRAY_BUFFER, iVertexStride * iNumVertices, fVertexData, GL_STATIC_DRAW);
   
   // set vertex declaration
   glVertexAttribPointer(GEVertexAttributes.Position, 3, GL_FLOAT, GL_FALSE, iVertexStride, 0);
   glVertexAttribPointer(GEVertexAttributes.Normal, 3, GL_FLOAT, GL_TRUE, iVertexStride, (void*)12);
   
   if(bUseTexture)
      glVertexAttribPointer(GEVertexAttributes.TextureCoord0, 2, GL_FLOAT, GL_FALSE, iVertexStride, (void*)24);

   // draw
   glDrawArrays(GL_TRIANGLES, 0, iNumVertices); 
}


//
//  GESpriteES20
//
GESpriteES20::GESpriteES20()
{
   iNumVertices = 4;
   iVertexStride = (3 + 2) * sizeof(float);
   
   cColor = GEColor(1.0f, 1.0f, 1.0f);
   bVisible = true;

   memset(&vPosition, 0, sizeof(GEVector3));
   memset(&vRotation, 0, sizeof(GEVector3));
   
   vScale.X = 1.0f;
   vScale.Y = 1.0f;
   vScale.Z = 1.0f;
   
   fVertexData = new float[iVertexStride * iNumVertices];
   
   setCenter(GEVector3(0.0f, 0.0f, 0.0f));
   setTextureCoordinates(1.0f, 1.0f, 
                         1.0f, 0.0f, 
                         0.0f, 1.0f, 
                         0.0f, 0.0f);
   
   // unbind buffers and vertex array object
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GESpriteES20::~GESpriteES20()
{
}

void GESpriteES20::getModelMatrix(GEMatrix4* ModelMatrix)
{
   GEMatrix4MakeTranslation(vPosition, ModelMatrix);
   GEMatrix4RotateX(ModelMatrix, vRotation.X);
   GEMatrix4RotateY(ModelMatrix, vRotation.Y);
   GEMatrix4RotateZ(ModelMatrix, vRotation.Z);
   GEMatrix4Scale(ModelMatrix, vScale);
}

void GESpriteES20::render()
{
   if(!bVisible)
      return;

   // load vertex data into vertex buffer
   glBufferData(GL_ARRAY_BUFFER, iVertexStride * iNumVertices, fVertexData, GL_STATIC_DRAW);
   
   // set vertex declaration   
   glVertexAttribPointer(GEVertexAttributes.Position, 3, GL_FLOAT, GL_FALSE, iVertexStride, 0);
   glVertexAttribPointer(GEVertexAttributes.TextureCoord0, 2, GL_FLOAT, GL_FALSE, iVertexStride, (void*)12);
   
   // draw
   glDrawArrays(GL_TRIANGLE_STRIP, 0, iNumVertices);
}

void GESpriteES20::setCenter(const GEVector3& Center)
{
   fVertexData[ 0] = -1 + Center.X; fVertexData[ 1] = -1 + Center.Y; fVertexData[ 2] = Center.Z;
   fVertexData[ 5] =  1 + Center.X; fVertexData[ 6] = -1 + Center.Y; fVertexData[ 7] = Center.Z;
   fVertexData[10] = -1 + Center.X; fVertexData[11] =  1 + Center.Y; fVertexData[12] = Center.Z;
   fVertexData[15] =  1 + Center.X; fVertexData[16] =  1 + Center.Y; fVertexData[17] = Center.Z;
}

void GESpriteES20::setTextureCoordinates(float Ax, float Ay, float Bx, float By,
                                         float Cx, float Cy, float Dx, float Dy)
{
   fVertexData[ 3] = Ax; fVertexData[ 4] = Ay;
   fVertexData[ 8] = Bx; fVertexData[ 9] = By;
   fVertexData[13] = Cx; fVertexData[14] = Cy;
   fVertexData[18] = Dx; fVertexData[19] = Dy;
}



//
//  GELabelES20
//
GELabelES20::GELabelES20(CTFont* Font, const char* Text, GEAlignment TextAligment,
                         unsigned int Width, unsigned int Height)
{
}

GELabelES20::~GELabelES20()
{
}

void GELabelES20::fillTexture()
{
}

unsigned int GELabelES20::convertAlignment(GEAlignment iAlignment)
{
   return 0;
}

void GELabelES20::getModelMatrix(GEMatrix4* ModelMatrix)
{
}

void GELabelES20::render()
{
}

void GELabelES20::setText(const char* Text)
{
   strcpy(sText, Text);
   fillTexture();
}
