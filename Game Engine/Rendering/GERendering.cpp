
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

GERendering::GERendering(void* Window, bool Windowed, unsigned int ScreenSizeX, unsigned int ScreenSizeY)
	: pWindow(Window)
	, bWindowed(Windowed)
	, iScreenSizeX(ScreenSizeX)
	, iScreenSizeY(ScreenSizeY)
{
}

GERendering::~GERendering()
{
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
