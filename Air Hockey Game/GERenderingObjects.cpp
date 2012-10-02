
/*
    Arturo Cepeda Pérez

    Rendering Engine (Direct3D)

    --- GERenderingObjects.cpp ---
*/

#include "GERenderingObjects.h"
#include <stdio.h>


//
//  GERenderingObject
//
void GERenderingObject::move(float DX, float DY, float DZ)
{
    vPosition.x += DX;
    vPosition.y += DY;
    vPosition.z += DZ;
}

void GERenderingObject::move(const GEVector& Move)
{
    vPosition.x += Move.X;
    vPosition.y += Move.Y;
    vPosition.z += Move.Z;
}

void GERenderingObject::scale(float SX, float SY, float SZ)
{
    vScale.x *= SX;
    vScale.y *= SY;
    vScale.z *= SZ;
}

void GERenderingObject::scale(const GEVector& Scale)
{
    vScale.x *= Scale.X;
    vScale.y *= Scale.Y;
    vScale.z *= Scale.Z;
}

void GERenderingObject::rotate(float RX, float RY, float RZ)
{
    qRotation.x += RX;
    qRotation.y += RY;
    qRotation.z += RZ;
}

void GERenderingObject::rotate(const GEVector& Rotate)
{
    qRotation.x += Rotate.X;
    qRotation.y += Rotate.Y;
    qRotation.z += Rotate.Z;
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
    Position->X = vPosition.x;
    Position->Y = vPosition.y;
    Position->Z = vPosition.z;
}

void GERenderingObject::getRotation(GEVector* Rotation)
{
    Rotation->X = qRotation.x;
    Rotation->Y = qRotation.y;
    Rotation->Z = qRotation.z;
}

void GERenderingObject::getScale(GEVector* Scale)
{
    Scale->X = vScale.x;
    Scale->Y = vScale.y;
    Scale->Z = vScale.z;
}

void GERenderingObject::getColor(GEColor* Color)
{
    Color->R = cColor.R;
    Color->G = cColor.G;
    Color->B = cColor.B;
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
    vPosition.x = X;
    vPosition.y = Y;
    vPosition.z = Z;
}

void GERenderingObject::setPosition(const GEVector& Position)
{
    vPosition.x = Position.X;
    vPosition.y = Position.Y;
    vPosition.z = Position.Z;
}

void GERenderingObject::setScale(float X, float Y, float Z)
{
    vScale.x = X;
    vScale.y = Y;
    vScale.z = Z;
}

void GERenderingObject::setScale(const GEVector& Scale)
{
    vScale.x = Scale.X;
    vScale.y = Scale.Y;
    vScale.z = Scale.Z;
}

void GERenderingObject::setRotation(float X, float Y, float Z)
{
    qRotation.x = X;
    qRotation.y = Y;
    qRotation.z = Z;
}

void GERenderingObject::setRotation(const GEVector& Rotation)
{
    qRotation.x = Rotation.X;
    qRotation.y = Rotation.Y;
    qRotation.z = Rotation.Z;
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
//  GEMesh
//
GEMesh::GEMesh(LPDIRECT3DDEVICE9 D3DDevice)
{
    d3ddev = D3DDevice;
    mMesh = NULL;
    mMaterials = NULL;
    mTextures = NULL;
    iNumMaterials = 0;

    iOpacity = 0xFF;
    bVisible = true;

    memset(&vPosition, 0, sizeof(D3DXVECTOR3));
    memset(&vCenter, 0, sizeof(D3DXVECTOR3));
    memset(&qRotation, 0, sizeof(D3DXQUATERNION));

    vScale.x = 1.0f;
    vScale.y = 1.0f;
    vScale.z = 1.0f;
}

GEMesh::~GEMesh()
{
}

void GEMesh::loadFromFile(const char* Filename)
{
    loadFromFile(Filename, "");
}

void GEMesh::loadFromFile(const char* Filename, const char* TexturesPath)
{
    LPD3DXBUFFER pD3DXMtrlBuffer;
    D3DXMATERIAL* d3dxMaterials;
    char sTextureFilename[256];

    // load mesh
    D3DXLoadMeshFromXA(Filename, D3DXMESH_SYSTEMMEM, d3ddev, NULL, &pD3DXMtrlBuffer, NULL, &iNumMaterials, &mMesh);

    // load materials and textures
    d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    mMaterials = new D3DMATERIAL9[iNumMaterials];
    mTextures = new LPDIRECT3DTEXTURE9[iNumMaterials];

    for(DWORD i = 0; i < iNumMaterials; i++)
    {
        mMaterials[i] = d3dxMaterials[i].MatD3D;
        mMaterials[i].Ambient = mMaterials[i].Diffuse;
        mMaterials[i].Specular.a = 0.5f;
        mMaterials[i].Specular.r = 0.5f;
        mMaterials[i].Specular.g = 0.5f;
        mMaterials[i].Specular.b = 0.5f;
        mTextures[i] = NULL;

        if(d3dxMaterials[i].pTextureFilename != NULL && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
        {
            sprintf(sTextureFilename, "%s%s", TexturesPath, d3dxMaterials[i].pTextureFilename);
            D3DXCreateTextureFromFileA(d3ddev, sTextureFilename, &mTextures[i]);
        }
    }

    pD3DXMtrlBuffer->Release();
}

void GEMesh::loadFromMemory(void* Data, unsigned int SizeOfData)
{
    LPD3DXBUFFER pD3DXMtrlBuffer;
    D3DXMATERIAL* d3dxMaterials;

    // load mesh
    D3DXLoadMeshFromXInMemory(Data, SizeOfData, D3DXMESH_SYSTEMMEM, d3ddev, NULL, &pD3DXMtrlBuffer, NULL, 
                              &iNumMaterials, &mMesh);
    
    // load materials and textures
    d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    mMaterials = new D3DMATERIAL9[iNumMaterials];
    mTextures = new LPDIRECT3DTEXTURE9[iNumMaterials];

    for(DWORD i = 0; i < iNumMaterials; i++)
    {
        mMaterials[i] = d3dxMaterials[i].MatD3D;
        mMaterials[i].Ambient = mMaterials[i].Diffuse;
        mMaterials[i].Specular = mMaterials[i].Diffuse;
        mTextures[i] = NULL;

        if(d3dxMaterials[i].pTextureFilename != NULL && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
            D3DXCreateTextureFromFileA(d3ddev, d3dxMaterials[i].pTextureFilename, &mTextures[i]);
    }

    pD3DXMtrlBuffer->Release();
}

void GEMesh::unload()
{
    if(mMaterials)
        delete[] mMaterials;

    for(unsigned int i = 0; i < iNumMaterials; i++)
    {
        if(mTextures[i])
            mTextures[i]->Release();
    }

    if(mTextures)
        delete[] mTextures;

    if(mMesh)
        mMesh->Release();

    iNumMaterials = 0;
}

void GEMesh::render()
{
    if(!bVisible)
        return;

    D3DXMatrixScaling(&mScale, vScale.x, vScale.y, vScale.z);
    D3DXMatrixRotationX(&mRotationX, qRotation.x);
    D3DXMatrixRotationY(&mRotationY, qRotation.y);
    D3DXMatrixRotationZ(&mRotationZ, qRotation.z);
    D3DXMatrixTranslation(&mTranslation, vPosition.x, vPosition.y, vPosition.z);

    mTransform = mScale * mRotationX * mRotationY * mRotationZ * mTranslation;
    d3ddev->SetTransform(D3DTS_WORLD, &mTransform);

    for(DWORD i = 0; i < iNumMaterials; i++)
    {
        d3ddev->SetMaterial(&mMaterials[i]);
        d3ddev->SetTexture(0, mTextures[i]);
        mMesh->DrawSubset(i);
    }
}



//
//  GESprite
//
GESprite::GESprite(LPDIRECT3DDEVICE9 D3DDevice)
{
    d3ddev = D3DDevice;
    sSprite = NULL;
    sTexture = NULL;
    iOpacity = 0xFF;
    bVisible = true;

    memset(&vPosition, 0, sizeof(D3DXVECTOR3));
    memset(&vCenter, 0, sizeof(D3DXVECTOR3));
    memset(&qRotation, 0, sizeof(D3DXQUATERNION));

    vScale.x = 1.0f;
    vScale.y = 1.0f;
    vScale.z = 1.0f;
}

GESprite::~GESprite()
{
}

void GESprite::loadFromFile(const char* Filename)
{
    D3DXCreateSprite(d3ddev, &sSprite);
    D3DXCreateTextureFromFile(d3ddev, Filename, &sTexture);

    D3DXIMAGE_INFO d3dImageInfo;
    D3DXGetImageInfoFromFile(Filename, &d3dImageInfo);

    vCenter.x = d3dImageInfo.Width / 2.0f;
    vCenter.y = d3dImageInfo.Height / 2.0f;
    vCenter.z = 0.0f;
}

void GESprite::unload()
{
    if(sTexture)
        sTexture->Release();

    if(sSprite)
        sSprite->Release();
}

void GESprite::render()
{
    sSprite->Begin(D3DXSPRITE_ALPHABLEND);

    D3DXVECTOR3 vSpriteCenter(vPosition.x + vCenter.x, vPosition.y + vCenter.y, vPosition.z + vCenter.z);
    D3DXMatrixTransformation(&mTransform, &vSpriteCenter, NULL, &vScale, &vSpriteCenter, &qRotation, NULL);

    sSprite->SetTransform(&mTransform);
    sSprite->Draw(sTexture, NULL, &vCenter, &vPosition, D3DCOLOR_ARGB(iOpacity, 0xFF, 0xFF, 0xFF));
    sSprite->End();
}

void GESprite::setCenter(float X, float Y, float Z)
{
    vCenter.x = X;
    vCenter.y = Y;
    vCenter.z = Z;
}



//
//  GECamera
//
GECamera::GECamera(LPDIRECT3DDEVICE9 D3DDevice)
{
    d3ddev = D3DDevice;

    // eye vector
    vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // lookat vector
    vLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // up vector
    vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    // right vector
    vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

GECamera::~GECamera()
{
}

void GECamera::getPosition(GEVector* Position)
{
    Position->X = vEye.x;
    Position->Y = vEye.y;
    Position->Z = vEye.z;
}

void GECamera::getLookAt(GEVector* LookAt)
{
    LookAt->X = vLookAt.x;
    LookAt->Y = vLookAt.y;
    LookAt->Z = vLookAt.z;
}

void GECamera::setPosition(float x, float y, float z)
{
    vEye.x = x;
    vEye.y = y;
    vEye.z = z;
}

void GECamera::setPosition(const GEVector& Position)
{
    vEye.x = Position.X;
    vEye.y = Position.Y;
    vEye.z = Position.Z;
}

void GECamera::move(float x, float y, float z)
{
    vEye.x += x;
    vEye.y += y;
    vEye.z += z;
}

void GECamera::move(const GEVector& Move)
{
    vEye.x += Move.X;
    vEye.y += Move.Y;
    vEye.z += Move.Z;
}

void GECamera::lookAt(float x, float y, float z)
{
    vLookAt.x = x;
    vLookAt.y = y;
    vLookAt.z = z;
}

void GECamera::lookAt(const GEVector& LookAt)
{
    vLookAt.x = LookAt.X;
    vLookAt.y = LookAt.Y;
    vLookAt.z = LookAt.Z;
}

void GECamera::orbit(const GEVector& ReferencePoint, float Distance, float Theta, float Phi)
{
    setPosition(Distance * cosf(Theta) * sinf(Phi),
                Distance * -cosf(Phi), 
                Distance * sinf(Theta) * sinf(Phi));
    lookAt(ReferencePoint.X, ReferencePoint.Y, ReferencePoint.Z);
}

void GECamera::use()
{
    D3DXMatrixLookAtLH(&mView, &vEye, &vLookAt, &vUp);
    d3ddev->SetTransform(D3DTS_VIEW, &mView);
}
