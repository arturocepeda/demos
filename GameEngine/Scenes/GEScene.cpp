
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Scene Management
//
//  --- GEScene.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GEScene.h"
#include <stdio.h>

GEScene::GEScene(GERendering* Render, GEAudio* Audio, void* GlobalData)
    : cRender(Render)
    , cAudio(Audio)
    , fDeltaTime(0.0f)
    , callbackScene(NULL)
    , iMouseX(0)
    , iMouseY(0)
    , iMouseLastX(0)
    , iMouseLastY(0)
{
}

void GEScene::sceneChange(int iNewScene)
{
    iNextScene = iNewScene;

    if(callbackScene)
        callbackScene(iNewScene);
}

void GEScene::init()
{
    iNextScene = INVALID_SCENE;
    internalInit();
}

void GEScene::inputKeyPress(char Key)
{
}

void GEScene::inputKeyRelease(char Key)
{
}

void GEScene::inputMouse(int X, int Y)
{
    iMouseLastX = iMouseX;
    iMouseLastY = iMouseY;
    iMouseX = X;
    iMouseY = Y;
}

void GEScene::inputMouseLeftButton()
{
}

void GEScene::inputMouseRightButton()
{
}

void GEScene::inputTouchBegin(int ID, const GEVector2& Point)
{
}

void GEScene::inputTouchMove(int ID, const GEVector2& PreviousPoint, const GEVector2& CurrentPoint)
{
}

void GEScene::inputTouchEnd(int ID, const GEVector2& Point)
{
}
   
void GEScene::updateAccelerometerStatus(const GEVector3& Status)
{
}

void GEScene::setDeltaTime(float DeltaTime)
{
    fDeltaTime = DeltaTime;
}

void GEScene::setCallback(void (*function)(unsigned int NewScene))
{
    callbackScene = function;
}

int GEScene::getNextScene()
{
    return iNextScene;
}
