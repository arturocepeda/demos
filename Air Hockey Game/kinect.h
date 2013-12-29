
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- kinect.h ---
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define KINECT_NUM_PLAYERS 2

struct KinectHandPosition
{
    float x[KINECT_NUM_PLAYERS];
    float y[KINECT_NUM_PLAYERS];
    float z[KINECT_NUM_PLAYERS];

    bool PlayerReady[KINECT_NUM_PLAYERS];
    int  PlayerID[KINECT_NUM_PLAYERS];
    bool PlayerRightHanded[KINECT_NUM_PLAYERS];

    bool KinectReady;
    bool Error;
    bool Exit;
};

struct KinectCalibration
{
    float Width;
    float MinimumDistance;
    float MaximumDistance;
};

DWORD WINAPI KinectTracking(LPVOID HandTracking);

