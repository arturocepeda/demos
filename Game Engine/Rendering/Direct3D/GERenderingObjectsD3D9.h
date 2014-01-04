
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering (Direct3D 9)
//
//  --- GERenderingObjectsD3D9.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Rendering/GERenderingObjects.h"
#include <d3d9.h>
#include <d3dx9.h>

class GEMeshD3D9 : public GEMesh
{
private:
    LPDIRECT3DDEVICE9 d3ddev;
    D3DXMATRIX mTransform;

    // mesh data
    LPD3DXMESH mMesh;
    D3DMATERIAL9* mMaterials;
    LPDIRECT3DTEXTURE9* mTextures;
    DWORD iNumMaterials;

    // matrices
    D3DXMATRIX mScale;
    D3DXMATRIX mRotation;
    D3DXMATRIX mTranslation;

public:
    GEMeshD3D9(LPDIRECT3DDEVICE9 Device);
    ~GEMeshD3D9();

    void loadFromFile(const char* Filename) override;
    void loadFromFile(const char* Filename, const char* TexturesPath) override;
    void loadFromMemory(void* Data, unsigned int SizeOfData) override;
    void unload() override;

    void render() override;
};


class GESpriteD3D9 : public GESprite
{
private:
    LPDIRECT3DDEVICE9 d3ddev;
    D3DXMATRIX mTransform;

    // sprite data
    LPD3DXSPRITE sSprite;
    LPDIRECT3DTEXTURE9 sTexture;

public:
    GESpriteD3D9(LPDIRECT3DDEVICE9 Device);
    ~GESpriteD3D9();

    void loadFromFile(const char* Filename) override;
    void unload() override;

    void render() override;
};


class GELabelD3D9 : public GELabel
{
private:
    LPDIRECT3DDEVICE9 d3ddev;
    LPD3DXFONT fFont;
    LPRECT rRegion;

    char sText[1024];
    unsigned int iWidth;
    unsigned int iHeight;

public:
    GELabelD3D9(LPDIRECT3DDEVICE9 Device, LPD3DXFONT Font, const char* Text, GEAlignment TextAligment,
                unsigned int Width, unsigned int Height);
    ~GELabelD3D9();

    void render() override;
    void setText(const char* Text) override;
};


class GECameraD3D9 : public GECamera
{
private:
    LPDIRECT3DDEVICE9 d3ddev;

    // view matrix
    D3DXMATRIX mView;

public:
    GECameraD3D9(LPDIRECT3DDEVICE9 Device);
    ~GECameraD3D9();

    void getPosition(GEVector3* Position);
    void getLookAt(GEVector3* LookAt);

    void setPosition(float X, float Y, float Z);
    void setPosition(const GEVector3& Position);

    void move(float X, float Y, float Z);
    void move(const GEVector3& Move);
    void lookAt(float X, float Y, float Z);
    void lookAt(const GEVector3& LookAt);
    void orbit(const GEVector3& ReferencePoint, float Distance, float Theta, float Phi);

    void use() override;
};
