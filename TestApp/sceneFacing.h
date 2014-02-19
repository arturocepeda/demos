
/*
    Arturo Cepeda Pérez

    --- sceneFacing.h ---
*/

#pragma once

#include "main.h"
#include "Scenes/GEScene.h"

class CSceneFacing : public GEScene
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

    // meshes
    GEMesh* mMeshGuy;
    GEMesh* mMeshRoom;

    // color
    GEColor cColorOption;
    GEColor cColorSelected;
    bool bColorSelectedInc;

    // fonts
    unsigned int iFontText;

    // labels
    GELabel* cLabelDebug;
    char sMessage[256];

    // data
    GEVector3 vGuyForward;

    void playSounds();

    void initRenderObjects();
    void releaseRenderObjects();

    void initSoundObjects();
    void releaseSoundObjects();

    void moveCameraForward(float Quantity);
    void moveCameraMouse();

    void updateGuyDirection();

public:
    CSceneFacing(GERendering* Render, GEAudio* Audio, void* GlobalData);
    ~CSceneFacing();

    void internalInit();
    void update();
    void render();
    void release();

    void inputKeyPress(char Key);
    void inputKeyRelease(char Key);
};
