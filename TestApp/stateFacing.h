
/*
    Arturo Cepeda Pérez

    --- stateFacing.h ---
*/

#pragma once

#include "main.h"
#include "States/GEState.h"

class CStateFacing : public GE::States::State
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
    GE::Rendering::Mesh* mMeshGuy;
    GE::Rendering::Mesh* mMeshRoom;

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
    GE::Vector3 vGuyForward;
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
   CStateFacing(GE::Rendering::RenderSystem* Render, GE::Audio::AudioSystem* Audio, void* GlobalData);
    ~CStateFacing();

    void internalInit();
    void update(float DeltaTime);
    void render();
    void release();

    void inputKeyPress(char Key);
    void inputKeyRelease(char Key);
};
