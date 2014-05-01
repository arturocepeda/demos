
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- main.cpp ---
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "main.h"

#include "Core/GETimer.h"
#include "Rendering/GERendering.h"
#include "Audio/GEAudio.h"
#include "Scenes/GEScene.h"

#include "Core/Win32/GETimerWin32.h"
#include "Rendering/Direct3D/GERenderingD3D9.h"
#include "Audio/FMOD/GEAudioFMOD.h"

#include "sceneFacing.h"
#include "sceneArrive.h"

#pragma comment(lib, "..\\GameEngine\\GameEngine.D3D9.lib")
#pragma comment(lib, "..\\..\\SDK\\DirectX\\Lib\\x86\\d3d9.lib")
#pragma comment(lib, "..\\..\\SDK\\DirectX\\Lib\\x86\\d3dx9.lib")
#pragma comment(lib, "..\\..\\SDK\\FMOD\\api\\lib\\fmodex_vc.lib")

SGlobal sGlobal;                    // global data
GERendering* cRender;               // rendering system
GEAudio* cAudio;                    // audio system
bool bEnd;                          // loop ending flag

// scenes
GEScene* cCurrentScene;
CSceneFacing* cSceneFacing;
CSceneArrive* cSceneArrive;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR sCmdLine, int iCmdShow)
{
    static char sAppName[] = "Test";
    HWND hWnd;
    MSG iMsg;
    WNDCLASSEX wndClass;
    POINT pMouse;
    
    // randomize
    srand((unsigned int)time(NULL));

    // screen size
    sGlobal.ScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
    sGlobal.ScreenSizeY = GetSystemMetrics(SM_CYSCREEN);

#ifdef _DEBUG
    sGlobal.ScreenSizeX = 1024;
    sGlobal.ScreenSizeY = 768;
#endif

    // class properties
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;  // full screen mode
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = sAppName;
    wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // register the class
    RegisterClassExA(&wndClass);

    // create the main window (for full screen mode)
    hWnd = CreateWindowExA(NULL, sAppName, "Test", WS_CAPTION, 0, 0, 
                           sGlobal.ScreenSizeX, sGlobal.ScreenSizeY, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    // keep the window's handler
    sGlobal.hWnd = hWnd;

    // initialize rendering and sound systems
#ifndef _DEBUG
    cRender = new GERenderingD3D9(hWnd, false, sGlobal.ScreenSizeX, sGlobal.ScreenSizeY);
#else
    cRender = new GERenderingD3D9(hWnd, true, sGlobal.ScreenSizeX, sGlobal.ScreenSizeY);
#endif
    cAudio = new GEAudioFMOD();
    cAudio->init();
    cAudio->setListenerPosition(GEVector3(0.0f, 0.0f, 0.0f));

    // hide the mouse pointer
    ShowCursor(false);

    // update frequency
    sGlobal.iFPS = 60;

    // timer
    GETimer* cTimer = new GETimerWin32();
    cTimer->start();

    bEnd = false;

    double dTimeInterval = 1000000.0 / sGlobal.iFPS;
    double dTimeDelta = 0.0;
    double dTimeBefore = 0.0;
    double dTimeNow;

    bool bMousePositionSet;
    
    // initialize scenes
    cCurrentScene = NULL;
    SceneChange(SCENE_ARRIVE);

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

        if(pMouse.x < MOUSE_CHECK_MARGIN)
        {
            pMouse.x = sGlobal.ScreenSizeX - MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }
        else if(pMouse.x >= (sGlobal.ScreenSizeX - MOUSE_CHECK_MARGIN))
        {
            pMouse.x = MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }
        if(pMouse.y < MOUSE_CHECK_MARGIN)
        {
            pMouse.y = sGlobal.ScreenSizeY - MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }
        else if(pMouse.y >= (sGlobal.ScreenSizeY - MOUSE_CHECK_MARGIN))
        {
            pMouse.y = MOUSE_SET_MARGIN;
            bMousePositionSet = true;
        }          

        if(bMousePositionSet)
        {
            SetCursorPos(pMouse.x, pMouse.y);
            cCurrentScene->inputMouse(pMouse.x, pMouse.y);
        }

        // update and render
        dTimeNow = cTimer->getTime();
        dTimeDelta = dTimeNow - dTimeBefore;

        if(dTimeDelta >= dTimeInterval)
        {
            dTimeBefore = dTimeNow;

            if(!cCurrentScene)
                continue;

            cCurrentScene->setDeltaTime((float)dTimeDelta * 0.000001f);
            cCurrentScene->inputMouse(pMouse.x, pMouse.y);
            cCurrentScene->update();

            cRender->renderBegin();
            cCurrentScene->render();
            cRender->renderEnd();
        }
    }

    cAudio->release();
    
    delete cAudio;
    delete cRender;
    delete cTimer;

    return (int)iMsg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    if(cCurrentScene)
    {
        switch(iMsg)
        {
        case WM_KEYDOWN:
            cCurrentScene->inputKeyPress((char)wParam);
            return 0;

        case WM_KEYUP:
            cCurrentScene->inputKeyRelease((char)wParam);
            return 0;

        case WM_LBUTTONDOWN:
            cCurrentScene->inputMouseLeftButton();
            return 0;

        case WM_RBUTTONDOWN:
            cCurrentScene->inputMouseRightButton();
            return 0;
        }
    }
    
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void SceneChange(unsigned int iNewScene)
{
    if(cCurrentScene)
    {
        cCurrentScene->release();
        delete cCurrentScene;
        cCurrentScene = NULL;
    }

    switch(iNewScene)
    {
    // exit
    case 0:             
        bEnd = true;
        break;

    // facing
    case SCENE_FACING:
        cSceneFacing = new CSceneFacing(cRender, cAudio, &sGlobal);
        cCurrentScene = (GEScene*)cSceneFacing;
        break;

    // arrive
    case SCENE_ARRIVE:
        cSceneArrive = new CSceneArrive(cRender, cAudio, &sGlobal);
        cCurrentScene = (GEScene*)cSceneArrive;
        break;
    }

    if(!bEnd)
    {
        cCurrentScene->setCallback(SceneChange);
        cCurrentScene->init();
    }
}
