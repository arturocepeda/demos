
/*
    Arturo Cepeda Pérez

    --- sceneArrive.h ---
*/

#pragma once

#include "main.h"
#include "Scenes/GEScene.h"
#include "car.h"

class CSceneArrive : public GEScene
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

    void playSounds();

    void initRenderObjects();
    void releaseRenderObjects();

    void initSoundObjects();
    void releaseSoundObjects();

    void moveCameraForward(float Quantity);
    void moveCameraMouse();

    float getRand();

public:
    CSceneArrive(GERendering* Render, GEAudio* Audio, void* GlobalData);
    ~CSceneArrive();

    void internalInit();
    void update();
    void render();
    void release();

    void inputKeyPress(char Key);
    void inputKeyRelease(char Key);
    void inputMouseLeftButton();
};
