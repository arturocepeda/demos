
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- main.h ---
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define STATE_FACING    1
#define STATE_ARRIVE    2

#define MOUSE_CHECK_MARGIN  16
#define MOUSE_SET_MARGIN    24

#define CAMERA_MOVE         5.0f
#define CAMERA_ROTATE       0.05f
#define CAMERA_PITCH_LIMIT  1.05f

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void StateChange(unsigned int iNewScene);

struct SGlobal
{
    // window values
    HWND hWnd;
    int ScreenSizeX;
    int ScreenSizeY;

    // game loop
    int iFPS;
};
