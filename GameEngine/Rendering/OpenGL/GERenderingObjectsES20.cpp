
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda PŽrez
//  iOS Game Engine
//
//  Rendering Engine (OpenGL ES)
//
//  --- GERenderingObjectsES20.mm ---
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
    GLKMatrix4 glModelMatrix;
    glModelMatrix = GLKMatrix4MakeTranslation(vPosition.X, vPosition.Y, vPosition.Z);
    glModelMatrix = GLKMatrix4RotateX(glModelMatrix, vRotation.X);
    glModelMatrix = GLKMatrix4RotateY(glModelMatrix, vRotation.Y);
    glModelMatrix = GLKMatrix4RotateZ(glModelMatrix, vRotation.Z);
    glModelMatrix = GLKMatrix4Scale(glModelMatrix, vScale.X, vScale.Y, vScale.Z);
    memcpy(ModelMatrix, &glModelMatrix, sizeof(GEMatrix4));
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
   glBindVertexArrayOES(0);
}

GESpriteES20::~GESpriteES20()
{
}

void GESpriteES20::getModelMatrix(GEMatrix4* ModelMatrix)
{
    GLKMatrix4 glModelMatrix;
    glModelMatrix = GLKMatrix4MakeTranslation(vPosition.X, vPosition.Y, vPosition.Z);
    glModelMatrix = GLKMatrix4RotateX(glModelMatrix, vRotation.X);
    glModelMatrix = GLKMatrix4RotateY(glModelMatrix, vRotation.Y);
    glModelMatrix = GLKMatrix4RotateZ(glModelMatrix, vRotation.Z);
    glModelMatrix = GLKMatrix4Scale(glModelMatrix, vScale.X, vScale.Y, vScale.Z);
    memcpy(ModelMatrix, &glModelMatrix, sizeof(GEMatrix4));
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
GELabelES20::GELabelES20(UIFont* Font, const char* Text, GEAlignment TextAligment,
                         unsigned int Width, unsigned int Height)
{
   iNumVertices = 4;
   iVertexStride = (3 + 2) * sizeof(float);
   
   bVisible = true;
   
   memset(&vPosition, 0, sizeof(GEVector3));
   memset(&vRotation, 0, sizeof(GEVector3));
   
   vScale.X = 1.0f;
   vScale.Y = 1.0f;
   vScale.Z = 1.0f;
   
   cColor.set(1.0f, 1.0f, 1.0f);
   
   strcpy(sText, Text);
   fFont = Font;
   iWidth = Width;
   iHeight = Height;   
   tAligment = convertAlignment(TextAligment);
   
   glGenTextures(1, &iTexture);
   fillTexture();
   
   float fAspectRatio = (float)iHeight / iWidth;
   
   fVertexData = new float[iVertexStride * iNumVertices];
   
   // set texture coordinates
   fVertexData[ 3] = 0.0f;  fVertexData[ 4] = fMaxT;
   fVertexData[ 8] = fMaxS; fVertexData[ 9] = fMaxT;
   fVertexData[13] = 0.0f;  fVertexData[14] = 0.0f;
   fVertexData[18] = fMaxS; fVertexData[19] = 0.0f;
   
   // set vertex positions
   fVertexData[1] = -fAspectRatio; fVertexData[2] = 0.0f;
   fVertexData[6] = -fAspectRatio; fVertexData[7] = 0.0f;
   fVertexData[11] = 0.0f; fVertexData[12] = 0.0f;
   fVertexData[16] = 0.0f; fVertexData[17] = 0.0f;
   
   switch(tAligment)
   {  
      // reference point: top left
      case UITextAlignmentLeft:         
         fVertexData[ 0] = 0.0f;
         fVertexData[ 5] = 1.0f;
         fVertexData[10] = 0.0f;
         fVertexData[15] = 1.0f;
         break;
         
      // reference point: top center
      case UITextAlignmentCenter:         
         fVertexData[ 0] = -0.5f;
         fVertexData[ 5] =  0.5f;
         fVertexData[10] = -0.5f;
         fVertexData[15] =  0.5f;
         break;
         
      // reference point: top right
      case UITextAlignmentRight:         
         fVertexData[ 0] = -1.0f;
         fVertexData[ 5] =  0.0f;
         fVertexData[10] = -1.0f;
         fVertexData[15] =  0.0f;
         break;
   }
}

GELabelES20::~GELabelES20()
{
   // release texture
   glDeleteTextures(1, &iTexture);
}

void GELabelES20::fillTexture()
{
   // set width and height to 2-power values
   int i;
   
   if((iWidth != 1) && (iWidth & (iWidth - 1))) 
   {
      i = 1;
      
      while(i < iWidth)
         i *= 2;
      
      iWidth = i;
	}
   	
   if((iHeight != 1) && (iHeight & (iHeight - 1))) 
   {
      i = 1;
      
      while(i < iHeight)
         i *= 2;
      
      iHeight = i;
	}
   
   // create bitmap context
   void* pData = malloc(iHeight * iWidth);
   
   CGColorSpaceRef cgColorSpace = CGColorSpaceCreateDeviceGray();
   CGContextRef cgContext = CGBitmapContextCreate(pData, iWidth, iHeight, 8, iWidth, cgColorSpace, kCGImageAlphaNone);
   CGColorSpaceRelease(cgColorSpace);
	
   // context settings
   CGContextSetGrayFillColor(cgContext, 1.0, 1.0);
   CGContextTranslateCTM(cgContext, 0.0, iHeight);
   CGContextScaleCTM(cgContext, 1.0, -1.0);
   
   // generate text
   CGSize cgSize = CGSizeMake(iWidth, iHeight);
    
   UIGraphicsPushContext(cgContext);
   NSString* nsText = [NSString stringWithUTF8String:sText];
   [nsText drawInRect:CGRectMake(0, 0, cgSize.width, cgSize.height) withFont:fFont
           lineBreakMode:UILineBreakModeWordWrap alignment:tAligment];
   UIGraphicsPopContext();
   
   fMaxS = cgSize.width / iWidth;
   fMaxT = cgSize.height / iHeight;
	
   // generate texture
   GLint iSaveName;
   glGetIntegerv(GL_TEXTURE_BINDING_2D, &iSaveName);    
   glBindTexture(GL_TEXTURE_2D, iTexture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, iWidth, iHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pData);
   glBindTexture(GL_TEXTURE_2D, iSaveName);
	
   // release context and data
   CGContextRelease(cgContext);
   free(pData);
}

NSTextAlignment GELabelES20::convertAlignment(GEAlignment iAlignment)
{
   switch(iAlignment)
   {
      case TopLeft:
      case CenterLeft:
      case BottomLeft:
         return UITextAlignmentLeft;
      case TopCenter:
      case CenterCenter:
      case BottomCenter:
         return UITextAlignmentCenter;
      case TopRight:
      case CenterRight:
      case BottomRight:
         return UITextAlignmentRight;
   }
}

void GELabelES20::getModelMatrix(GEMatrix4* ModelMatrix)
{
    GLKMatrix4 glModelMatrix;
    glModelMatrix = GLKMatrix4MakeTranslation(vPosition.X, vPosition.Y, vPosition.Z);
    glModelMatrix = GLKMatrix4RotateX(glModelMatrix, vRotation.X);
    glModelMatrix = GLKMatrix4RotateY(glModelMatrix, vRotation.Y);
    glModelMatrix = GLKMatrix4RotateZ(glModelMatrix, vRotation.Z);
    glModelMatrix = GLKMatrix4Scale(glModelMatrix, vScale.X, vScale.Y, vScale.Z);
    memcpy(ModelMatrix, &glModelMatrix, sizeof(GEMatrix4));
}

void GELabelES20::render()
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

void GELabelES20::setText(const char* Text)
{
   strcpy(sText, Text);
   fillTexture();
}
