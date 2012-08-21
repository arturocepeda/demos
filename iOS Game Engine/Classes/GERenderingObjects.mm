
/*
   Arturo Cepeda PŽrez

	Rendering Engine (OpenGL)

   --- GERenderingObjects.mm ---
*/

#include "GERenderingObjects.h"


//
//  GERenderingObject
//
void GERenderingObject::move(float DX, float DY, float DZ)
{
   vPosition.X += DX;
   vPosition.Y += DY;
   vPosition.Z += DZ;
}

void GERenderingObject::move(const GEVector& Move)
{
   vPosition.X += Move.X;
   vPosition.Y += Move.Y;
   vPosition.Z += Move.Z;
}

void GERenderingObject::scale(float SX, float SY, float SZ)
{
   vScale.X *= SX;
   vScale.Y *= SY;
   vScale.Z *= SZ;
}

void GERenderingObject::scale(const GEVector& Scale)
{
   vScale.X *= Scale.X;
   vScale.Y *= Scale.Y;
   vScale.Z *= Scale.Z;
}

void GERenderingObject::rotate(float RX, float RY, float RZ)
{
   vRotation.X += RX;
   vRotation.Y += RY;
   vRotation.Z += RZ;
}

void GERenderingObject::rotate(const GEVector& Rotate)
{
   vRotation.X += Rotate.X;
   vRotation.Y += Rotate.Y;
   vRotation.Z += Rotate.Z;
}

void GERenderingObject::show()
{
   bVisible = true;
}

void GERenderingObject::hide()
{
   bVisible = false;
}

void GERenderingObject::getPosition(GEVector* Position)
{
   Position->X = vPosition.X;
   Position->Y = vPosition.Y;
   Position->Z = vPosition.Z;
}

void GERenderingObject::getRotation(GEVector* Rotation)
{
   Rotation->X = vRotation.X;
   Rotation->Y = vRotation.Y;
   Rotation->Z = vRotation.Z;
}

void GERenderingObject::getScale(GEVector* Scale)
{
   Scale->X = vScale.X;
   Scale->Y = vScale.Y;
   Scale->Z = vScale.Z;
}

float GERenderingObject::getOpacity()
{
   return fOpacity;
}

bool GERenderingObject::getVisible()
{
    return bVisible;
}

void GERenderingObject::setPosition(float X, float Y, float Z)
{
   vPosition.X = X;
   vPosition.Y = Y;
   vPosition.Z = Z;
}

void GERenderingObject::setPosition(const GEVector& Position)
{
   vPosition.X = Position.X;
   vPosition.Y = Position.Y;
   vPosition.Z = Position.Z;
}

void GERenderingObject::setScale(float X, float Y, float Z)
{
   vScale.X = X;
   vScale.Y = Y;
   vScale.Z = Z;
}

void GERenderingObject::setScale(const GEVector& Scale)
{
   vScale.X = Scale.X;
   vScale.Y = Scale.Y;
   vScale.Z = Scale.Z;
}

void GERenderingObject::setRotation(float X, float Y, float Z)
{
   vRotation.X = X;
   vRotation.Y = Y;
   vRotation.Z = Z;
}

void GERenderingObject::setRotation(const GEVector& Rotation)
{
   vRotation.X = Rotation.X;
   vRotation.Y = Rotation.Y;
   vRotation.Z = Rotation.Z;
}

void GERenderingObject::setColor(float R, float G, float B)
{
   cColor.R = R;
   cColor.G = G;
   cColor.B = B;
}

void GERenderingObject::setColor(const GEColor& Color)
{
   cColor = Color;
}

void GERenderingObject::setOpacity(float Opacity)
{
   fOpacity = Opacity;
}

void GERenderingObject::setVisible(bool Visible)
{
   bVisible = Visible;
}



//
//  GEMesh
//
GEMesh::GEMesh()
{   
   bVisible = true;
   
   iNumVertices = 0;
   fVertex = NULL;
   fNormals = NULL;
   tTexture = 0;
   fTextureCoordinate = NULL;

   memset(&vPosition, 0, sizeof(GEVector));
   memset(&vCenter, 0, sizeof(GEVector));
   memset(&vRotation, 0, sizeof(GEVector));

   vScale.X = 1.0f;
   vScale.Y = 1.0f;
   vScale.Z = 1.0f;
   
   cColor.set(1.0f, 1.0f, 1.0f);
   fOpacity = 1.0f;
}

GEMesh::~GEMesh()
{
}

void GEMesh::loadFromHeader(unsigned int NumVertices, float *Vertex, float *Normals)
{
   iNumVertices = NumVertices;
   fVertex = Vertex;
   fNormals = Normals;
   tTexture = 0;
   fTextureCoordinate = NULL;
}

void GEMesh::loadFromHeader(unsigned int NumVertices, float* Vertex, float* Normals, 
                            GLuint Texture, float* TextureCoordinate)
{
   iNumVertices = NumVertices;
   fVertex = Vertex;
   fNormals = Normals;
   tTexture = Texture;
   fTextureCoordinate = TextureCoordinate;
}

void GEMesh::unload()
{
   iNumVertices = 0;
   tTexture = 0;
   
   delete[] fVertex;
   delete[] fNormals;
   delete[] fTextureCoordinate;
}

void GEMesh::render()
{
    if(!bVisible)
        return;
  
   glMatrixMode(GL_MODELVIEW);
   
   glBindTexture(GL_TEXTURE_2D, tTexture);   
   glPushMatrix();
      
   glTranslatef(vPosition.X, vPosition.Y, vPosition.Z); 
   glScalef(vScale.X, vScale.Y, vScale.Z);
   glRotatef(vRotation.X, 1.0f, 0.0f, 0.0f); 
   glRotatef(vRotation.Y, 0.0f, 1.0f, 0.0f);
   glRotatef(vRotation.Z, 0.0f, 0.0f, 1.0f);
      
   glVertexPointer(3, GL_FLOAT, 0, fVertex);
   glNormalPointer(GL_FLOAT, 0, fNormals);
   
   if(fTextureCoordinate)
      glTexCoordPointer(2, GL_FLOAT, 0, fTextureCoordinate);
   
   glColor4f(cColor.R, cColor.G, cColor.B, fOpacity);   
   
   glDrawArrays(GL_TRIANGLES, 0, iNumVertices);      
   glPopMatrix();
}



//
//  GESprite
//
GESprite::GESprite(GLuint Texture, const GETextureSize& TextureSize)
{
   tTexture = Texture;   
   fOpacity = 1.0f;
   bVisible = true;

   memset(&vPosition, 0, sizeof(GEVector));
   memset(&vRotation, 0, sizeof(GEVector));
   
   vScale.X = 1.0f;
   vScale.Y = 1.0f;
   vScale.Z = 1.0f;
   
   vCenter.X = TextureSize.Width / 2.0f;
   vCenter.Y = TextureSize.Height / 2.0f;
   vCenter.Z = 0.0f;
}

GESprite::~GESprite()
{
}

void GESprite::render()
{
   if(!bVisible)
      return;
   
	const GLfloat fSpriteVertices[] = {-1, -1, 0,  
                                       1, -1, 0, 
                                      -1,  1, 0,  
                                       1,  1, 0};
   
   const GLfloat fSpriteTextureCoordinates[] = {1, 1,  
                                                1, 0,  
                                                0, 1,  
                                                0, 0};
   
	glVertexPointer(3, GL_FLOAT, 0, fSpriteVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, fSpriteTextureCoordinates);
        
   glBindTexture(GL_TEXTURE_2D, tTexture);
   glPushMatrix();
   
   glTranslatef(vPosition.X, vPosition.Y, vPosition.Z);
   glScalef(vScale.X, vScale.Y, vScale.Z);
   glRotatef(vRotation.X, 1.0f, 0.0f, 0.0f);
   glRotatef(vRotation.Y, 0.0f, 1.0f, 0.0f);
   glRotatef(vRotation.Z, 0.0f, 0.0f, 1.0f);
   glColor4f(1.0f, 1.0f, 1.0f, fOpacity);
   
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);     
   glPopMatrix();
}

void GESprite::setCenter(float X, float Y, float Z)
{
    vCenter.X = X;
    vCenter.Y = Y;
    vCenter.Z = Z;
}


//
//  GELabel
//
GELabel::GELabel(NSString* Text, NSString* FontName, float FontSize, UITextAlignment TextAligment,
                 unsigned int TextureSize)
{
   bVisible = true;
   
   memset(&vPosition, 0, sizeof(GEVector));
   memset(&vRotation, 0, sizeof(GEVector));
   
   vScale.X = 1.0f;
   vScale.Y = 1.0f;
   vScale.Z = 1.0f;
   
   cColor.set(1.0f, 1.0f, 1.0f);
   fOpacity = 1.0f;
   
   tAligment = TextAligment;
   tTexture = [[Texture2D alloc] initWithString:Text
                                     dimensions:CGSizeMake(TextureSize, TextureSize)
                                      alignment:tAligment
                                       fontName:FontName
                                       fontSize:FontSize];
}

GELabel::~GELabel()
{
   [tTexture release];
}

void GELabel::render()
{
   if(!bVisible)
      return;
   
   glPushMatrix();

   glScalef(vScale.X, vScale.Y, vScale.Z);
   glRotatef(vRotation.X, 1.0f, 0.0f, 0.0f);
   glRotatef(vRotation.Y, 0.0f, 1.0f, 0.0f);
   glRotatef(vRotation.Z, 0.0f, 0.0f, 1.0f);
   
   glColor4f(cColor.R, cColor.G, cColor.B, fOpacity);      
   [tTexture drawInRect:CGRectMake(vPosition.X, vPosition.Y, 1.0f, 1.0f)];
   
   glPopMatrix();
}



//
//  GECamera
//
GECamera::GECamera()
{
   memset(&vEye, 0, sizeof(GEVector));
   memset(&vAngle, 0, sizeof(GEVector));
}

GECamera::~GECamera()
{
}

void GECamera::move(float DX, float DY, float DZ)
{
   vEye.X += DX;
   vEye.Y += DY;
   vEye.Z += DZ;
}

void GECamera::move(const GEVector& Move)
{
   vEye.X += Move.X;
   vEye.Y += Move.Y;
   vEye.Z += Move.Z;
}

void GECamera::setPosition(float X, float Y, float Z)
{
   vEye.X = X;
   vEye.Y = Y;
   vEye.Z = Z;
}

void GECamera::setPosition(const GEVector& Position)
{
   vEye.X = Position.X;
   vEye.Y = Position.Y;
   vEye.Z = Position.Z;
}

void GECamera::setAngle(float X, float Y, float Z)
{
   vAngle.X = X;
   vAngle.Y = Y;
   vAngle.Z = Z;
}

void GECamera::setAngle(const GEVector& Angle)
{
   vAngle.X = Angle.X;
   vAngle.Y = Angle.Y;
   vAngle.Z = Angle.Z;
}

void GECamera::use()
{   
   glMatrixMode(GL_PROJECTION);
   
   glLoadIdentity();
   glOrthof(CDevice::getOrthoLeft(), CDevice::getOrthoRight(), 
            CDevice::getOrthoBottom(), CDevice::getOrthoTop(), 
            GL_ORTHO_ZNEAR, GL_ORTHO_ZFAR);
    
   glTranslatef(vEye.X, vEye.Y, vEye.Z);    
   glRotatef(vAngle.X, 1.0f, 0.0f, 0.0f);
   glRotatef(vAngle.Y, 0.0f, 1.0f, 0.0f);
   glRotatef(vAngle.Z, 0.0f, 0.0f, 1.0f);
}
