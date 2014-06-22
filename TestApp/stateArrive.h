
/*
    Arturo Cepeda Pérez

    --- stateArrive.h ---
*/

#pragma once

#include "main.h"
#include "States/GEState.h"
#include "car.h"

class CStateArrive : public GEState
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
    GEMesh* mMeshCar;
    GEMesh* mMeshRoom;
    GEMesh* mMeshTarget;

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
    CCar cCar;
    GEVector3 vTargetPoint;
    float fDeltaTime;

    void playSounds();

    void initRenderObjects();
    void releaseRenderObjects();

    void initSoundObjects();
    void releaseSoundObjects();

    void moveCameraForward(float Quantity);
    void moveCameraMouse();

    float getRand();

public:
    CStateArrive(GERendering* Render, GEAudio* Audio, void* GlobalData);
    ~CStateArrive();

    void internalInit();
    void update(float DeltaTime);
    void render();
    void release();

    void inputKeyPress(char Key);
    void inputKeyRelease(char Key);
    void inputMouseLeftButton();
};
