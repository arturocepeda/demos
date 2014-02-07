
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- main.h ---
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define SCENE_TEST  1

#define MOUSE_CHECK_MARGIN  16
#define MOUSE_SET_MARGIN    24

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SceneChange(unsigned int iNewScene);

struct SGlobal
{
    // window values
    HWND hWnd;
    int ScreenSizeX;
    int ScreenSizeY;

    // game loop
    int iFPS;
};
