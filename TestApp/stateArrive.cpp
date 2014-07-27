
/*
    Arturo Cepeda Pérez

    --- stateArrive.cpp ---
*/

#include "stateArrive.h"
#include <stdio.h>
#include <cmath>

using namespace GE;
using namespace GE::States;
using namespace GE::Rendering;
using namespace GE::Audio;

CStateArrive::CStateArrive(RenderSystem* Render, AudioSystem* Audio, void* GlobalData)
    : State(Render, Audio, GlobalData)
    , bMovingForward(false)
    , bMovingBackward(false)
    , bMovingLeft(false)
    , bMovingRight(false)
    , fDeltaTime(0.0f)
{
}

CStateArrive::~CStateArrive()
{
}

void CStateArrive::internalInit()
{
    initRenderObjects();
    initSoundObjects();
}

void CStateArrive::initRenderObjects()
{
    // lighting
    cRender->setAmbientLightColor(Color((byte)255, 255, 255));

    // camera
    cRender->createCamera(&cCamera);
    cCamera->setPosition(Vector3(-5.0f, 2.0f, 0.0f));
    fPitch = 0.5f;
    fYaw = -0.8f;

    // meshes
    cRender->createMesh(&mMeshRoom);
    cRender->createMesh(&mMeshCar);
    cRender->createMesh(&mMeshTarget);

    mMeshCar->loadFromFile("meshes\\van.x");
    mMeshCar->setPosition(0.0f, 0.0f, 0.0f);
    mMeshCar->setScale(0.25f, 0.25f, 0.25f);

    vTargetPoint = mMeshCar->getPosition();

    mMeshRoom->loadFromFile("meshes\\room.x", "textures\\");
    mMeshRoom->setPosition(0.0f, 1.5f, 0.0f);

    mMeshTarget->loadFromFile("meshes\\target.x");
    mMeshTarget->setVisible(false);

    // fonts
    iFontText = 0;
    cRender->defineFont(iFontText, "Courier New", 36.0f, true);

    // colors
    cColorOption.set((byte)140, 33, 27);
    cColorSelected.set((byte)252, 200, 200);
    bColorSelectedInc = false;

    // labels
    cRender->createLabel(&cLabelDebug, iFontText, Alignment::TopCenter, Vector2(1024.0f, 128.0f), "");
    cLabelDebug->setPosition(0.0f, 24.0f);
}

void CStateArrive::initSoundObjects()
{
}

void CStateArrive::update(float DeltaTime)
{
    fDeltaTime = DeltaTime;

    // camera
    if(bMovingForward)
        moveCameraForward(fDeltaTime * CAMERA_MOVE);
    if(bMovingBackward)
        moveCameraForward(-fDeltaTime * CAMERA_MOVE);
    if(bMovingLeft)
        cCamera->moveLeft(fDeltaTime * CAMERA_MOVE);
    if(bMovingRight)
        cCamera->moveLeft(-fDeltaTime * CAMERA_MOVE);
    moveCameraMouse();

    // car
    cCar.update(fDeltaTime, vTargetPoint);
    mMeshCar->setPosition(cCar.getPosition());
    mMeshCar->setRotation(0.0f, cCar.getAngle() + GE_PI, 0.0f);

    // selected color animation
    cColorSelected.R += (bColorSelectedInc ? fDeltaTime : -fDeltaTime) * 0.001f;

    if(cColorSelected.R > 0.95f)
        bColorSelectedInc = false;
    else if(cColorSelected.R < 0.6f)
        bColorSelectedInc = true;

    cLabelDebug->setColor(cColorSelected);
}

void CStateArrive::render()
{
    cCamera->use();
    cRender->renderMesh(mMeshRoom);
    cRender->renderMesh(mMeshCar);
    cRender->renderMesh(mMeshTarget);
    cRender->renderLabel(cLabelDebug);
}

void CStateArrive::release()
{
    releaseSoundObjects();
    releaseRenderObjects();
}

void CStateArrive::releaseRenderObjects()
{
    // camera
    cRender->releaseCamera(&cCamera);

    // fonts
    cRender->releaseFont(iFontText);

    // meshes
    cRender->releaseMesh(&mMeshCar);
    cRender->releaseMesh(&mMeshRoom);
    cRender->releaseMesh(&mMeshTarget);

    // labels
    cRender->releaseLabel(&cLabelDebug);
}

void CStateArrive::releaseSoundObjects()
{
}

void CStateArrive::inputKeyPress(char Key)
{
    switch(Key)
    {
    case KEY_ESC:
        stateChange(0);
        break;
        
    case 'W':
        bMovingForward = true;
        break;

    case 'S':
        bMovingBackward = true;
        break;

    case 'A':
        bMovingLeft = true;
        break;

    case 'D':
        bMovingRight = true;
        break;

    case KEY_UP:
        break;
        
    case KEY_DOWN:
        break;
    }
}

void CStateArrive::inputKeyRelease(char Key)
{
    switch(Key)
    {
    case 'W':
        bMovingForward = false;
        break;

    case 'S':
        bMovingBackward = false;
        break;

    case 'A':
        bMovingLeft = false;
        break;

    case 'D':
        bMovingRight = false;
        break;
    }
}

void CStateArrive::inputMouseLeftButton()
{
    vTargetPoint = Vector3(getRand() * 5.0f, 0.0f, getRand() * 5.0f);

    mMeshTarget->setPosition(vTargetPoint);
    mMeshTarget->setVisible(true);
}

float CStateArrive::getRand()
{
    return (float)rand() / RAND_MAX + (float)rand() / RAND_MAX - 1.0f;
}

void CStateArrive::moveCameraForward(float Quantity)
{
    cCamera->moveForward(Quantity);
}

void CStateArrive::moveCameraMouse()
{
    fYaw += (iMouseX - iMouseLastX) * fDeltaTime * CAMERA_ROTATE;
    fPitch -= (iMouseY - iMouseLastY) * fDeltaTime * CAMERA_ROTATE;

    fPitch = __max(-CAMERA_PITCH_LIMIT, fPitch);
    fPitch = __min(+CAMERA_PITCH_LIMIT, fPitch);

    float fVertical = sinf(fPitch);
    float fInThePlane = cosf(fPitch);
    float fForwardBack = fInThePlane * cosf(fYaw);
    float fLeftRight = fInThePlane * sinf(fYaw);

    cCamera->lookAt(cCamera->getPosition() + Vector3(fLeftRight, fVertical, fForwardBack));
}
