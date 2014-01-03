
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering
//
//  --- GERenderingObjects.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GERenderingObjects.h"
#include <stdio.h>


//
//  GERenderingObject
//
GERenderingObject::~GERenderingObject()
{
}

void GERenderingObject::move(float DX, float DY, float DZ)
{
    vPosition.X += DX;
    vPosition.Y += DY;
    vPosition.Z += DZ;
}

void GERenderingObject::move(const GEVector3& Move)
{
    vPosition += Move;
}

void GERenderingObject::rotate(float RX, float RY, float RZ)
{
    vRotation.X += RX;
    vRotation.Y += RY;
    vRotation.Z += RZ;
    //D3DXQUATERNION qAux;
    //D3DXQuaternionRotationYawPitchRoll(&qAux, RY, RX, RZ);
    //qRotation = qAux * qRotation;
}

void GERenderingObject::rotate(const GEVector3& Rotate)
{
    vRotation += Rotate;
    //D3DXQUATERNION qAux;
    //D3DXQuaternionRotationYawPitchRoll(&qAux, Rotate.Y, Rotate.X, Rotate.Z);
    //qRotation = qAux * qRotation;
}

GEVector3& GERenderingObject::getPosition()
{
    return vPosition;
}

GEVector3& GERenderingObject::getRotation()
{
    return vRotation;
}

void GERenderingObject::setPosition(float X, float Y, float Z)
{
    vPosition.X = X;
    vPosition.Y = Y;
    vPosition.Z = Z;
}

void GERenderingObject::setPosition(const GEVector3& Position)
{
    vPosition = Position;
}

void GERenderingObject::setRotation(float X, float Y, float Z)
{
    vRotation = GEVector3(X, Y, Z);
    //D3DXQuaternionRotationYawPitchRoll(&qRotation, Y, X, Z);
}

void GERenderingObject::setRotation(const GEVector3& Rotation)
{
    vRotation = Rotation;
    //D3DXQuaternionRotationYawPitchRoll(&qRotation, Rotation.Y, Rotation.X, Rotation.Z);
}


//
//  GERenderingObjectVisible
//
void GERenderingObjectVisible::scale(float SX, float SY, float SZ)
{
    vScale.X *= SX;
    vScale.Y *= SY;
    vScale.Z *= SZ;
}

void GERenderingObjectVisible::scale(const GEVector3& Scale)
{
    vScale.X *= Scale.X;
    vScale.Y *= Scale.Y;
    vScale.Z *= Scale.Z;
}

void GERenderingObjectVisible::show()
{
    bVisible = true;
}

void GERenderingObjectVisible::hide()
{
    bVisible = false;
}

GEVector3& GERenderingObjectVisible::getScale()
{
    return vScale;
}

GEColor& GERenderingObjectVisible::getColor()
{
    return cColor;
}

unsigned int GERenderingObjectVisible::getTexture()
{
    return iTexture;
}

float GERenderingObjectVisible::getOpacity()
{
    return cColor.A;
}

bool GERenderingObjectVisible::getVisible()
{
    return bVisible;
}

void GERenderingObjectVisible::getModelMatrix(GEMatrix4* ModelMatrix)
{
}

void GERenderingObjectVisible::setScale(float X, float Y, float Z)
{
    vScale.X = X;
    vScale.Y = Y;
    vScale.Z = Z;
}

void GERenderingObjectVisible::setScale(const GEVector3& Scale)
{
    vScale = Scale;
}

void GERenderingObjectVisible::setColor(const GEColor& Color)
{
    cColor = Color;
}

void GERenderingObjectVisible::setTexture(unsigned int Texture)
{
    iTexture = Texture;
}

void GERenderingObjectVisible::setOpacity(float Opacity)
{
    cColor.A = Opacity;
}

void GERenderingObjectVisible::setVisible(bool Visible)
{
    bVisible = Visible;
}


//
//  GEMesh
//
void GEMesh::loadFromFile(const char* Filename)
{
}

void GEMesh::loadFromFile(const char* Filename, const char* TexturesPath)
{
}

void GEMesh::loadFromMemory(void* Data, unsigned int SizeOfData)
{
}

void GEMesh::loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals)
{
}

void GEMesh::loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals, float* TextureCoordinate)
{
}

void GEMesh::unload()
{
}


//
//  GESprite
//
void GESprite::setCenter(const GEVector3& Center)
{
    vCenter = Center;
}

void GESprite::loadFromFile(const char* Filename)
{
}

void GESprite::unload()
{
}


//
//  GELabel
//
void GELabel::setFont(unsigned int Font)
{
    iFont = Font;
}

void GELabel::setAligment(GEAlignment Alignment)
{
    iAlignment = Alignment;
}


//
//  GECamera
//
GECamera::GECamera()
{
    // eye vector
    vPosition = GEVector3(0.0f, 0.0f, 0.0f);

    // lookat vector
    vLookAt = GEVector3(0.0f, 0.0f, 0.0f);

    // up vector
    vUp = GEVector3(0.0f, 1.0f, 0.0f);

    // right vector
    vRight = GEVector3(1.0f, 0.0f, 0.0f);
}

GEVector3& GECamera::getLookAt()
{
    return vLookAt;
}

void GECamera::lookAt(float x, float y, float z)
{
    vLookAt.X = x;
    vLookAt.Y = y;
    vLookAt.Z = z;
}

void GECamera::lookAt(const GEVector3& LookAt)
{
    vLookAt = LookAt;
}

void GECamera::orbit(const GEVector3& ReferencePoint, float Distance, float Theta, float Phi)
{
    setPosition(ReferencePoint.X + (Distance * cosf(Theta) * sinf(Phi)),
                ReferencePoint.Y + (Distance * -cosf(Phi)), 
                ReferencePoint.Z + (Distance * sinf(Theta) * sinf(Phi)));
    lookAt(ReferencePoint);
}

void GECamera::use()
{
}
