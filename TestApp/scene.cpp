
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- sceneMenu.cpp ---
*/

#include "scene.h"
#include <stdio.h>

#define PI 3.141592f

CScene::CScene(GERendering* Render, GEAudio* Audio, void* GlobalData)
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

CScene::~CScene()
{
}

void CScene::internalInit()
{
    initRenderObjects();
    initSoundObjects();
}

void CScene::initRenderObjects()
{
    // lighting
    cRender->setAmbientLightColor(GEColor((byte)100, 100, 100));

    // camera
    cRender->createCamera(&cCamera);
    cCamera->setPosition(GEVector3(-5.0f, 2.0f, 0.0f));
    fPitch = 0.5f;
    fYaw = -0.8f;

    // meshes (background)
    cRender->createMesh(&mMeshMallet1);
    cRender->createMesh(&mMeshMallet2);
    cRender->createMesh(&mMeshTable);
    cRender->createMesh(&mMeshRoom);

    mMeshMallet1->loadFromFile("meshes\\mallet.x");
    mMeshMallet2->loadFromFile("meshes\\mallet.x");
    mMeshTable->loadFromFile("meshes\\table.x", "textures\\");
    mMeshRoom->loadFromFile("meshes\\room.x", "textures\\");

    // fonts
    iFontText = 0;
    iFontOption = 1;
    iFontSelected = 2;

    cRender->defineFont(iFontText, "Courier New", 36.0f, true);
    cRender->defineFont(iFontOption, "Courier New", 60.0f, true);
    cRender->defineFont(iFontSelected, "Courier New", 65.0f, true);

    // colors
    cColorOption.set((byte)140, 33, 27);
    cColorSelected.set((byte)252, 200, 200);
    bColorSelectedInc = false;
}

void CScene::initSoundObjects()
{
}

void CScene::update()
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

    // selected color animation
    cColorSelected.R += (bColorSelectedInc ? fDeltaTime : -fDeltaTime) * 0.001f;

    if(cColorSelected.R > 0.95f)
        bColorSelectedInc = false;
    else if(cColorSelected.R < 0.6f)
        bColorSelectedInc = true;
}

void CScene::render()
{
    cCamera->use();

    mMeshRoom->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshRoom);

    mMeshTable->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshTable);
}

void CScene::release()
{
    releaseSoundObjects();
    releaseRenderObjects();
}

void CScene::releaseRenderObjects()
{
    // camera
    cRender->releaseCamera(&cCamera);

    // fonts
    cRender->releaseFont(iFontText);
    cRender->releaseFont(iFontOption);
    cRender->releaseFont(iFontSelected);

    // meshes
    delete mMeshMallet1;
    delete mMeshMallet2;
    delete mMeshTable;
    delete mMeshRoom;
}

void CScene::releaseSoundObjects()
{
}

void CScene::inputKeyPress(char Key)
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

void CScene::inputKeyRelease(char Key)
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

void CScene::moveCameraForward(float Quantity)
{
    float fCameraY = cCamera->getPosition().Y;
    cCamera->moveForward(Quantity);
    
    GEVector3 vCameraNewPosition = cCamera->getPosition();
    cCamera->setPosition(GEVector3(vCameraNewPosition.X, fCameraY, vCameraNewPosition.Z));
}

void CScene::moveCameraMouse()
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
