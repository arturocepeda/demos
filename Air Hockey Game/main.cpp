
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
#include "States/GEState.h"

#include "Rendering/Direct3D/GERenderingD3D9.h"
#include "Audio/FMOD/GEAudioFMOD.h"

#include "stateMenu.h"
#include "stateMatch.h"

#pragma comment(lib, "..\\..\\GameEngine\\GameEngine.D3D9.lib")
#pragma comment(lib, "..\\..\\SDK\\DirectX\\Lib\\x86\\d3d9.lib")
#pragma comment(lib, "..\\..\\SDK\\DirectX\\Lib\\x86\\d3dx9.lib")
#pragma comment(lib, "..\\..\\SDK\\FMOD\\api\\lib\\fmodex_vc.lib")
#pragma comment(lib, "ws2_32.lib")

SGlobal sGlobal;                    // global data
GERendering* cRender;               // rendering system
GEAudio* cAudio;                    // audio system
bool bEnd;                          // loop ending flag

// states
GEState* cCurrentState;
CStateMenu* cStateMenu;
CStateMatch* cStateMatch;

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
    GETimer cTimer;
    cTimer.start();

    bEnd = false;

    double dTimeInterval = 1000000.0 / sGlobal.iFPS;
    double dTimeDelta = 0.0;
    double dTimeBefore = 0.0;
    double dTimeNow;
    
    // initialize states
    cCurrentState = NULL;
    StateChange(STATE_MENU);

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
        dTimeNow = cTimer.getTime();
        dTimeDelta = dTimeNow - dTimeBefore;

        if(dTimeDelta >= dTimeInterval)
        {
            dTimeBefore = dTimeNow;

            if(cCurrentState)
            {
                cCurrentState->inputMouse(pMouse.x, pMouse.y);
                cCurrentState->update((float)dTimeDelta * 0.001f);

                cRender->renderBegin();
                cCurrentState->render();
                cRender->renderEnd();
            }
        }
    }

    cAudio->release();
    
    delete cAudio;
    delete cRender;

    return (int)iMsg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    if(cCurrentState)
    {
        switch(iMsg)
        {
        case WM_KEYDOWN:
            cCurrentState->inputKeyPress((char)wParam);
            return 0;

        case WM_LBUTTONDOWN:
            cCurrentState->inputMouseLeftButton();
            return 0;

        case WM_RBUTTONDOWN:
            cCurrentState->inputMouseRightButton();
            return 0;
        }
    }
    
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void StateChange(unsigned int iNewState)
{
    if(cCurrentState)
    {
        cCurrentState->release();
        delete cCurrentState;
        cCurrentState = NULL;
    }

    switch(iNewState)
    {
    // exit
    case 0:             
        bEnd = true;
        break;

    // menu
    case STATE_MENU:
        cStateMenu = new CStateMenu(cRender, cAudio, &sGlobal);
        cCurrentState = (GEState*)cStateMenu;
        break;

    // match
    case STATE_MATCH:
        cStateMatch = new CStateMatch(cRender, cAudio, &sGlobal);
        cCurrentState = (GEState*)cStateMatch;
        break;
    }

    if(!bEnd)
    {
        cCurrentState->setCallback(StateChange);
        cCurrentState->init();
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
