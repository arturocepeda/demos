
/*
    Arturo Cepeda Pérez

    --- stateFacing.cpp ---
*/

#include "stateFacing.h"
#include <stdio.h>
#include <cmath>

using namespace GE;
using namespace GE::States;
using namespace GE::Core;
using namespace GE::Rendering;
using namespace GE::Audio;

CStateFacing::CStateFacing(RenderSystem* Render, AudioSystem* Audio, void* GlobalData)
    : State(Render, Audio, GlobalData)
    , bMovingForward(false)
    , bMovingBackward(false)
    , bMovingLeft(false)
    , bMovingRight(false)
    , fDeltaTime(0.0f)
{
}

CStateFacing::~CStateFacing()
{
}

void CStateFacing::internalInit()
{
    initRenderObjects();
    initSoundObjects();
}

void CStateFacing::initRenderObjects()
{
    // lighting
    cRender->setAmbientLightColor(Color((byte)255, 255, 255));

    // camera
    cRender->createCamera(&cCamera);
    cCamera->setPosition(Vector3(-5.0f, 2.0f, 0.0f));
    fPitch = 0.5f;
    fYaw = -0.8f;

    // meshes
    cRender->createMesh(&mMeshGuy);
    cRender->createMesh(&mMeshRoom);

    mMeshGuy->loadFromFile("meshes\\guy.x", "textures\\");
    mMeshGuy->setPosition(0.0f, -1.4f, 0.0f);
    mMeshGuy->setScale(2.0f, 2.0f, 2.0f);

    mMeshRoom->loadFromFile("meshes\\room.x", "textures\\");
    mMeshRoom->setPosition(0.0f, 0.0f, 0.0f);

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

void CStateFacing::initSoundObjects()
{
}

void CStateFacing::update(float DeltaTime)
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

    // guy
    updateGuyDirection();

    // selected color animation
    cColorSelected.R += (bColorSelectedInc ? fDeltaTime : -fDeltaTime) * 0.001f;

    if(cColorSelected.R > 0.95f)
        bColorSelectedInc = false;
    else if(cColorSelected.R < 0.6f)
        bColorSelectedInc = true;

    cLabelDebug->setColor(cColorSelected);
}

void CStateFacing::render()
{
    cCamera->use();
    cRender->renderMesh(mMeshRoom);
    cRender->renderMesh(mMeshGuy);
    cRender->renderLabel(cLabelDebug);
}

void CStateFacing::release()
{
    releaseSoundObjects();
    releaseRenderObjects();
}

void CStateFacing::releaseRenderObjects()
{
    // camera
    cRender->releaseCamera(&cCamera);

    // fonts
    cRender->releaseFont(iFontText);

    // meshes
    delete mMeshGuy;
    delete mMeshRoom;

    // labels
    delete cLabelDebug;
}

void CStateFacing::releaseSoundObjects()
{
}

void CStateFacing::inputKeyPress(char Key)
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

void CStateFacing::inputKeyRelease(char Key)
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

void CStateFacing::moveCameraForward(float Quantity)
{
    float fCameraY = cCamera->getPosition().Y;
    cCamera->moveForward(Quantity);
    
    Vector3 vCameraNewPosition = cCamera->getPosition();
    cCamera->setPosition(Vector3(vCameraNewPosition.X, fCameraY, vCameraNewPosition.Z));
}

void CStateFacing::moveCameraMouse()
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

void CStateFacing::updateGuyDirection()
{
    static const float AngleNear = 0.0001f;
    static const float AngleStep = 2.0f;

    Vector3 vCameraPosition = cCamera->getPosition();
    Vector3 vGuyPosition = mMeshGuy->getPosition();

    Vector3 vGuyTargetForward = vCameraPosition - vGuyPosition;
    vGuyTargetForward.Y = 0.0f;
    vGuyTargetForward.normalize();

    float fTargetRotation = getSimplifiedAngle(-atan2f(vGuyTargetForward.Z, vGuyTargetForward.X) - GE_HALFPI);

    Vector3 vCurrentGuyRotation = mMeshGuy->getRotation();
    float fAngleDifference = getSimplifiedAngle(vCurrentGuyRotation.Y - fTargetRotation);

    sprintf(sMessage, "Angle: %.2f Rad\nTarget: %.2f Rad", vCurrentGuyRotation.Y, fTargetRotation);
    cLabelDebug->setText(sMessage);

    if(fabs(fAngleDifference) < AngleNear)
        return;

    vCurrentGuyRotation.Y = getSimplifiedAngle(vCurrentGuyRotation.Y - AngleStep * fAngleDifference * fDeltaTime);
    mMeshGuy->setRotation(vCurrentGuyRotation);
}
