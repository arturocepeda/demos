
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

#include "Core/GEUtils.h"
#include "GEGraphicsDevice.h"
#include "GERenderingObjects.h"

#define TEXTURES 32


struct GETextureSize
{
    unsigned int Width;
    unsigned int Height;
};


namespace GELights
{
    enum
    {
        PointLight1,
        
        Count
    };
}


namespace GEShaderPrograms
{
    enum
    {
        HUD,
        Text,
        MeshColor,
        MeshTexture,
        
        Count
    };
}


class GERendering
{
protected:
    void* pWindow;
    bool bWindowed;
    unsigned int iScreenWidth;
    unsigned int iScreenHeight;

    GEGraphicsDevice* cDevice;
    GEColor cBackgroundColor;

    GELine* cPixelToScreenX;
    GELine* cPixelToScreenY;
    
    unsigned int tTextures[TEXTURES];
    GETextureSize tTextureSize[TEXTURES];
    
    unsigned int iNumberOfActiveLights;
    GELight sAmbientLight;
    GELight sLights[GELights::Count];
    
    virtual void setupProjectionMatrix() = 0;
    virtual void clearBuffers() = 0;

public:
    GERendering(void* Window, bool Windowed, unsigned int ScreenWidth, unsigned int ScreenHeight);
    virtual ~GERendering();

    // screen
    unsigned int getScreenWidth();
    unsigned int getScreenHeight();

    GEVector2 pixelToScreen(const GEVector2& PixelPosition);

    // background
    void setBackgroundColor(const GEColor& Color);
    
    // textures
    virtual void loadTexture(unsigned int TextureIndex, const char* Name);
    virtual void loadTextureCompressed(unsigned int TextureIndex, const char* Name,
                                       unsigned int Size, unsigned int BPP, bool Alpha = false);
    unsigned int getTexture(unsigned int TextureIndex);
    GETextureSize& getTextureSize(unsigned int TextureIndex);
    
    // rendering mode
    virtual void set2D(bool Portrait = true);
    virtual void set3D(bool Portrait = true);
    virtual void useShaderProgram(unsigned int iProgramIndex);
    
    // meshes
    virtual void createMesh(GEMesh** Mesh) = 0;
    void releaseMesh(GEMesh** Mesh);

    // sprites
    virtual void createSprite(GESprite** Sprite) = 0;
    void releaseSprite(GESprite** Sprite);
    
    // labels
    virtual void createLabel(GELabel** Label, unsigned int Font, GEAlignment Alignment,
                             unsigned int Width, unsigned int Height, const char* Text = "") = 0;
    void releaseLabel(GELabel** Label);

    // cameras
    virtual void createCamera(GECamera** Camera) = 0;
    virtual void useCamera(GECamera* Camera);
    void releaseCamera(GECamera** Camera);

    // lighting
    virtual void setAmbientLightColor(const GEColor& Color);
    virtual void setAmbientLightIntensity(float Intensity);

    virtual unsigned int createDirectionalLight(const GEColor& Color, float Range,
                                                const GEVector3& Direction) = 0;
    virtual unsigned int createPointLight(const GEColor& Color, float Range, float Attenuation,
                                          const GEVector3& Position) = 0;
    virtual unsigned int createSpotLight(const GEColor& Color, float Range, float Attenuation,
                                         const GEVector3& Position, const GEVector3& Direction,
                                         float Theta, float Phi, float Falloff) = 0;

    void setNumberOfActiveLights(unsigned int N);
    virtual void switchLight(unsigned int Light, bool On);
    virtual void moveLight(unsigned int Light, const GEVector3& Delta);
    virtual void releaseLight(unsigned int Light);
    
    virtual void setLightColor(unsigned int Light, const GEColor& Color);
    virtual void setLightRange(unsigned int Light, float Range);
    virtual void setLightIntensity(unsigned int LightIndex, float Intensity);
    virtual void setLightPosition(unsigned int Light, const GEVector3& Position);
    virtual void setLightDirection(unsigned int Light, const GEVector3& Direction);

    // view ports
    virtual void defineViewPort(unsigned int ViewPort, int X, int Y, int Width, int Height) {}
    virtual void useViewPort(unsigned int ViewPort) {}
    virtual void releaseViewPort(unsigned int ViewPort) {}

    // fonts
    virtual void defineFont(unsigned int Font, const char* FontName, float Size, unsigned int Width,
                            unsigned int Height, bool Bold = false, bool Italic = false) = 0;
    virtual void releaseFont(unsigned int Font) = 0;

    // transformations
    virtual void worldToScreen(const GEVector3* PositionWorld, GEVector3* PositionScreen) = 0;
    virtual void screenToWorld(const GEVector3* PositionScreen, GEVector3* WorldPointNear, GEVector3* WorldPointFar) = 0;

    // rendering
    virtual void renderBegin() = 0;
    virtual void renderMesh(GEMesh* Mesh);
    virtual void renderSprite(GESprite* Sprite);
    virtual void renderLabel(GELabel* Label);
    virtual void renderEnd() = 0;
};
