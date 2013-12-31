
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
{
    cRender = Render;
    cAudio = Audio;
    callbackScene = NULL;
}

void GEScene::sceneChange(unsigned int iNewScene)
{
    if(callbackScene)
        callbackScene(iNewScene);
}

void GEScene::inputMouse(int X, int Y)
{
    iMouseX = X;
    iMouseY = Y;
}

void GEScene::setCallback(void (*function)(unsigned int NewScene))
{
    callbackScene = function;
}
