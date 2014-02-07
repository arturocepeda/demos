
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- sceneMenu.h ---
*/

#pragma once

#include "main.h"
#include "Scenes/GEScene.h"

#define CAMERA_MOVE         0.005f
#define CAMERA_ROTATE       0.00005f
#define CAMERA_PITCH_LIMIT  1.05f

class CScene : public GEScene
{
private:
    GERendering* cRender;
    GEAudio* cAudio;

    // global data
    SGlobal* sGlobal;

    // camera
    GECamera* cCamera;
    float fPitch;
    float fYaw;

    bool bMovingForward;
    bool bMovingBackward;
    bool bMovingLeft;
    bool bMovingRight;

    // meshes (background)
    GEMesh* mMeshMallet1;
    GEMesh* mMeshMallet2;
    GEMesh* mMeshTable;
    GEMesh* mMeshRoom;

    // color
    GEColor cColorOption;
    GEColor cColorSelected;
    bool bColorSelectedInc;

    // fonts
    unsigned int iFontOption;
    unsigned int iFontSelected;
    unsigned int iFontText;

    void playSounds();

    void initRenderObjects();
    void releaseRenderObjects();

    void initSoundObjects();
    void releaseSoundObjects();

    void moveCameraForward(float Quantity);
    void moveCameraMouse();

public:
    CScene(GERendering* Render, GEAudio* Audio, void* GlobalData);
    ~CScene();

    void internalInit();
    void update();
    void render();
    void release();

    void inputKeyPress(char Key);
    void inputKeyRelease(char Key);
};
