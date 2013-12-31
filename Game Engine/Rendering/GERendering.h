
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering
//
//  --- GERendering.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "GEGraphicsDevice.h"
#include "GERenderingObjects.h"

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

class GERendering
{
protected:
    void* pWindow;
    bool bWindowed;
    unsigned int iScreenSizeX;
    unsigned int iScreenSizeY;

    GEGraphicsDevice* cDevice;

    virtual void setupProjectionMatrix() = 0;
    virtual void clearBuffers() = 0;

public:
    GERendering(void* Window, bool Windowed, unsigned int ScreenSizeX, unsigned int ScreenSizeY);
    virtual ~GERendering();

    // meshes
    virtual void createMesh(GEMesh** Mesh) = 0;
    void releaseMesh(GEMesh** Mesh);

    // sprites
    virtual void createSprite(GESprite** Sprite) = 0;
    void releaseSprite(GESprite** Sprite);

    // cameras
    virtual void createCamera(GECamera** Camera) = 0;
    void releaseCamera(GECamera** Camera);

    // lighting
    virtual void setAmbientLight(unsigned char R, unsigned char G, unsigned char B) = 0;

    virtual unsigned int createDirectionalLight(float R, float G, float B, float A, float Range,
                                                float DirX, float DirY, float DirZ) = 0;
    virtual unsigned int createPointLight(float R, float G, float B, float A, float Range, float Attenuation,
                                          float PosX, float PosY, float PosZ) = 0;
    virtual unsigned int createSpotLight(float R, float G, float B, float A, float Range, float Attenuation,
                                         float PosX, float PosY, float PosZ, float DirX, float DirY, float DirZ,
                                         float Theta, float Phi, float Falloff) = 0;

    virtual void switchLight(unsigned int Light, bool On) = 0;
    virtual void moveLight(unsigned int Light, float DX, float DY, float DZ) = 0;
    virtual void releaseLight(unsigned int Light) = 0;
    virtual void setLightColor(unsigned int Light, float R, float G, float B, float A) = 0;
    virtual void setLightRange(unsigned int Light, float Range) = 0;
    virtual void setLightPosition(unsigned int Light, float PosX, float PosY, float PosZ) = 0;
    virtual void setLightDirection(unsigned int Light, float DirX, float DirY, float DirZ) = 0;

    // view ports
    virtual void defineViewPort(unsigned int ViewPort, int X, int Y, int Width, int Height) = 0;
    virtual void useViewPort(unsigned int ViewPort) = 0;
    virtual void releaseViewPort(unsigned int ViewPort) = 0;

    // regions
    virtual void defineRegion(unsigned int Region, int Top, int Bottom, int Left, int Right) = 0;
    virtual void releaseRegion(unsigned int Region) = 0;

    // fonts
    virtual void defineFont(unsigned int Font, unsigned int Height, unsigned int Width, bool Bold, bool Italic,
                            const char* FontName) = 0;
    virtual void releaseFont(unsigned int Font) = 0;

    // transformations
    virtual void worldToScreen(const GEVector3* PositionWorld, GEVector3* PositionScreen) = 0;
    virtual void screenToWorld(const GEVector3* PositionScreen, GEVector3* WorldPointNear, GEVector3* WorldPointFar) = 0;

    // rendering
    virtual void renderBegin() = 0;
    void renderMesh(GEMesh* Mesh);
    void renderSprite(GESprite* Sprite);
    virtual void renderText(const char* Text, unsigned int Font, const GEColor& Color, unsigned char Opacity,
                            unsigned int Region, GEAlignment Alignment) = 0;
    virtual void renderEnd() = 0;
};
