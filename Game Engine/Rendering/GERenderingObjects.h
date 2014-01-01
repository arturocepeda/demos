
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering
//
//  --- GERenderingObjects.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GETypes.h"
#include "GEGraphicsDevice.h"

class GERenderingObject
{
protected:
    GEVector3 vPosition;
    GEVector3 vScale;    
    GEVector3 vCenter;
    GEQuaternion qRotation;

    byte iOpacity;
    GEColor cColor;

    bool bVisible;

public:
    virtual ~GERenderingObject();

    void move(float DX, float DY, float DZ);
    void move(const GEVector3& Move);
    void scale(float SX, float SY, float SZ);
    void scale(const GEVector3& Scale);
    void rotate(float RX, float RY, float RZ);
    void rotate(const GEVector3& Rotate);
    void show();
    void hide();

    virtual void render() = 0;

    void getPosition(GEVector3* Position);
    void getScale(GEVector3* Scale);
    void getColor(GEColor* Color);
    byte getOpacity();
    bool getVisible();

    void setPosition(float X, float Y, float Z);
    void setPosition(const GEVector3& Position);
    void setScale(float X, float Y, float Z);
    void setScale(const GEVector3& Scale);
    void setRotation(float X, float Y, float Z);
    void setRotation(const GEVector3& Rotation);
    void setColor(const GEColor& Color);
    void setOpacity(byte Opacity);
    void setVisible(bool Visible);
};

class GEMesh : public GERenderingObject
{
public:
    virtual void loadFromFile(const char* Filename) = 0;
    virtual void loadFromFile(const char* Filename, const char* TexturesPath) = 0;
    virtual void loadFromMemory(void* Data, unsigned int SizeOfData) = 0;
    virtual void unload() = 0;
};


class GESprite : public GERenderingObject
{
public:
    virtual void setCenter(float X, float Y, float Z) = 0;

    virtual void loadFromFile(const char* Filename) = 0;
    virtual void unload() = 0;
};


class GECamera
{
protected:
    // camera vectors
    GEVector3 vEye;
    GEVector3 vLookAt;
    GEVector3 vUp;
    GEVector3 vRight;

public:
    GECamera();
    virtual ~GECamera();

    void getPosition(GEVector3* Position);
    void getLookAt(GEVector3* LookAt);

    void setPosition(float X, float Y, float Z);
    void setPosition(const GEVector3& Position);

    void move(float X, float Y, float Z);
    void move(const GEVector3& Move);
    void lookAt(float X, float Y, float Z);
    void lookAt(const GEVector3& LookAt);
    void orbit(const GEVector3& ReferencePoint, float Distance, float Theta, float Phi);

    virtual void use() = 0;
};
