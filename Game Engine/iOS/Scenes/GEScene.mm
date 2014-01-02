
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
    , dDeltaTime(0.0)
    , callbackScene(NULL)
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

void GEScene::inputKey(char Key)
{
}

void GEScene::inputMouse(int X, int Y)
{
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

void GEScene::setDeltaTime(double DeltaTime)
{
    dDeltaTime = DeltaTime;
}

void GEScene::setCallback(void (*function)(unsigned int NewScene))
{
    callbackScene = function;
}

int GEScene::getNextScene()
{
    return iNextScene;
}
