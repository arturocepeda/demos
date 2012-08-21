
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- main.h ---
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "AHAI.h"

//#define _DEBUG_
//#define _KINECT_

#define SCENE_MENU      1
#define SCENE_MATCH     2

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SceneChange(unsigned int iNewScene);
void DefaultSettings();

struct SGlobal
{
    // window values
    HWND hWnd;
    int ScreenSizeX;
    int ScreenSizeY;

    // game loop
    int iFPS;

    // interface
    int iHumanPlayers;
    bool bSplit;
    bool bKinect;

    // settings
    int iGameMaxMinutes;
    int iGameMaxGoals;
    bool bTableAir;
    AHAILevel cPlayer2AILevel;
    bool bAutomaticReplay;

    // multiplayer
    bool bMultiplayer;
    bool bServer;
    char sIPServer[16];
    unsigned int iPort;

    // Kinect calibration
    float fKinectWidth;
    float fKinectMinimumDistance;
    float fKinectMaximumDistance;
    bool bPlayer1RightHanded;
    bool bPlayer2RightHanded;
};

#endif
