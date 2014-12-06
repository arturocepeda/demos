
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Windows (DirectX 11)
//
//  --- main.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GETypes.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define GE_MOUSE_CHECK_MARGIN  8
#define GE_MOUSE_SET_MARGIN    24

#define GE_CAMERA_MOVE         5.0f
#define GE_CAMERA_ROTATE       0.05f
#define GE_CAMERA_PITCH_LIMIT  1.05f

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
GE::Vector2 GetMouseScreenPosition();
void StateChange(unsigned int iNewScene);
