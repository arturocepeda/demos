
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

#include "sceneMenu.h"
#include "sceneMatch.h"

#pragma comment(lib, "..\\GameEngine\\GameEngine.D3D9.lib")
#pragma comment(lib, "..\\..\\SDK\\DirectX\\Lib\\x86\\d3d9.lib")
#pragma comment(lib, "..\\..\\SDK\\DirectX\\Lib\\x86\\d3dx9.lib")
#pragma comment(lib, "..\\..\\SDK\\FMOD\\api\\lib\\fmodex_vc.lib")
#pragma comment(lib, "ws2_32.lib")

SGlobal sGlobal;                    // global data
GERendering* cRender;               // rendering system
GEAudio* cAudio;                    // audio system
bool bEnd;                          // loop ending flag

// scenes
GEScene* cCurrentScene;
CSceneMenu* cSceneMenu;
CSceneMatch* cSceneMatch;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR sCmdLine, int iCmdShow)
{
    static char sAppName[] = "AirHockey3d";
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
    hWnd = CreateWindowExA(NULL, sAppName, "Air Hockey", WS_EX_TOPMOST | WS_POPUP, 0, 0, 
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
    sGlobal.iFPS = 72;

    // set default settings
    DefaultSettings();

    // timer
    GETimer* cTimer = new GETimerWin32();
    cTimer->start();

    bEnd = false;

    double dTimeInterval = 1000000.0 / sGlobal.iFPS;
    double dTimeDelta = 0.0;
    double dTimeBefore = 0.0;
    double dTimeNow;
    
    // initialize scenes
    cCurrentScene = NULL;
    SceneChange(SCENE_MENU);

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
        ScreenToClient(hWnd, &pMouse);            

        // update and render
        dTimeNow = cTimer->getTime();
        dTimeDelta = dTimeNow - dTimeBefore;

        if(dTimeDelta >= dTimeInterval)
        {
            dTimeBefore = dTimeNow;

            if(cCurrentScene)
            {
                cCurrentScene->setDeltaTime((float)dTimeDelta * 0.001f);
                cCurrentScene->inputMouse(pMouse.x, pMouse.y);
                cCurrentScene->update();

                cRender->renderBegin();
                cCurrentScene->render();
                cRender->renderEnd();
            }
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

    // menu
    case SCENE_MENU:
        cSceneMenu = new CSceneMenu(cRender, cAudio, &sGlobal);
        cCurrentScene = (GEScene*)cSceneMenu;
        break;

    // match
    case SCENE_MATCH:
        cSceneMatch = new CSceneMatch(cRender, cAudio, &sGlobal);
        cCurrentScene = (GEScene*)cSceneMatch;
        break;
    }

    if(!bEnd)
    {
        cCurrentScene->setCallback(SceneChange);
        cCurrentScene->init();
    }
}

void DefaultSettings()
{
    // game settings
    sGlobal.iGameMaxGoals = 7;
    sGlobal.iGameMaxMinutes = 2;
    
    sGlobal.bSplit = false;
    sGlobal.bTableAir = true;
    sGlobal.bAutomaticReplay = true;

    // multiplayer settings
    sGlobal.bMultiplayer = false;
    sGlobal.bServer = true;
    sGlobal.iPort = 1200;
    strcpy(sGlobal.sIPServer, "192.168.0.1");

    // AI level
    sGlobal.cPlayer2AILevel.DefaultSpeed = 10.0f;
    sGlobal.cPlayer2AILevel.SpeedForShooting = 120.0f;
    sGlobal.cPlayer2AILevel.SpeedForDefending = 11.0f;
    sGlobal.cPlayer2AILevel.SpeedForClearing = 80.0f;
    sGlobal.cPlayer2AILevel.AttackThresholdSpeed = 60.0f;

    // Kinect settings
    sGlobal.fKinectWidth = 250.0f;
    sGlobal.fKinectMinimumDistance = 1200.0f;
    sGlobal.fKinectMaximumDistance = 1600.0f;
    sGlobal.bPlayer1RightHanded = true;
    sGlobal.bPlayer2RightHanded = true;
}
