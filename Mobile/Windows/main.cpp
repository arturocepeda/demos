
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Windows (DirectX 11)
//
//  --- main.cpp ---
//
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "main.h"
#include "config.h"

#include "Core/GEDevice.h"
#include "Core/GETimer.h"
#include "Core/GETime.h"
#include "States/GEState.h"

#include "Rendering/DX11/GERenderSystemDX11.h"
#include "Audio/FMOD/GEAudioSystemFMOD.h"

#include "StateSample.h"

#pragma comment(lib, "GameEngine.DX11.lib")
#pragma comment(lib, "pugixml.Windows.lib")
#pragma comment(lib, "stb.Windows.lib")
#pragma comment(lib, "../../../GameEngine/Externals/FMOD/lib/fmodex_vc.lib")

using namespace GE::Core;
using namespace GE::States;
using namespace GE::Rendering;
using namespace GE::Audio;

char sAppName[] = "GameEngine";

// window
HWND hWnd;

// engine objects
RenderSystem* cRender;              // rendering system
AudioSystem* cAudio;                // audio system
bool bEnd;                          // loop ending flag

// states
std::vector<State*> cStates;
State* cCurrentState;

// mouse
POINT pMouse;
Line* cPixelToScreenX;
Line* cPixelToScreenY;
bool bMouseLeftButton = false;
GE::Vector2 vMouseLastPosition(0.0f, 0.0f);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR sCmdLine, int iCmdShow)
{
    MSG iMsg;
    WNDCLASSEX wndClass;
    
    // randomize
    srand((GE::uint)time(NULL));

    // screen size
    int iFullscreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int iFullscreenHeight = GetSystemMetrics(SM_CYSCREEN);

#ifdef _DEBUG
    Device::ScreenWidth = 400;
    Device::ScreenHeight = 600;
#else
    Device::ScreenWidth = iFullscreenWidth;
    Device::ScreenHeight = iFullscreenHeight;
#endif

    cPixelToScreenX = new Line(0.0f, -1.0f, Device::ScreenWidth, 1.0f);
    cPixelToScreenY = new Line(0.0f, Device::getAspectRatio(), Device::ScreenHeight, -Device::getAspectRatio());

    int iWindowWidth = Device::ScreenWidth;
    int iWindowHeight = Device::ScreenHeight;

    // class properties
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = sAppName;
    wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // register the class
    RegisterClassExA(&wndClass);

    // create the main window
    GE::uint iWindowPositionX = (iFullscreenWidth / 2) - (iWindowWidth / 2);
    GE::uint iWindowPositionY = (iFullscreenHeight / 2) - (iWindowHeight / 2);

    hWnd = CreateWindowExA(NULL, sAppName, "GameEngine", WS_CAPTION | WS_SYSMENU, iWindowPositionX, iWindowPositionY, 
                           Device::ScreenWidth, Device::ScreenHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    // initialize rendering and sound systems
#ifndef _DEBUG
    cRender = new RenderSystemDX11(hWnd, false);
#else
    cRender = new RenderSystemDX11(hWnd, true);
#endif
    cAudio = new AudioSystemFMOD();
    cAudio->init();
    cAudio->setListenerPosition(GE::Vector3(0.0f, 0.0f, 0.0f));

#ifndef _DEBUG
    // hide the mouse pointer
    ShowCursor(false);
#endif

    // timer
    Timer cTimer;
    cTimer.start();
    Time::reset();

    bEnd = false;

    double dTimeInterval = 1000000.0 / GE_FPS;
    double dTimeDelta = 0.0;
    double dTimeBefore = 0.0;
    double dTimeNow;

    bool bMousePositionSet;
    
    // create states
    cStates.push_back(new GEStateSample((void*)0));
    // ...
    // ...

    // select the first state 
    cCurrentState = 0;
    StateChange(0);

    // game loop
    while(!bEnd)
    {
        // input
        while(PeekMessage(&iMsg, NULL, 0, 0, PM_REMOVE)) 
        {
            if(iMsg.message == WM_CLOSE || iMsg.message == WM_QUIT) 
            {
                bEnd = true;
            }
            else 
            {
                TranslateMessage(&iMsg);
                DispatchMessage(&iMsg);
            }
        }

        GetCursorPos(&pMouse);
        bMousePositionSet = false;

        if(pMouse.x < GE_MOUSE_CHECK_MARGIN)
        {
            pMouse.x = iFullscreenWidth - GE_MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }
        else if(pMouse.x >= (iFullscreenWidth - GE_MOUSE_CHECK_MARGIN))
        {
            pMouse.x = GE_MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }
        if(pMouse.y < GE_MOUSE_CHECK_MARGIN)
        {
            pMouse.y = iFullscreenHeight - GE_MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }
        else if(pMouse.y >= (iFullscreenHeight - GE_MOUSE_CHECK_MARGIN))
        {
            pMouse.y = GE_MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }          

        if(bMousePositionSet)
        {
            SetCursorPos(pMouse.x, pMouse.y);
            cCurrentState->inputMouse(pMouse.x, pMouse.y);
        }

        if(bMouseLeftButton)
        {
           GE::Vector2 vMouseCurrentPosition = GetMouseScreenPosition();
           cCurrentState->inputTouchMove(0, vMouseLastPosition, vMouseCurrentPosition);
           vMouseLastPosition = vMouseCurrentPosition;
        }

        // update and render
        dTimeNow = cTimer.getTime();
        dTimeDelta = dTimeNow - dTimeBefore;

        if(dTimeDelta >= dTimeInterval)
        {
            dTimeBefore = dTimeNow;

            if(!cCurrentState)
                continue;

            Time::setDelta((float)dTimeDelta * 0.000001f);

            cCurrentState->inputMouse(pMouse.x, pMouse.y);
            cCurrentState->update();

            cRender->renderBegin();
            cRender->renderFrame();
            cRender->renderEnd();
            cRender->clearRenderingQueues();
        }
    }

    cCurrentState->release();

    for(GE::uint i = 0; i < cStates.size(); i++)
       delete cStates[i];

    cAudio->release();
    
    delete cAudio;
    delete cRender;

    delete cPixelToScreenX;
    delete cPixelToScreenY;

    return (int)iMsg.wParam;
}

GE::Vector2 GetMouseScreenPosition()
{
   RECT sRect;
   GetWindowRect(hWnd, &sRect);
   return GE::Vector2((float)cPixelToScreenX->y(pMouse.x - sRect.left), (float)cPixelToScreenY->y(pMouse.y - sRect.top));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    if(cCurrentState)
    {
        switch(iMsg)
        {
        case WM_CLOSE:
           bEnd = true;
           return 0;

        case WM_KEYDOWN:
            cCurrentState->inputKeyPress((char)wParam);
            return 0;

        case WM_KEYUP:
            cCurrentState->inputKeyRelease((char)wParam);
            return 0;

        case WM_LBUTTONDOWN:
            bMouseLeftButton = true;
            vMouseLastPosition = GetMouseScreenPosition();
            cCurrentState->inputMouseLeftButton();
            cCurrentState->inputTouchBegin(0, vMouseLastPosition);
            return 0;

        case WM_LBUTTONUP:
            bMouseLeftButton = false;
            vMouseLastPosition = GetMouseScreenPosition();
            cCurrentState->inputTouchEnd(0, vMouseLastPosition);
            return 0;

        case WM_RBUTTONDOWN:
            cCurrentState->inputMouseRightButton();
            return 0;
        }
    }
    
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void StateChange(GE::uint iNewState)
{
   if(cCurrentState)
      cCurrentState->release();

   cCurrentState = cStates[iNewState];
   cCurrentState->setCallback(StateChange);
   cCurrentState->init();
}
