
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering (Direct3D 9)
//
//  --- GEGraphicsDeviceD3D9.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GEGraphicsDeviceD3D9.h"

GEGraphicsDeviceD3D9::GEGraphicsDeviceD3D9(void* Window, bool Windowed, unsigned int ScreenSizeX, unsigned int ScreenSizeY)
{
    HWND hWnd = (HWND)Window;

    // Direct3D system
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    // Direct3D device
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Windowed = Windowed;

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    //d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
    d3dpp.MultiSampleQuality = 0;

    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = ScreenSizeX;
    d3dpp.BackBufferHeight = ScreenSizeY;

    HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
                                   D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

    if(FAILED(hr))
    {
        MessageBox(hWnd, "Couldn't initialize Direct3D", "Error", 0);
        exit(1);
    }
}

GEGraphicsDeviceD3D9::~GEGraphicsDeviceD3D9()
{
    d3ddev->Release();
    d3d->Release();
}

void* GEGraphicsDeviceD3D9::getNativePointer()
{
    return d3ddev;
}
