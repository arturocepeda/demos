
/*
    Arturo Cepeda Pérez

	Rendering Engine (Direct3D 9)

    --- GERendering.h ---
*/

// Library links: d3d9.lib, d3dx9.lib

#ifndef _GERENDERING_H_
#define _GERENDERING_H_

#include "GERenderingObjects.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#define CAMERAS     16
#define VIEWPORTS   16
#define REGIONS     16
#define FONTS       16

#define ALIGN_TOP_LEFT      1
#define ALIGN_TOP_CENTER    2
#define ALIGN_TOP_RIGHT     3
#define ALIGN_CENTER_LEFT   4
#define ALIGN_CENTER_CENTER 5
#define ALIGN_CENTER_RIGHT  6
#define ALIGN_BOTTOM_LEFT   7
#define ALIGN_BOTTOM_CENTER 8
#define ALIGN_BOTTOM_RIGHT  9

class GERendering
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;

    unsigned int iScreenSizeX;
    unsigned int iScreenSizeY;

    unsigned int iNumLights;

    D3DVIEWPORT9* vViewPorts[VIEWPORTS];
    GECamera* GECameras[CAMERAS];
    LPRECT rRegions[REGIONS];
    LPD3DXFONT fFonts[FONTS];

    void setupProjectionMatrix();
    void clearBuffers();

public:
	GERendering(HWND DeviceWindow, bool Windowed, unsigned int ScreenSizeX, unsigned int ScreenSizeY);
	~GERendering();

    // meshes
    void createMesh(GEMesh** Mesh);
    void releaseMesh(GEMesh** Mesh);

    // sprites
    void createSprite(GESprite** Sprite);
    void releaseSprite(GESprite** Sprite);

    // cameras
    void createCamera(GECamera** Camera);
    void releaseCamera(GECamera** Camera);

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

    // rendering
	void renderBegin();
    void renderMesh(GEMesh* Mesh);
    void renderSprite(GESprite* Sprite);
    void renderText(const char* Text, unsigned int Font, const GEColor& Color, unsigned char Opacity,
                    unsigned int Region, unsigned int Alignment);
    void renderEnd();
};

#endif
