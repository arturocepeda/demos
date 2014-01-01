
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda P�rez
//  Game Engine
//
//  Scene Management
//
//  --- GEScene.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Rendering/GERendering.h"
#include "Audio/GEAudio.h"

#define KEY_UP          38
#define KEY_DOWN        40
#define KEY_LEFT        37
#define KEY_RIGHT       39

#define KEY_ENTER       13
#define KEY_ESC         27
#define KEY_BACKSPACE   8
#define KEY_SHIFT       16
#define KEY_TAB         9
#define KEY_CAPSLOCK    20

#define KEY_INSERT      45
#define KEY_DELETE      46
#define KEY_HOME        36
#define KEY_END         35
#define KEY_PAGEUP      33
#define KEY_PAGEDOWN    34

class GEScene
{
protected:
    // rendering and audio system
    GERendering* cRender;
    GEAudio* cAudio;

    // delta time
    double dDeltaTime;

    // frame counter
    unsigned int iCurrentFrame;

    // scene management
    void (*callbackScene)(unsigned int iNewScene);

    // mouse position
    int iMouseX;
    int iMouseY;

    void sceneChange(unsigned int iNewScene);

public:
    GEScene(GERendering* Render, GEAudio* Audio, void* GlobalData);

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void release() = 0;

    virtual void inputKey(char Key);

    virtual void inputMouse(int X, int Y);
    virtual void inputMouseLeftButton();
    virtual void inputMouseRightButton();

    void setDeltaTime(double DeltaTime);
    void setCallback(void (*function)(unsigned int NewScene));
};
