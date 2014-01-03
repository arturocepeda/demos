
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
//  GEMeshES20
//
GEMeshES20::GEMeshES20()
{   
   bVisible = true;
   
   iNumVertices = 0;
   fVertex = NULL;
   fNormals = NULL;
   fTextureCoordinate = NULL;

   memset(&vPosition, 0, sizeof(GEVector3));
   memset(&vRotation, 0, sizeof(GEVector3));
   vScale.set(1.0f, 1.0f, 1.0f);
   cColor.A = 1.0f;
   iTexture = 0;

   // generate vertex array
   glGenVertexArraysOES(1, &iVertexArray);
   glBindVertexArrayOES(iVertexArray);
   
   // generate vertex buffers
   glGenBuffers(GEVertexAttributes.Count, iVertexBuffers);
}

GEMeshES20::~GEMeshES20()
{
   // release vertex array and vertex buffers
   glDeleteBuffers(GEVertexAttributes.Count, iVertexBuffers);
   glDeleteVertexArraysOES(1, &iVertexArray);
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
   iNumVertices = NumVertices;
   fVertex = Vertex;
   fNormals = Normals;
   fTextureCoordinate = NULL;
   
   // bind vertex array object
   glBindVertexArrayOES(iVertexArray);
   
   // fill vertex array buffers
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.Position]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * iNumVertices, fVertex, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.Position);
   glVertexAttribPointer(GEVertexAttributes.Position, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.Normal]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * iNumVertices, fNormals, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.Normal);
   glVertexAttribPointer(GEVertexAttributes.Normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // unbind buffers and vertex array object
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArrayOES(0);
}

void GEMeshES20::loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals, float* TextureCoordinate)
{
   iNumVertices = NumVertices;
   fVertex = Vertex;
   fNormals = Normals;
   fTextureCoordinate = TextureCoordinate;
   
   // bind vertex array object
   glBindVertexArrayOES(iVertexArray);
   
   // fill vertex array buffers
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.Position]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * iNumVertices, fVertex, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.Position);
   glVertexAttribPointer(GEVertexAttributes.Position, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.Normal]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * iNumVertices, fNormals, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.Normal);
   glVertexAttribPointer(GEVertexAttributes.Normal, 3, GL_FLOAT, GL_FALSE, 0, 0);   
   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.TextureCoord0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * iNumVertices, fTextureCoordinate, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.TextureCoord0);
   glVertexAttribPointer(GEVertexAttributes.TextureCoord0, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
   // unbind buffers and vertex array object
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArrayOES(0);
}

void GEMeshES20::unload()
{
   iNumVertices = 0;
   
   fVertex = NULL;
   fNormals = NULL;
   fTextureCoordinate = NULL;
}

void GEMeshES20::render()
{
   if(!bVisible)
      return;
   
   // bind vertex array object
   glBindVertexArrayOES(iVertexArray);

   // draw
   glDrawArrays(GL_TRIANGLES, 0, iNumVertices); 
}


//
//  GESpriteES20
//
GESpriteES20::GESpriteES20()
{
   iNumVertices = 4;
   
   cColor = GEColor(1.0f, 1.0f, 1.0f);
   bVisible = true;

   memset(&vPosition, 0, sizeof(GEVector3));
   memset(&vRotation, 0, sizeof(GEVector3));
   
   vScale.X = 1.0f;
   vScale.Y = 1.0f;
   vScale.Z = 1.0f;
   
   fVertex = new float[iNumVertices * 3];
   fTextureCoordinate = new float[iNumVertices * 2];
   
   setCenter(GEVector3(0.0f, 0.0f, 0.0f));
   setTextureCoordinates(1.0f, 1.0f, 
                         1.0f, 0.0f, 
                         0.0f, 1.0f, 
                         0.0f, 0.0f);
   
   // generate vertex array
   glGenVertexArraysOES(1, &iVertexArray);
   glBindVertexArrayOES(iVertexArray);
   
   // generate vertex buffers
   glGenBuffers(2, iVertexBuffers);
   
   // fill vertex array buffers
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.Position]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * iNumVertices, fVertex, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.Position);
   glVertexAttribPointer(GEVertexAttributes.Position, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.TextureCoord0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * iNumVertices, fTextureCoordinate, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.TextureCoord0);
   glVertexAttribPointer(GEVertexAttributes.TextureCoord0, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
   glDisableVertexAttribArray(GEVertexAttributes.Normal);
   
   // unbind buffers and vertex array object
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArrayOES(0);
}

GESpriteES20::~GESpriteES20()
{
   // release vertex array and vertex buffers
   glDeleteBuffers(GEVertexAttributes.Count, iVertexBuffers);
   glDeleteVertexArraysOES(1, &iVertexArray);
   
   // release vertex data
   delete[] fVertex;
   delete[] fTextureCoordinate;
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
   
   // bind vertex array object
   glBindVertexArrayOES(iVertexArray);

   // draw
   glDrawArrays(GL_TRIANGLE_STRIP, 0, iNumVertices);
}

void GESpriteES20::setCenter(const GEVector3& Center)
{
   fVertex[0] = -1 + Center.X; fVertex[ 1] = -1 + Center.Y; fVertex[ 2] = Center.Z;
   fVertex[3] =  1 + Center.X; fVertex[ 4] = -1 + Center.Y; fVertex[ 5] = Center.Z;   
   fVertex[6] = -1 + Center.X; fVertex[ 7] =  1 + Center.Y; fVertex[ 8] = Center.Z;   
   fVertex[9] =  1 + Center.X; fVertex[10] =  1 + Center.Y; fVertex[11] = Center.Z;
   
   // update vertex buffer data
   glBindVertexArrayOES(iVertexArray);   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.Position]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * iNumVertices, fVertex);   
}

void GESpriteES20::setTextureCoordinates(float Ax, float Ay, float Bx, float By,
                                         float Cx, float Cy, float Dx, float Dy)
{
   fTextureCoordinate[0] = Ax; fTextureCoordinate[1] = Ay;
   fTextureCoordinate[2] = Bx; fTextureCoordinate[3] = By;
   fTextureCoordinate[4] = Cx; fTextureCoordinate[5] = Cy;
   fTextureCoordinate[6] = Dx; fTextureCoordinate[7] = Dy;
   
   // update vertex buffer data
   glBindVertexArrayOES(iVertexArray);   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.TextureCoord0]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * iNumVertices, fTextureCoordinate);   
}



//
//  GELabelES20
//
GELabelES20::GELabelES20(UIFont* Font, const char* Text, GEAlignment TextAligment,
                         unsigned int Width, unsigned int Height)
{
   iNumVertices = 4;
   
   bVisible = true;
   
   memset(&vPosition, 0, sizeof(GEVector3));
   memset(&vRotation, 0, sizeof(GEVector3));
   
   vScale.X = 1.0f;
   vScale.Y = 1.0f;
   vScale.Z = 1.0f;
   
   cColor.set(1.0f, 1.0f, 1.0f);
   
   fVertex = new float[iNumVertices * 3];
   fTextureCoordinate = new float[iNumVertices * 2];
   
   strcpy(sText, Text);
   fFont = Font;
   iWidth = Width;
   iHeight = Height;   
   tAligment = convertAlignment(TextAligment);
   
   glGenTextures(1, &iTexture);
   fillTexture();
   
   float fAspectRatio = (float)iHeight / iWidth;      
   
   fTextureCoordinate[0] = 0.0f;  fTextureCoordinate[1] = fMaxT;
   fTextureCoordinate[2] = fMaxS; fTextureCoordinate[3] = fMaxT;
   fTextureCoordinate[4] = 0.0f;  fTextureCoordinate[5] = 0.0f;
   fTextureCoordinate[6] = fMaxS; fTextureCoordinate[7] = 0.0f;
   
   switch(tAligment)
   {  
      // reference point: top left
      case UITextAlignmentLeft:         
         fVertex[0] = 0.0f; fVertex[1] = -fAspectRatio; fVertex[2] = 0.0f;
         fVertex[3] = 1.0f; fVertex[4] = -fAspectRatio; fVertex[5] = 0.0f;
         fVertex[6] = 0.0f; fVertex[7] = 0.0f; fVertex[8] = 0.0f;
         fVertex[9] = 1.0f; fVertex[10] = 0.0f; fVertex[11] = 0.0f;
         break;
         
      // reference point: top center
      case UITextAlignmentCenter:         
         fVertex[0] = -0.5f; fVertex[1] = -fAspectRatio; fVertex[2] = 0.0f;
         fVertex[3] = 0.5f; fVertex[4] = -fAspectRatio; fVertex[5] = 0.0f;
         fVertex[6] = -0.5f; fVertex[7] = 0.0f; fVertex[8] = 0.0f;
         fVertex[9] = 0.5f; fVertex[10] = 0.0f; fVertex[11] = 0.0f;        
         break;
         
      // reference point: top right
      case UITextAlignmentRight:         
         fVertex[0] = -1.0f; fVertex[1] = -fAspectRatio; fVertex[2] = 0.0f;
         fVertex[3] = 0.0f; fVertex[4] = -fAspectRatio; fVertex[5] = 0.0f;
         fVertex[6] = -1.0f; fVertex[7] = 0.0f; fVertex[8] = 0.0f;
         fVertex[9] = 0.0f; fVertex[10] = 0.0f; fVertex[11] = 0.0f;         
         break;
   }
   
   // generate vertex array
   glGenVertexArraysOES(1, &iVertexArray);
   glBindVertexArrayOES(iVertexArray);
   
   // generate vertex buffers
   glGenBuffers(2, iVertexBuffers);
   
   // fill vertex array buffers
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.Position]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * iNumVertices, fVertex, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.Position);
   glVertexAttribPointer(GEVertexAttributes.Position, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glBindBuffer(GL_ARRAY_BUFFER, iVertexBuffers[GEVertexAttributes.TextureCoord0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * iNumVertices, fTextureCoordinate, GL_STATIC_DRAW);
   glEnableVertexAttribArray(GEVertexAttributes.TextureCoord0);
   glVertexAttribPointer(GEVertexAttributes.TextureCoord0, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
   glDisableVertexAttribArray(GEVertexAttributes.Normal);
   
   // unbind buffers and vertex array object
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArrayOES(0);
}

GELabelES20::~GELabelES20()
{
   // release vertex array and vertex buffers
   glDeleteBuffers(GEVertexAttributes.Count, iVertexBuffers);
   glDeleteVertexArraysOES(1, &iVertexArray);
      
   // release texture
   glDeleteTextures(1, &iTexture);

   // release vertex data
   delete[] fVertex;
   delete[] fTextureCoordinate;
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
   
   // bind vertex array object
   glBindVertexArrayOES(iVertexArray);

   // draw
   glDrawArrays(GL_TRIANGLE_STRIP, 0, iNumVertices);
}

void GELabelES20::setText(const char* Text)
{
   strcpy(sText, Text);
   fillTexture();
}
