
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

void GERendering::releaseCamera(GECamera** Camera)
{
    if(*Camera)
    {
        delete *Camera;
        *Camera = NULL;
    }
}

void GERendering::renderMesh(GEMesh* Mesh)
{
    Mesh->render();
}

void GERendering::renderSprite(GESprite* Sprite)
{
    Sprite->render();
}
