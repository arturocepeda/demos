
/*
    Arturo Cepeda Pérez

    --- stateArrive.h ---
*/

#pragma once

#include "main.h"
#include "States/GEState.h"
#include "car.h"

class CStateArrive : public GE::States::State
{
private:
    // camera
    GE::Rendering::Camera* cCamera;
    float fPitch;
    float fYaw;

    bool bMovingForward;
    bool bMovingBackward;
    bool bMovingLeft;
    bool bMovingRight;

    // meshes
    GE::Rendering::Mesh* mMeshCar;
    GE::Rendering::Mesh* mMeshRoom;
    GE::Rendering::Mesh* mMeshTarget;

    // color
    GE::Color cColorOption;
    GE::Color cColorSelected;
    bool bColorSelectedInc;

    // fonts
    unsigned int iFontText;

    // labels
    GE::Rendering::Label* cLabelDebug;
    char sMessage[256];

    // data
    CCar cCar;
    GE::Vector3 vTargetPoint;
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
    CStateArrive(GE::Rendering::RenderSystem* Render, GE::Audio::AudioSystem* Audio, void* GlobalData);
    ~CStateArrive();

    void internalInit();
    void update(float DeltaTime);
    void render();
    void release();

    void inputKeyPress(char Key);
    void inputKeyRelease(char Key);
    void inputMouseLeftButton();
};
