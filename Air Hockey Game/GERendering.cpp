
/*
    Arturo Cepeda Pérez

    Rendering Engine (Direct3D)

    --- GERendering.cpp ---
*/

#include "GERendering.h"


//
//  GERendering
//
GERendering::GERendering(HWND DeviceWindow, bool Windowed, unsigned int ScreenSizeX, unsigned int ScreenSizeY)
{
    iScreenSizeX = ScreenSizeX;
    iScreenSizeY = ScreenSizeY;
    
    memset(GECameras, 0, CAMERAS * sizeof(GECamera*));
    memset(vViewPorts, 0, VIEWPORTS * sizeof(D3DVIEWPORT9*));
    memset(rRegions, 0, REGIONS * sizeof(LPRECT));
    memset(fFonts, 0, FONTS * sizeof(LPD3DXFONT));

    // default view port
    defineViewPort(0, 0, 0, iScreenSizeX, iScreenSizeY);

    // Direct3D system
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    // Direct3D device
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.hDeviceWindow = DeviceWindow;
    d3dpp.Windowed = Windowed;

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    //d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
    d3dpp.MultiSampleQuality = 0;

    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = iScreenSizeX;
    d3dpp.BackBufferHeight = iScreenSizeY;

    HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DeviceWindow, 
                                   D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

    if(FAILED(hr))
    {
        MessageBox(DeviceWindow, "Couldn't initialize Direct3D", "Error", 0);
        exit(1);
    }

    // lighting settings
    d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
    iNumLights = 0;

    // Z-buffer
    d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    
    // stencil buffer
    d3ddev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
    d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    d3ddev->SetRenderState(D3DRS_STENCILREF, 0);
    d3ddev->SetRenderState(D3DRS_STENCILMASK, 0);
    d3ddev->SetRenderState(D3DRS_STENCILWRITEMASK, D3DSTENCILOP_KEEP);
    d3ddev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

    // specular lighting
    d3ddev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    d3ddev->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
    
    // anti-aliasing
    d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

    // set projection matrix
    setupProjectionMatrix();
}

GERendering::~GERendering()
{
    unsigned int i;

    // cameras
    for(i = 0; i < CAMERAS; i++)
    {
        if(GECameras[i])
            delete GECameras[i];
    }

    // fonts
    for(i = 0; i < FONTS; i++)
    {
        if(fFonts[i])
            fFonts[i]->Release();
    }

    // regions
    for(i = 0; i < REGIONS; i++)
    {
        if(rRegions[i])
            delete rRegions[i];
    }

    // view ports
    for(i = 0; i < VIEWPORTS; i++)
    {
        if(vViewPorts[i])
            delete vViewPorts[i];
    }

    // direct3d
    d3ddev->Release();
    d3d->Release();
}

void GERendering::createMesh(GEMesh** Mesh)
{
    *Mesh = new GEMesh(d3ddev);
}

void GERendering::releaseMesh(GEMesh** Mesh)
{
    if(*Mesh)
    {
        delete *Mesh;
        *Mesh = NULL;
    }
}

void GERendering::createSprite(GESprite** Sprite)
{
    *Sprite = new GESprite(d3ddev);
}

void GERendering::releaseSprite(GESprite** Sprite)
{
    if(*Sprite)
    {
        delete *Sprite;
        *Sprite = NULL;
    }
}

void GERendering::createCamera(GECamera** Camera)
{
    *Camera = new GECamera(d3ddev);
}

void GERendering::releaseCamera(GECamera** Camera)
{
    if(*Camera)
    {
        delete *Camera;
        *Camera = NULL;
    }
}

void GERendering::clearBuffers()
{
    d3ddev->SetViewport(vViewPorts[0]);
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 50), 1.0f, 0);

    for(unsigned int i = 1; i < VIEWPORTS; i++)
    {
        if(vViewPorts[i])
        {
            d3ddev->SetViewport(vViewPorts[i]);
            d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 50), 1.0f, 0);
        }
    }
}

void GERendering::setupProjectionMatrix()
{
    FLOAT fAspect = (FLOAT)iScreenSizeX / iScreenSizeY;
    FLOAT fMinViewDistance = 1.0f;
    FLOAT fMaxViewDistance = 1000.0f;

    D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI / 4, fAspect, fMinViewDistance, fMaxViewDistance);
    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void GERendering::setAmbientLight(unsigned char R, unsigned char G, unsigned char B)
{
    d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(R, G, B));
}

unsigned int GERendering::createDirectionalLight(float R, float G, float B, float A, float Range,
                                                 float DirX, float DirY, float DirZ)
{
    D3DLIGHT9 dLight;
    D3DXVECTOR3 vDir;

    ZeroMemory(&dLight, sizeof(D3DLIGHT9));

    dLight.Type = D3DLIGHT_DIRECTIONAL;
    dLight.Diffuse.r = R;
    dLight.Diffuse.g = G;
    dLight.Diffuse.b = B;
    dLight.Diffuse.a = A;
    dLight.Specular = dLight.Diffuse;
    dLight.Range = Range;

    vDir = D3DXVECTOR3(DirX, DirY, DirZ);
    D3DXVec3Normalize((D3DXVECTOR3*)&dLight.Direction, &vDir);

    d3ddev->SetLight(iNumLights, &dLight);

    return iNumLights++;
}

unsigned int GERendering::createPointLight(float R, float G, float B, float A, float Range, float Attenuation,
                                           float PosX, float PosY, float PosZ)
{
    D3DLIGHT9 dLight;

    ZeroMemory(&dLight, sizeof(D3DLIGHT9));

    dLight.Type = D3DLIGHT_POINT;
    dLight.Diffuse.r = R;
    dLight.Diffuse.g = G;
    dLight.Diffuse.b = B;
    dLight.Diffuse.a = A;
    dLight.Specular = dLight.Diffuse;
    dLight.Range = Range;
    dLight.Attenuation0 = Attenuation;
    dLight.Position = D3DXVECTOR3(PosX, PosY, PosZ);

    d3ddev->SetLight(iNumLights, &dLight);

    return iNumLights++;
}

unsigned int GERendering::createSpotLight(float R, float G, float B, float A, float Range, float Attenuation,
                                          float PosX, float PosY, float PosZ, float DirX, float DirY, float DirZ,
                                          float Theta, float Phi, float Falloff)
{
    D3DLIGHT9 dLight;

    ZeroMemory(&dLight, sizeof(D3DLIGHT9));

    dLight.Type = D3DLIGHT_SPOT;
    dLight.Diffuse.r = R;
    dLight.Diffuse.g = G;
    dLight.Diffuse.b = B;
    dLight.Diffuse.a = A;
    dLight.Specular = dLight.Diffuse;
    dLight.Range = Range;
    dLight.Attenuation0 = Attenuation;
    dLight.Position = D3DXVECTOR3(PosX, PosY, PosZ);
    dLight.Direction = D3DXVECTOR3(DirX, DirY, DirZ);
    dLight.Theta = Theta;
    dLight.Phi = Phi;
    dLight.Falloff = Falloff;

    d3ddev->SetLight(iNumLights, &dLight);

    return iNumLights++;
}

void GERendering::switchLight(unsigned int Light, bool On)
{
    d3ddev->LightEnable(Light, On);
}

void GERendering::moveLight(unsigned int Light, float DX, float DY, float DZ)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Position.x += DX;
    dLight.Position.x += DY;
    dLight.Position.x += DZ;
    d3ddev->SetLight(Light, &dLight);
}

void GERendering::releaseLight(unsigned int Light)
{
    d3ddev->LightEnable(Light, false);
}

void GERendering::setLightColor(unsigned int Light, float R, float G, float B, float A)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Diffuse.r = R;
    dLight.Diffuse.g = G;
    dLight.Diffuse.b = B;
    dLight.Diffuse.a = A;
    d3ddev->SetLight(Light, &dLight);
}

void GERendering::setLightRange(unsigned int Light, float Range)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Range = Range;
    d3ddev->SetLight(Light, &dLight);
}

void GERendering::setLightPosition(unsigned int Light, float PosX, float PosY, float PosZ)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Position = D3DXVECTOR3(PosX, PosY, PosZ);
    d3ddev->SetLight(Light, &dLight);
}

void GERendering::setLightDirection(unsigned int Light, float DirX, float DirY, float DirZ)
{
    D3DLIGHT9 dLight;
    D3DXVECTOR3 vDir;

    d3ddev->GetLight(Light, &dLight);
    vDir = D3DXVECTOR3(DirX, DirY, DirZ);
    D3DXVec3Normalize((D3DXVECTOR3*)&dLight.Direction, &vDir);    
    d3ddev->SetLight(Light, &dLight);
}

void GERendering::defineViewPort(unsigned int ViewPort, int X, int Y, int Width, int Height)
{
    if(ViewPort >= VIEWPORTS)
        return;

    if(!vViewPorts[ViewPort])
        vViewPorts[ViewPort] = new D3DVIEWPORT9();

    vViewPorts[ViewPort]->X = X;
    vViewPorts[ViewPort]->Y = Y;
    vViewPorts[ViewPort]->Width = Width;
    vViewPorts[ViewPort]->Height = Height;
    vViewPorts[ViewPort]->MinZ = 0.0f;
    vViewPorts[ViewPort]->MaxZ = 1.0f;
}

void GERendering::releaseViewPort(unsigned int ViewPort)
{
    if(ViewPort == 0 || ViewPort >= VIEWPORTS || !vViewPorts[ViewPort])
        return;

    delete vViewPorts[ViewPort];
    vViewPorts[ViewPort] = NULL;
}

void GERendering::useViewPort(unsigned int ViewPort)
{
    d3ddev->SetViewport(vViewPorts[ViewPort]);
}

void GERendering::defineRegion(unsigned int Region, int Top, int Bottom, int Left, int Right)
{
    if(Region >= REGIONS)
        return;

    if(!rRegions[Region])
        rRegions[Region] = new RECT();

    rRegions[Region]->top = Top;
    rRegions[Region]->bottom = Bottom;
    rRegions[Region]->left = Left;
    rRegions[Region]->right = Right;
}

void GERendering::releaseRegion(unsigned int Region)
{
    if(rRegions[Region])
    {
        delete rRegions[Region];
        rRegions[Region] = NULL;
    }
}

void GERendering::defineFont(unsigned int Font, unsigned int Height, unsigned int Width, bool Bold, bool Italic,
                             const char* FontName)
{
    if(Font >= FONTS)
        return;

    D3DXCreateFontA(d3ddev, Height, Width, Bold? FW_BOLD: FW_NORMAL, 0, Italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT(FontName), &fFonts[Font]);
}

void GERendering::releaseFont(unsigned int Font)
{
    if(fFonts[Font])
    {
        fFonts[Font]->Release();
        fFonts[Font] = NULL;
    }
}

void GERendering::worldToScreen(const GEPoint* PositionWorld, GEPoint* PositionScreen)
{
    D3DXMATRIX matProjection;
    d3ddev->GetTransform(D3DTS_PROJECTION, &matProjection);

    D3DXMATRIX matView;
    d3ddev->GetTransform(D3DTS_VIEW, &matView);

    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    
    D3DVIEWPORT9 vPort;
    d3ddev->GetViewport(&vPort);

    D3DXVECTOR3 vPositionWorld(PositionWorld->X, PositionWorld->Y, PositionWorld->Z);

    D3DXVECTOR3 vPositionScreen;
    D3DXVec3Project(&vPositionScreen, &vPositionWorld, &vPort, &matProjection, &matView, &matIdentity);

    PositionScreen->X = vPositionScreen.x;
    PositionScreen->Y = vPositionScreen.y;
    PositionScreen->Z = vPositionScreen.z;  // if < 1.0f, then the world position is in the screen
}

void GERendering::screenToWorld(const GEPoint* PositionScreen, GEPoint* WorldPointNear, GEPoint* WorldPointFar)
{
    D3DXMATRIX matProjection;
    d3ddev->GetTransform(D3DTS_PROJECTION, &matProjection);

    D3DXMATRIX matView;
    d3ddev->GetTransform(D3DTS_VIEW, &matView);

    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    
    D3DVIEWPORT9 vPort;
    d3ddev->GetViewport(&vPort);

    D3DXVECTOR3 vPositionScreen1(PositionScreen->X, PositionScreen->Y, 0.0f);
    D3DXVECTOR3 vPositionScreen2(PositionScreen->X, PositionScreen->Y, 1.0f);

    D3DXVECTOR3 vPositionWorld1;
    D3DXVec3Unproject(&vPositionWorld1, &vPositionScreen1, &vPort, &matProjection, &matView, &matIdentity);

    D3DXVECTOR3 vPositionWorld2;
    D3DXVec3Unproject(&vPositionWorld2, &vPositionScreen2, &vPort, &matProjection, &matView, &matIdentity);

    WorldPointNear->set(vPositionWorld1.x, vPositionWorld1.y, vPositionWorld1.z);
    WorldPointFar->set(vPositionWorld2.x, vPositionWorld2.y, vPositionWorld2.z);
}

void GERendering::renderBegin()
{
    clearBuffers();
    d3ddev->BeginScene();
}

void GERendering::renderMesh(GEMesh* Mesh)
{
    Mesh->render();
}

void GERendering::renderSprite(GESprite* Sprite)
{
    Sprite->render();
}

void GERendering::renderText(const char* Text, unsigned int Font, const GEColor& Color, unsigned char Opacity, 
                             unsigned int Region, unsigned int Alignment)
{
    int iFormat;

    switch(Alignment)
    {
    case ALIGN_TOP_LEFT:
        iFormat = DT_TOP | DT_LEFT;
        break;
    case ALIGN_TOP_CENTER:
        iFormat = DT_TOP | DT_CENTER;
        break;
    case ALIGN_TOP_RIGHT:
        iFormat = DT_TOP | DT_RIGHT;
        break;
    case ALIGN_CENTER_LEFT:
        iFormat = DT_VCENTER | DT_LEFT;
        break;
    case ALIGN_CENTER_CENTER:
        iFormat = DT_VCENTER | DT_CENTER;
        break;
    case ALIGN_CENTER_RIGHT:
        iFormat = DT_VCENTER | DT_RIGHT;
        break;
    case ALIGN_BOTTOM_LEFT:
        iFormat = DT_BOTTOM | DT_LEFT | DT_SINGLELINE;
        break;
    case ALIGN_BOTTOM_CENTER:
        iFormat = DT_BOTTOM | DT_CENTER | DT_SINGLELINE;
        break;
    case ALIGN_BOTTOM_RIGHT:
        iFormat = DT_BOTTOM | DT_RIGHT | DT_SINGLELINE;
        break;
    default:
        iFormat = 0;
    }

    fFonts[Font]->DrawTextA(NULL, Text, -1, rRegions[Region], iFormat, 
                            D3DCOLOR_ARGB(Opacity, Color.R, Color.G, Color.B));
}

void GERendering::renderEnd()
{
    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}
