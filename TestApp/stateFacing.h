
/*
    Arturo Cepeda Pérez

    --- stateFacing.h ---
*/

#pragma once

#include "main.h"
#include "States/GEState.h"

class CStateFacing : public GEState
{
private:
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
    float fDeltaTime;

    void playSounds();

    void initRenderObjects();
    void releaseRenderObjects();

    void initSoundObjects();
    void releaseSoundObjects();

    void moveCameraForward(float Quantity);
    void moveCameraMouse();

    void updateGuyDirection();

public:
    CStateFacing(GERendering* Render, GEAudio* Audio, void* GlobalData);
    ~CStateFacing();

    void internalInit();
    void update(float DeltaTime);
    void render();
    void release();

    void inputKeyPress(char Key);
    void inputKeyRelease(char Key);
};
