
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
#define FONTS       16

class GERenderingD3D9 : public GERendering
{
private:
    LPDIRECT3DDEVICE9 d3ddev;

    unsigned int iNumLights;

    D3DVIEWPORT9* vViewPorts[VIEWPORTS];
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

    // labels
    void createLabel(GELabel** Label, unsigned int Font, GEAlignment Alignment,
                     unsigned int Width, unsigned int Height, const char* Text = "") override;

    // cameras
    void createCamera(GECamera** Camera) override;

    // lighting
    void setAmbientLightColor(const GEColor& Color) override;

    unsigned int createDirectionalLight(const GEColor& Color, float Range,
                                        const GEVector3& Direction) override;
    unsigned int createPointLight(const GEColor& Color, float Range, float Attenuation,
                                  const GEVector3& Position) override;
    unsigned int createSpotLight(const GEColor& Color, float Range, float Attenuation,
                                 const GEVector3& Position, const GEVector3& Direction,
                                 float Theta, float Phi, float Falloff) override;

    void switchLight(unsigned int Light, bool On) override;
    void moveLight(unsigned int Light, const GEVector3& Delta) override;
    void releaseLight(unsigned int Light) override;
    void setLightColor(unsigned int Light, const GEColor& Color) override;
    void setLightRange(unsigned int Light, float Range) override;
    void setLightPosition(unsigned int Light, const GEVector3& Position) override;
    void setLightDirection(unsigned int Light, const GEVector3& Direction) override;

    // view ports
    void defineViewPort(unsigned int ViewPort, int X, int Y, int Width, int Height) override;
    void useViewPort(unsigned int ViewPort) override;
    void releaseViewPort(unsigned int ViewPort) override;

    // fonts
    void defineFont(unsigned int Font, const char* FontName, float Size,
                    bool Bold = false, bool Italic = false) override;
    void releaseFont(unsigned int Font) override;

    // transformations
    void worldToScreen(const GEVector3* PositionWorld, GEVector3* PositionScreen) override;
    void screenToWorld(const GEVector3* PositionScreen, GEVector3* WorldPointNear,
                       GEVector3* WorldPointFar) override;

    // rendering
    void renderBegin() override;
    void renderEnd() override;
};
