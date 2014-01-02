
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering (Direct3D 9)
//
//  --- GERenderingD3D9.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GERenderingD3D9.h"
#include "GEGraphicsDeviceD3D9.h"
#include "GERenderingObjectsD3D9.h"

GERenderingD3D9::GERenderingD3D9(void* Window, bool Windowed, unsigned int ScreenWidth, unsigned int ScreenHeight)
    : GERendering(Window, Windowed, ScreenWidth, ScreenHeight)
{
    cDevice = new GEGraphicsDeviceD3D9(pWindow, bWindowed, iScreenWidth, iScreenHeight);
    d3ddev = (LPDIRECT3DDEVICE9)cDevice->getNativePointer();

    memset(vViewPorts, 0, VIEWPORTS * sizeof(D3DVIEWPORT9*));
    memset(rRegions, 0, REGIONS * sizeof(LPRECT));
    memset(fFonts, 0, FONTS * sizeof(LPD3DXFONT));

    // default view port
    defineViewPort(0, 0, 0, iScreenWidth, iScreenHeight);

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

GERenderingD3D9::~GERenderingD3D9()
{
    unsigned int i;

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

    // device
    delete cDevice;
}

void GERenderingD3D9::clearBuffers()
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

void GERenderingD3D9::setupProjectionMatrix()
{
    FLOAT fAspect = (FLOAT)iScreenWidth / iScreenHeight;
    FLOAT fMinViewDistance = 1.0f;
    FLOAT fMaxViewDistance = 1000.0f;

    D3DXMatrixPerspectiveFovLH(&mProjection, D3DX_PI / 4, fAspect, fMinViewDistance, fMaxViewDistance);
    d3ddev->SetTransform(D3DTS_PROJECTION, &mProjection);
}

void GERenderingD3D9::createMesh(GEMesh** Mesh)
{
    *Mesh = new GEMeshD3D9(d3ddev);
}

void GERenderingD3D9::createSprite(GESprite** Sprite)
{
    *Sprite = new GESpriteD3D9(d3ddev);
}

void GERenderingD3D9::createCamera(GECamera** Camera)
{
    *Camera = new GECameraD3D9(d3ddev);
}

void GERenderingD3D9::setAmbientLight(const GEColor& Color)
{
    DWORD dwColor = D3DCOLOR_XRGB((byte)(Color.R * 255.0f), (byte)(Color.G * 255.0f),
                                  (byte)(Color.B * 255.0f));
    d3ddev->SetRenderState(D3DRS_AMBIENT, dwColor);
}

unsigned int GERenderingD3D9::createDirectionalLight(const GEColor& Color, float Range,
                                                     const GEVector3& Direction)
{
    D3DLIGHT9 dLight;
    D3DXVECTOR3 vDir;

    ZeroMemory(&dLight, sizeof(D3DLIGHT9));

    dLight.Type = D3DLIGHT_DIRECTIONAL;
    dLight.Diffuse.r = Color.R;
    dLight.Diffuse.g = Color.G;
    dLight.Diffuse.b = Color.B;
    dLight.Diffuse.a = Color.A;
    dLight.Specular = dLight.Diffuse;
    dLight.Range = Range;

    vDir = D3DXVECTOR3(Direction.X, Direction.Y, Direction.Z);
    D3DXVec3Normalize((D3DXVECTOR3*)&dLight.Direction, &vDir);

    d3ddev->SetLight(iNumLights, &dLight);

    return iNumLights++;
}

unsigned int GERenderingD3D9::createPointLight(const GEColor& Color, float Range, float Attenuation,
                                               const GEVector3& Position)
{
    D3DLIGHT9 dLight;

    ZeroMemory(&dLight, sizeof(D3DLIGHT9));

    dLight.Type = D3DLIGHT_POINT;
    dLight.Diffuse.r = Color.R;
    dLight.Diffuse.g = Color.G;
    dLight.Diffuse.b = Color.B;
    dLight.Diffuse.a = Color.A;
    dLight.Specular = dLight.Diffuse;
    dLight.Range = Range;
    dLight.Attenuation0 = Attenuation;
    dLight.Position = D3DXVECTOR3(Position.X, Position.Y, Position.Z);

    d3ddev->SetLight(iNumLights, &dLight);

    return iNumLights++;
}

unsigned int GERenderingD3D9::createSpotLight(const GEColor& Color, float Range, float Attenuation,
                                              const GEVector3& Position, const GEVector3& Direction,
                                              float Theta, float Phi, float Falloff)
{
    D3DLIGHT9 dLight;

    ZeroMemory(&dLight, sizeof(D3DLIGHT9));

    dLight.Type = D3DLIGHT_SPOT;
    dLight.Diffuse.r = Color.R;
    dLight.Diffuse.g = Color.G;
    dLight.Diffuse.b = Color.B;
    dLight.Diffuse.a = Color.A;
    dLight.Specular = dLight.Diffuse;
    dLight.Range = Range;
    dLight.Attenuation0 = Attenuation;
    dLight.Position = D3DXVECTOR3(Position.X, Position.Y, Position.Z);
    D3DXVECTOR3 vDir(Direction.X, Direction.Y, Direction.Z);
    D3DXVec3Normalize((D3DXVECTOR3*)&dLight.Direction, &vDir);
    dLight.Theta = Theta;
    dLight.Phi = Phi;
    dLight.Falloff = Falloff;

    d3ddev->SetLight(iNumLights, &dLight);

    return iNumLights++;
}

void GERenderingD3D9::switchLight(unsigned int Light, bool On)
{
    d3ddev->LightEnable(Light, On);
}

void GERenderingD3D9::moveLight(unsigned int Light, const GEVector3& Delta)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Position.x += Delta.X;
    dLight.Position.x += Delta.Y;
    dLight.Position.x += Delta.Z;
    d3ddev->SetLight(Light, &dLight);
}

void GERenderingD3D9::releaseLight(unsigned int Light)
{
    d3ddev->LightEnable(Light, false);
}

void GERenderingD3D9::setLightColor(unsigned int Light, const GEColor& Color)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Diffuse.r = Color.R;
    dLight.Diffuse.g = Color.G;
    dLight.Diffuse.b = Color.B;
    dLight.Diffuse.a = Color.A;
    d3ddev->SetLight(Light, &dLight);
}

void GERenderingD3D9::setLightRange(unsigned int Light, float Range)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Range = Range;
    d3ddev->SetLight(Light, &dLight);
}

void GERenderingD3D9::setLightPosition(unsigned int Light, const GEVector3& Position)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    dLight.Position = D3DXVECTOR3(Position.X, Position.Y, Position.Z);
    d3ddev->SetLight(Light, &dLight);
}

void GERenderingD3D9::setLightDirection(unsigned int Light, const GEVector3& Direction)
{
    D3DLIGHT9 dLight;

    d3ddev->GetLight(Light, &dLight);
    D3DXVECTOR3 vDir(Direction.X, Direction.Y, Direction.Z);
    D3DXVec3Normalize((D3DXVECTOR3*)&dLight.Direction, &vDir);
    d3ddev->SetLight(Light, &dLight);
}

void GERenderingD3D9::defineViewPort(unsigned int ViewPort, int X, int Y, int Width, int Height)
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

void GERenderingD3D9::releaseViewPort(unsigned int ViewPort)
{
    if(ViewPort == 0 || ViewPort >= VIEWPORTS || !vViewPorts[ViewPort])
        return;

    delete vViewPorts[ViewPort];
    vViewPorts[ViewPort] = NULL;
}

void GERenderingD3D9::useViewPort(unsigned int ViewPort)
{
    d3ddev->SetViewport(vViewPorts[ViewPort]);
}

void GERenderingD3D9::defineRegion(unsigned int Region, int Top, int Bottom, int Left, int Right)
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

void GERenderingD3D9::releaseRegion(unsigned int Region)
{
    if(rRegions[Region])
    {
        delete rRegions[Region];
        rRegions[Region] = NULL;
    }
}

void GERenderingD3D9::defineFont(unsigned int Font, unsigned int Height, unsigned int Width, bool Bold, bool Italic,
                                 const char* FontName)
{
    if(Font >= FONTS)
        return;

    D3DXCreateFontA(d3ddev, Height, Width, Bold? FW_BOLD: FW_NORMAL, 0, Italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT(FontName), &fFonts[Font]);
}

void GERenderingD3D9::releaseFont(unsigned int Font)
{
    if(fFonts[Font])
    {
        fFonts[Font]->Release();
        fFonts[Font] = NULL;
    }
}

void GERenderingD3D9::worldToScreen(const GEVector3* PositionWorld, GEVector3* PositionScreen)
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

void GERenderingD3D9::screenToWorld(const GEVector3* PositionScreen, GEVector3* WorldPointNear, GEVector3* WorldPointFar)
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

void GERenderingD3D9::renderBegin()
{
    clearBuffers();
    d3ddev->BeginScene();
}

void GERenderingD3D9::renderText(const char* Text, unsigned int Font, const GEColor& Color, unsigned int Region,
                                 GEAlignment Alignment, float Opacity)
{
    int iFormat;

    switch(Alignment)
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

    DWORD dwColor = D3DCOLOR_ARGB((byte)(Opacity * 255.0f), (byte)(Color.R * 255.0f),
                                  (byte)(Color.G * 255.0f), (byte)(Color.B * 255.0f));
    fFonts[Font]->DrawTextA(NULL, Text, -1, rRegions[Region], iFormat, dwColor);
}

void GERenderingD3D9::renderEnd()
{
    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}
