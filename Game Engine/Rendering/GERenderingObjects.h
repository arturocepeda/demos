
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


struct GELight
{
    GEVector3 Position;
    GEVector3 Direction;
    GEColor Color;
    float Intensity;
    float Range;
    bool On;
};


enum GEAlignment
{
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    CenterCenter,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};


class GERenderingObject
{
protected:
    GEVector3 vPosition;
    GEVector3 vRotation;
    GEQuaternion qRotation;
    
public:
    virtual ~GERenderingObject();
    
    void move(float DX, float DY, float DZ = 0.0f);
    void move(const GEVector3& Move);
    void rotate(float RX, float RY, float RZ = 0.0f);
    void rotate(const GEVector3& Rotate);
    
    GEVector3& getPosition();
    GEVector3& getRotation();
    
    void setPosition(float X, float Y, float Z = 0.0f);
    void setPosition(const GEVector3& Position);
    void setRotation(float X, float Y, float Z = 0.0f);
    void setRotation(const GEVector3& Rotation);
};


class GERenderingObjectVisible : public GERenderingObject
{
protected:
    GEVector3 vScale;
    GEVector3 vCenter;
    
    GEColor cColor;
    
    unsigned int iNumVertices;
    unsigned int iTexture;

    bool bVisible;

public:
    void scale(float SX, float SY, float SZ = 1.0f);
    void scale(const GEVector3& Scale);
    void show();
    void hide();

    virtual void render() = 0;
    
    GEVector3& getScale();
    GEColor& getColor();
    float getOpacity();
    unsigned int getTexture();
    bool getVisible();
    virtual void getModelMatrix(GEMatrix4* ModelMatrix);

    void setScale(float X, float Y, float Z = 1.0f);
    void setScale(const GEVector3& Scale);
    void setColor(const GEColor& Color);
    void setTexture(unsigned int Texture);
    void setOpacity(float Opacity);
    void setVisible(bool Visible);
};


class GEMesh : public GERenderingObjectVisible
{
public:
    virtual void loadFromFile(const char* Filename);
    virtual void loadFromFile(const char* Filename, const char* TexturesPath);
    virtual void loadFromMemory(void* Data, unsigned int SizeOfData);
    virtual void loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals);
    virtual void loadFromArrays(unsigned int NumVertices, float* Vertex, float* Normals, float* TextureCoordinate);
    virtual void unload();
};


class GESprite : public GERenderingObjectVisible
{
protected:
    GEVector3 vCenter;
    
public:
    void setCenter(const GEVector3& Center);

    virtual void loadFromFile(const char* Filename);
    virtual void unload();
};


class GELabel : public GERenderingObjectVisible
{
protected:
    unsigned int iFont;
    GEAlignment iAlignment;
    
public:
    void setFont(unsigned int Font);
    void setAligment(GEAlignment Alignment);
    virtual void setText(const char* Text) = 0;
};


class GECamera : public GERenderingObject
{
protected:
    GEVector3 vLookAt;
    GEVector3 vUp;
    GEVector3 vRight;

public:
    GECamera();
    
    GEVector3& getLookAt();

    void lookAt(float X, float Y, float Z);
    void lookAt(const GEVector3& LookAt);
    void orbit(const GEVector3& ReferencePoint, float Distance, float Theta, float Phi);

    virtual void use();
};
