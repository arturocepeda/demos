
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

void GERenderingObject::scale(float SX, float SY, float SZ)
{
    vScale.X *= SX;
    vScale.Y *= SY;
    vScale.Z *= SZ;
}

void GERenderingObject::scale(const GEVector3& Scale)
{
    vScale.X *= Scale.X;
    vScale.Y *= Scale.Y;
    vScale.Z *= Scale.Z;
}

void GERenderingObject::rotate(float RX, float RY, float RZ)
{
    //D3DXQUATERNION qAux;
    //D3DXQuaternionRotationYawPitchRoll(&qAux, RY, RX, RZ);
    //qRotation = qAux * qRotation;
}

void GERenderingObject::rotate(const GEVector3& Rotate)
{
    //D3DXQUATERNION qAux;
    //D3DXQuaternionRotationYawPitchRoll(&qAux, Rotate.Y, Rotate.X, Rotate.Z);
    //qRotation = qAux * qRotation;
}

void GERenderingObject::show()
{
    bVisible = true;
}

void GERenderingObject::hide()
{
    bVisible = false;
}

void GERenderingObject::getPosition(GEVector3* Position)
{
    Position->X = vPosition.X;
    Position->Y = vPosition.Y;
    Position->Z = vPosition.Z;
}

void GERenderingObject::getScale(GEVector3* Scale)
{
    Scale->X = vScale.X;
    Scale->Y = vScale.Y;
    Scale->Z = vScale.Z;
}

void GERenderingObject::getColor(GEColor* Color)
{
    Color->R = cColor.R;
    Color->G = cColor.G;
    Color->B = cColor.B;
    Color->A = cColor.A;
}

unsigned char GERenderingObject::getOpacity()
{
    return iOpacity;
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

void GERenderingObject::setPosition(const GEVector3& Position)
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

void GERenderingObject::setScale(const GEVector3& Scale)
{
    vScale.X = Scale.X;
    vScale.Y = Scale.Y;
    vScale.Z = Scale.Z;
}

void GERenderingObject::setRotation(float X, float Y, float Z)
{
    //D3DXQuaternionRotationYawPitchRoll(&qRotation, Y, X, Z);
}

void GERenderingObject::setRotation(const GEVector3& Rotation)
{
    //D3DXQuaternionRotationYawPitchRoll(&qRotation, Rotation.Y, Rotation.X, Rotation.Z);
}

void GERenderingObject::setColor(const GEColor& Color)
{
    cColor = Color;
}

void GERenderingObject::setOpacity(unsigned char Opacity)
{
    iOpacity = Opacity;
}

void GERenderingObject::setVisible(bool Visible)
{
    bVisible = Visible;
}


//
//  GECamera
//
GECamera::GECamera()
{
    // eye vector
    vEye = GEVector3(0.0f, 0.0f, 0.0f);

    // lookat vector
    vLookAt = GEVector3(0.0f, 0.0f, 0.0f);

    // up vector
    vUp = GEVector3(0.0f, 1.0f, 0.0f);

    // right vector
    vRight = GEVector3(1.0f, 0.0f, 0.0f);
}

GECamera::~GECamera()
{
}

void GECamera::getPosition(GEVector3* Position)
{
    Position->X = vEye.X;
    Position->Y = vEye.Y;
    Position->Z = vEye.Z;
}

void GECamera::getLookAt(GEVector3* LookAt)
{
    LookAt->X = vLookAt.X;
    LookAt->Y = vLookAt.Y;
    LookAt->Z = vLookAt.Z;
}

void GECamera::setPosition(float x, float y, float z)
{
    vEye.X = x;
    vEye.Y = y;
    vEye.Z = z;
}

void GECamera::setPosition(const GEVector3& Position)
{
    vEye.X = Position.X;
    vEye.Y = Position.Y;
    vEye.Z = Position.Z;
}

void GECamera::move(float x, float y, float z)
{
    vEye.X += x;
    vEye.Y += y;
    vEye.Z += z;
}

void GECamera::move(const GEVector3& Move)
{
    vEye += Move;
}

void GECamera::lookAt(float x, float y, float z)
{
    vLookAt.X = x;
    vLookAt.Y = y;
    vLookAt.Z = z;
}

void GECamera::lookAt(const GEVector3& LookAt)
{
    vLookAt.X = LookAt.X;
    vLookAt.Y = LookAt.Y;
    vLookAt.Z = LookAt.Z;
}

void GECamera::orbit(const GEVector3& ReferencePoint, float Distance, float Theta, float Phi)
{
    setPosition(ReferencePoint.X + (Distance * cosf(Theta) * sinf(Phi)),
                ReferencePoint.Y + (Distance * -cosf(Phi)), 
                ReferencePoint.Z + (Distance * sinf(Theta) * sinf(Phi)));
    lookAt(ReferencePoint.X, ReferencePoint.Y, ReferencePoint.Z);
}
