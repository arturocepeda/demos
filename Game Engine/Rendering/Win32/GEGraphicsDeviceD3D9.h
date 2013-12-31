
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering (Direct3D 9)
//
//  --- GEGraphicsDeviceD3D9.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Rendering/GEGraphicsDevice.h"
#include <d3d9.h>
#include <d3dx9.h>

class GEGraphicsDeviceD3D9 : public GEGraphicsDevice
{
private:
    LPDIRECT3D9 d3d;
    LPDIRECT3DDEVICE9 d3ddev;

public:
    GEGraphicsDeviceD3D9(void* Window, bool Windowed, unsigned int ScreenSizeX, unsigned int ScreenSizeY);
    ~GEGraphicsDeviceD3D9();

    void* getNativePointer() override;
};
