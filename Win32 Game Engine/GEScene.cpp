
/*
    Arturo Cepeda Pérez

	Scene Management

    --- GEScene.cpp ---
*/

#include "GEScene.h"

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
