
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering
//
//  --- GERendering.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GERendering.h"
#include <stdio.h>

GERendering::GERendering(void* Window, bool Windowed, unsigned int ScreenWidth, unsigned int ScreenHeight)
	: pWindow(Window)
	, bWindowed(Windowed)
	, iScreenWidth(ScreenWidth)
	, iScreenHeight(ScreenHeight)
{
    float fAspectRatio = (float)iScreenHeight / iScreenWidth;
    cPixelToScreenX = new GELine(0.0f, -1.0f, iScreenWidth, 1.0f);
    cPixelToScreenY = new GELine(0.0f, fAspectRatio, iScreenHeight, -fAspectRatio);
}

GERendering::~GERendering()
{
    delete cPixelToScreenX;
    delete cPixelToScreenY;
}

GEVector2 GERendering::pixelToScreen(const GEVector2& PixelPosition)
{
    return GEVector2((float)cPixelToScreenX->y(PixelPosition.X), (float)cPixelToScreenY->y(PixelPosition.Y));
}

void GERendering::setBackgroundColor(const GEColor& Color)
{
    cBackgroundColor = Color;
}

void GERendering::loadTexture(unsigned int TextureIndex, const char* Name)
{
}

void GERendering::loadTextureCompressed(unsigned int TextureIndex, const char* Name,
                                        unsigned int Size, unsigned int BPP, bool Alpha)
{
}

unsigned int GERendering::getTexture(unsigned int TextureIndex)
{
    return tTextures[TextureIndex];
}

GETextureSize& GERendering::getTextureSize(unsigned int TextureIndex)
{
    return tTextureSize[TextureIndex];
}

void GERendering::set2D(bool Portrait)
{
}

void GERendering::set3D(bool Portrait)
{
}

void GERendering::useShaderProgram(unsigned int iProgramIndex)
{
}

void GERendering::setAmbientLightColor(const GEColor& Color)
{
    sAmbientLight.Color = Color;
}

void GERendering::setAmbientLightIntensity(float Intensity)
{
    sAmbientLight.Intensity = Intensity;
}

void GERendering::setNumberOfActiveLights(unsigned int N)
{
    iNumberOfActiveLights = N;
}

void GERendering::moveLight(unsigned int LightIndex, const GEVector3& Delta)
{
    sLights[LightIndex].Position += Delta;
}

void GERendering::switchLight(unsigned int LightIndex, bool On)
{
    sLights[LightIndex].On = On;
}

void GERendering::releaseLight(unsigned int Light)
{
}

void GERendering::setLightPosition(unsigned int LightIndex, const GEVector3& Position)
{
    sLights[LightIndex].Position = Position;
}

void GERendering::setLightColor(unsigned int LightIndex, const GEColor& Color)
{
    sLights[LightIndex].Color = Color;
}

void GERendering::setLightIntensity(unsigned int LightIndex, float Intensity)
{
    sLights[LightIndex].Intensity = Intensity;
}

void GERendering::setLightRange(unsigned int LightIndex, float Range)
{
    sLights[LightIndex].Range = Range;
}

void GERendering::setLightDirection(unsigned int LightIndex, const GEVector3& Direction)
{
    sLights[LightIndex].Direction = Direction;
}

void GERendering::releaseMesh(GEMesh** Mesh)
{
    if(*Mesh)
    {
        delete *Mesh;
        *Mesh = NULL;
    }
}

void GERendering::releaseSprite(GESprite** Sprite)
{
    if(*Sprite)
    {
        delete *Sprite;
        *Sprite = NULL;
    }
}

void GERendering::releaseLabel(GELabel** Label)
{
    if(*Label)
    {
        delete *Label;
        *Label = NULL;
    }
}

void GERendering::useCamera(GECamera* Camera)
{
    Camera->use();
}

void GERendering::releaseCamera(GECamera** Camera)
{
    if(*Camera)
    {
        delete *Camera;
        *Camera = NULL;
    }
}

void GERendering::defineViewPort(unsigned int ViewPort, int X, int Y, int Width, int Height)
{
}

void GERendering::useViewPort(unsigned int ViewPort)
{
}

void GERendering::releaseViewPort(unsigned int ViewPort)
{
}

void GERendering::renderMesh(GEMesh* Mesh)
{
    Mesh->render();
}

void GERendering::renderSprite(GESprite* Sprite)
{
    Sprite->render();
}

void GERendering::renderLabel(GELabel* Label)
{
    Label->render();
}
