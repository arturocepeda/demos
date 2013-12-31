
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering (Direct3D 9)
//
//  --- GERenderingD3D9.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Rendering/GERendering.h"
#include <d3d9.h>
#include <d3dx9.h>

#define VIEWPORTS   16
#define REGIONS     16
#define FONTS       16

class GERenderingD3D9 : public GERendering
{
private:
    LPDIRECT3DDEVICE9 d3ddev;

    unsigned int iNumLights;

    D3DVIEWPORT9* vViewPorts[VIEWPORTS];
    LPRECT rRegions[REGIONS];
    LPD3DXFONT fFonts[FONTS];

    D3DXMATRIX mProjection;

    void setupProjectionMatrix() override;
    void clearBuffers() override;

public:
    GERenderingD3D9(void* Window, bool Windowed, unsigned int ScreenSizeX, unsigned int ScreenSizeY);
    ~GERenderingD3D9();

    // meshes
    void createMesh(GEMesh** Mesh) override;

    // sprites
    void createSprite(GESprite** Sprite) override;

    // cameras
    void createCamera(GECamera** Camera) override;

    // lighting
    void setAmbientLight(unsigned char R, unsigned char G, unsigned char B);

    unsigned int createDirectionalLight(float R, float G, float B, float A, float Range,
                                        float DirX, float DirY, float DirZ);
    unsigned int createPointLight(float R, float G, float B, float A, float Range, float Attenuation,
                                  float PosX, float PosY, float PosZ);
    unsigned int createSpotLight(float R, float G, float B, float A, float Range, float Attenuation,
                                 float PosX, float PosY, float PosZ, float DirX, float DirY, float DirZ,
                                 float Theta, float Phi, float Falloff);

    void switchLight(unsigned int Light, bool On);
    void moveLight(unsigned int Light, float DX, float DY, float DZ);
    void releaseLight(unsigned int Light);
    void setLightColor(unsigned int Light, float R, float G, float B, float A);
    void setLightRange(unsigned int Light, float Range);
    void setLightPosition(unsigned int Light, float PosX, float PosY, float PosZ);
    void setLightDirection(unsigned int Light, float DirX, float DirY, float DirZ);

    // view ports
    void defineViewPort(unsigned int ViewPort, int X, int Y, int Width, int Height);
    void useViewPort(unsigned int ViewPort);
    void releaseViewPort(unsigned int ViewPort);

    // regions
    void defineRegion(unsigned int Region, int Top, int Bottom, int Left, int Right);
    void releaseRegion(unsigned int Region);

    // fonts
    void defineFont(unsigned int Font, unsigned int Height, unsigned int Width, bool Bold, bool Italic,
                    const char* FontName);
    void releaseFont(unsigned int Font);

    // transformations
    void worldToScreen(const GEVector3* PositionWorld, GEVector3* PositionScreen);
    void screenToWorld(const GEVector3* PositionScreen, GEVector3* WorldPointNear, GEVector3* WorldPointFar);

    // rendering
    void renderBegin();
    void renderText(const char* Text, unsigned int Font, const GEColor& Color, unsigned char Opacity,
                    unsigned int Region, GEAlignment Alignment);
    void renderEnd();
};
