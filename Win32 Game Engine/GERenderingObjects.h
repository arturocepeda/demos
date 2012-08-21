
/*
    Arturo Cepeda Pérez

	Rendering Engine (Direct3D 9)

    --- GERenderingObjects.h ---
*/

// Library links: d3d9.lib, d3dx9.lib

#ifndef _GERENDERINGOBJECTS_H_
#define _GERENDERINGOBJECTS_H_

#include <d3d9.h>
#include <d3dx9.h>

struct GEColor
{
    unsigned char R;
    unsigned char G;
    unsigned char B;

    GEColor()
    {
        set(0xFF, 0xFF, 0xFF);
    }

    GEColor(unsigned char cR, unsigned char cG, unsigned char cB)
    {
        set(cR, cG, cB);
    }

    void set(unsigned char cR, unsigned char cG, unsigned char cB)
    {
        R = cR;
        G = cG;
        B = cB;
    }
};

struct GEVector
{
    float X;
    float Y;
    float Z;

    GEVector()
    {
        set(0.0f, 0.0f, 0.0f);
    }

    GEVector(float vX, float vY, float vZ)
    {
        set(vX, vY, vZ);
    }

    void set(float vX, float vY, float vZ)
    {
        X = vX;
        Y = vY;
        Z = vZ;
    }
};

class GERenderingObject
{
protected:
    LPDIRECT3DDEVICE9 d3ddev;

    D3DXVECTOR3 vPosition;
    D3DXVECTOR3 vScale;    
    D3DXVECTOR3 vCenter;
    D3DXQUATERNION qRotation;

    D3DXMATRIX mTransform;

    unsigned char iOpacity;
    GEColor cColor;

    bool bVisible;

public:
    void move(float DX, float DY, float DZ);
    void move(const GEVector& Move);
    void scale(float SX, float SY, float SZ);
    void scale(const GEVector& Scale);
    void rotate(float RX, float RY, float RZ);
    void rotate(const GEVector& Rotate);
    void show();
    void hide();

    virtual void render() = 0;

    void getPosition(GEVector* Position);
    void getRotation(GEVector* Rotation);
    void getScale(GEVector* Scale);
    void getColor(GEColor* Color);
    unsigned char getOpacity();
    bool getVisible();

    void setPosition(float X, float Y, float Z);
    void setPosition(const GEVector& Position);
    void setScale(float X, float Y, float Z);
    void setScale(const GEVector& Scale);
    void setRotation(float X, float Y, float Z);
    void setRotation(const GEVector& Rotation);
    void setColor(const GEColor& Color);
    void setOpacity(unsigned char Opacity);
    void setVisible(bool Visible);
};

class GEMesh : public GERenderingObject
{
private:
    // mesh data
    LPD3DXMESH mMesh;
	D3DMATERIAL9* mMaterials;
	LPDIRECT3DTEXTURE9* mTextures;
	DWORD iNumMaterials;

    // matrices
    D3DXMATRIX mScale;
    D3DXMATRIX mRotationX, mRotationY, mRotationZ;
    D3DXMATRIX mTranslation;

public:
    GEMesh(LPDIRECT3DDEVICE9 D3DDevice);
    ~GEMesh();

    void loadFromFile(const char* Filename);
    void loadFromFile(const char* Filename, const char* TexturesPath);
    void loadFromMemory(void* Data, unsigned int SizeOfData);
    void unload();

    void render();
};


class GESprite : public GERenderingObject
{
private:
    // sprite data
    LPD3DXSPRITE sSprite;
    LPDIRECT3DTEXTURE9 sTexture;

public:
    GESprite(LPDIRECT3DDEVICE9 D3DDevice);
    ~GESprite();

    void loadFromFile(const char* Filename);
    void unload();

    void render();

    void setCenter(float X, float Y, float Z);
};


class GECamera
{
private:
    LPDIRECT3DDEVICE9 d3ddev;

    // camera vectors
	D3DXVECTOR3 vEye;
	D3DXVECTOR3 vLookAt;
	D3DXVECTOR3 vUp;
    D3DXVECTOR3 vRight;

public:
    GECamera(LPDIRECT3DDEVICE9 D3DDevice);
    ~GECamera();

    void setPosition(float X, float Y, float Z);
    void setPosition(const GEVector& Position);
    void move(float X, float Y, float Z);
    void move(const GEVector& Move);
    void lookAt(float X, float Y, float Z);
    void lookAt(const GEVector& LookAt);
    void orbit(const GEVector& ReferencePoint, float Distance, float Theta, float Phi);

    void use();
};

#endif
