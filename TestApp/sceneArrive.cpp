
/*
    Arturo Cepeda Pérez

    --- sceneArrive.cpp ---
*/

#include "sceneArrive.h"
#include <stdio.h>
#include <cmath>

CSceneArrive::CSceneArrive(GERendering* Render, GEAudio* Audio, void* GlobalData)
    : GEScene(Render, Audio, GlobalData)
    , bMovingForward(false)
    , bMovingBackward(false)
    , bMovingLeft(false)
    , bMovingRight(false)
{
    cRender = Render;
    cAudio = Audio;
    sGlobal = (SGlobal*)GlobalData;
}

CSceneArrive::~CSceneArrive()
{
}

void CSceneArrive::internalInit()
{
    initRenderObjects();
    initSoundObjects();
}

void CSceneArrive::initRenderObjects()
{
    // lighting
    cRender->setAmbientLightColor(GEColor((byte)255, 255, 255));

    // camera
    cRender->createCamera(&cCamera);
    cCamera->setPosition(GEVector3(-5.0f, 2.0f, 0.0f));
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
    cRender->createLabel(&cLabelDebug, iFontText, GEAlignment::TopCenter, 1024, 128, "");
    cLabelDebug->setPosition(0.0f, 24.0f);
}

void CSceneArrive::initSoundObjects()
{
}

void CSceneArrive::update()
{
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
    mMeshCar->setRotation(0.0f, cCar.getAngle() + PI, 0.0f);

    // selected color animation
    cColorSelected.R += (bColorSelectedInc ? fDeltaTime : -fDeltaTime) * 0.001f;

    if(cColorSelected.R > 0.95f)
        bColorSelectedInc = false;
    else if(cColorSelected.R < 0.6f)
        bColorSelectedInc = true;

    cLabelDebug->setColor(cColorSelected);
}

void CSceneArrive::render()
{
    cCamera->use();
    cRender->renderMesh(mMeshRoom);
    cRender->renderMesh(mMeshCar);
    cRender->renderMesh(mMeshTarget);
    cRender->renderLabel(cLabelDebug);
}

void CSceneArrive::release()
{
    releaseSoundObjects();
    releaseRenderObjects();
}

void CSceneArrive::releaseRenderObjects()
{
    // camera
    cRender->releaseCamera(&cCamera);

    // fonts
    cRender->releaseFont(iFontText);

    // meshes
    delete mMeshCar;
    delete mMeshRoom;
    delete mMeshTarget;

    // labels
    delete cLabelDebug;
}

void CSceneArrive::releaseSoundObjects()
{
}

void CSceneArrive::inputKeyPress(char Key)
{
    switch(Key)
    {
    case KEY_ESC:
        SceneChange(0);
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

void CSceneArrive::inputKeyRelease(char Key)
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

void CSceneArrive::inputMouseLeftButton()
{
    vTargetPoint = GEVector3(getRand() * 5.0f, 0.0f, getRand() * 5.0f);

    mMeshTarget->setPosition(vTargetPoint);
    mMeshTarget->setVisible(true);
}

float CSceneArrive::getRand()
{
    return (float)rand() / RAND_MAX + (float)rand() / RAND_MAX - 1.0f;
}

void CSceneArrive::moveCameraForward(float Quantity)
{
    cCamera->moveForward(Quantity);
}

void CSceneArrive::moveCameraMouse()
{
    fYaw += (iMouseX - iMouseLastX) * fDeltaTime * CAMERA_ROTATE;
    fPitch -= (iMouseY - iMouseLastY) * fDeltaTime * CAMERA_ROTATE;

    fPitch = __max(-CAMERA_PITCH_LIMIT, fPitch);
    fPitch = __min(+CAMERA_PITCH_LIMIT, fPitch);

    float fVertical = sinf(fPitch);
    float fInThePlane = cosf(fPitch);
    float fForwardBack = fInThePlane * cosf(fYaw);
    float fLeftRight = fInThePlane * sinf(fYaw);

    cCamera->lookAt(cCamera->getPosition() + GEVector3(fLeftRight, fVertical, fForwardBack));
}
