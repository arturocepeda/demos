
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering (Direct3D 9)
//
//  --- GERenderingObjectsD3D9.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GERenderingObjectsD3D9.h"
#include <stdio.h>


//
//  GEMeshD3D9
//
GEMeshD3D9::GEMeshD3D9(LPDIRECT3DDEVICE9 Device)
{
    d3ddev = Device;
    mMesh = NULL;
    mMaterials = NULL;
    mTextures = NULL;
    iNumMaterials = 0;

    cColor = GEColor(1.0f, 1.0f, 1.0f);
    bVisible = true;

    memset(&vPosition, 0, sizeof(GEVector3));
    memset(&vCenter, 0, sizeof(GEVector3));
    memset(&qRotation, 0, sizeof(D3DXQUATERNION));

    vScale.X = 1.0f;
    vScale.Y = 1.0f;
    vScale.Z = 1.0f;
}

GEMeshD3D9::~GEMeshD3D9()
{
}

void GEMeshD3D9::loadFromFile(const char* Filename)
{
    loadFromFile(Filename, "");
}

void GEMeshD3D9::loadFromFile(const char* Filename, const char* TexturesPath)
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

void GEMeshD3D9::loadFromMemory(void* Data, unsigned int SizeOfData)
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

void GEMeshD3D9::unload()
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

void GEMeshD3D9::render()
{
    if(!bVisible)
        return;

    D3DXMatrixScaling(&mScale, vScale.X, vScale.Y, vScale.Z);
    //D3DXMatrixRotationQuaternion(&mRotation, &qRotation);
    D3DXMatrixTranslation(&mTranslation, vPosition.X, vPosition.Y, vPosition.Z);

    mTransform = mScale * /*mRotation * */mTranslation;
    d3ddev->SetTransform(D3DTS_WORLD, &mTransform);

    for(DWORD i = 0; i < iNumMaterials; i++)
    {
        d3ddev->SetMaterial(&mMaterials[i]);
        d3ddev->SetTexture(0, mTextures[i]);
        mMesh->DrawSubset(i);
    }
}



//
//  GESpriteD3D9
//
GESpriteD3D9::GESpriteD3D9(LPDIRECT3DDEVICE9 Device)
{
    d3ddev = Device;
    sSprite = NULL;
    sTexture = NULL;

    cColor = GEColor(1.0f, 1.0f, 1.0f);
    bVisible = true;

    memset(&vPosition, 0, sizeof(D3DXVECTOR3));
    memset(&vCenter, 0, sizeof(D3DXVECTOR3));
    memset(&qRotation, 0, sizeof(D3DXQUATERNION));

    vScale.X = 1.0f;
    vScale.Y = 1.0f;
    vScale.Z = 1.0f;
}

GESpriteD3D9::~GESpriteD3D9()
{
}

void GESpriteD3D9::loadFromFile(const char* Filename)
{
    D3DXCreateSprite(d3ddev, &sSprite);
    D3DXCreateTextureFromFile(d3ddev, Filename, &sTexture);

    D3DXIMAGE_INFO d3dImageInfo;
    D3DXGetImageInfoFromFile(Filename, &d3dImageInfo);

    vCenter.X = d3dImageInfo.Width / 2.0f;
    vCenter.Y = d3dImageInfo.Height / 2.0f;
    vCenter.Z = 0.0f;
}

void GESpriteD3D9::unload()
{
    if(sTexture)
        sTexture->Release();

    if(sSprite)
        sSprite->Release();
}

void GESpriteD3D9::render()
{
    sSprite->Begin(D3DXSPRITE_ALPHABLEND);

    D3DXVECTOR3 d3dvSpriteCenter(vPosition.X + vCenter.X, vPosition.Y + vCenter.Y, vPosition.Z + vCenter.Z);
    D3DXVECTOR3 d3dvScale(vScale.X, vScale.Y, vScale.Z);
    D3DXQUATERNION d3dqRotation(qRotation.X, qRotation.Y, qRotation.Z, qRotation.W);

    D3DXMatrixTransformation(&mTransform, &d3dvSpriteCenter, NULL, &d3dvScale, &d3dvSpriteCenter, &d3dqRotation, NULL);
    sSprite->SetTransform(&mTransform);

    D3DXVECTOR3 d3dvCenter(vCenter.X, vCenter.Y, vCenter.Z);
    D3DXVECTOR3 d3dvPosition(vPosition.X, vPosition.Y, vPosition.Z);

    DWORD dwColor = D3DCOLOR_ARGB((byte)(cColor.A * 255.0f), (byte)(cColor.R * 255.0f),
                                  (byte)(cColor.G * 255.0f), (byte)(cColor.B * 255.0f));
    sSprite->Draw(sTexture, NULL, &d3dvCenter, &d3dvPosition, dwColor);
    sSprite->End();
}



//
//  GELabelD3D9
//
GELabelD3D9::GELabelD3D9(LPDIRECT3DDEVICE9 Device, LPD3DXFONT Font, const char* Text, GEAlignment TextAligment,
                         unsigned int Width, unsigned int Height)
    : d3ddev(Device)
    , fFont(Font)
    , iWidth(Width)
    , iHeight(Height)
{
    cColor = GEColor(1.0f, 1.0f, 1.0f);
    vScale = GEVector3(1.0f, 1.0f, 1.0f);
    bVisible = true;

    memset(&vPosition, 0, sizeof(D3DXVECTOR3));
    memset(&vCenter, 0, sizeof(D3DXVECTOR3));
    memset(&qRotation, 0, sizeof(D3DXQUATERNION));

    strcpy(sText, Text);
    iAlignment = TextAligment;
    rRegion = new RECT();
}

GELabelD3D9::~GELabelD3D9()
{
    delete rRegion;
}

void GELabelD3D9::render()
{
    // rectangle
    rRegion->top = (LONG)vPosition.Y;
    rRegion->bottom = (LONG)vPosition.Y + iHeight;
    rRegion->left = (LONG)vPosition.X;
    rRegion->right = (LONG)vPosition.X + iWidth;

    // format
    int iFormat;

    switch(iAlignment)
    {
    case GEAlignment::TopLeft:
        iFormat = DT_TOP | DT_LEFT;
        break;
    case GEAlignment::TopCenter:
        iFormat = DT_TOP | DT_CENTER;
        break;
    case GEAlignment::TopRight:
        iFormat = DT_TOP | DT_RIGHT;
        break;
    case GEAlignment::CenterLeft:
        iFormat = DT_VCENTER | DT_LEFT;
        break;
    case GEAlignment::CenterCenter:
        iFormat = DT_VCENTER | DT_CENTER;
        break;
    case GEAlignment::CenterRight:
        iFormat = DT_VCENTER | DT_RIGHT;
        break;
    case GEAlignment::BottomLeft:
        iFormat = DT_BOTTOM | DT_LEFT | DT_SINGLELINE;
        break;
    case GEAlignment::BottomCenter:
        iFormat = DT_BOTTOM | DT_CENTER | DT_SINGLELINE;
        break;
    case GEAlignment::BottomRight:
        iFormat = DT_BOTTOM | DT_RIGHT | DT_SINGLELINE;
        break;
    default:
        iFormat = 0;
    }

    // color
    DWORD dwColor = D3DCOLOR_ARGB((byte)(cColor.A * 255.0f), (byte)(cColor.R * 255.0f),
                                  (byte)(cColor.G * 255.0f), (byte)(cColor.B * 255.0f));

    // render
    fFont->DrawTextA(NULL, sText, -1, rRegion, iFormat, dwColor);
}

void GELabelD3D9::setText(const char* Text)
{
    strcpy(sText, Text);
}


//
//  GECameraD3D9
//
GECameraD3D9::GECameraD3D9(LPDIRECT3DDEVICE9 Device)
{
    d3ddev = Device;
}

GECameraD3D9::~GECameraD3D9()
{
}

void GECameraD3D9::use()
{
    D3DXVECTOR3 d3dvEye(vPosition.X, vPosition.Y, vPosition.Z);
    D3DXVECTOR3 d3dvLookAt(vLookAt.X, vLookAt.Y, vLookAt.Z);
    D3DXVECTOR3 d3dvUp(vUp.X, vUp.Y, vUp.Z);

    D3DXMatrixLookAtLH(&mView, &d3dvEye, &d3dvLookAt, &d3dvUp);
    d3ddev->SetTransform(D3DTS_VIEW, &mView);
}
