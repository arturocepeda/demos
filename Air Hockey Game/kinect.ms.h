
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- kinect.ms.h ---
*/

//  C:\Program Files\Microsoft SDKs\Kinect\v1.5\inc
//  C:\Program Files\Microsoft SDKs\Kinect\v1.5\lib\x86\Kinect10.lib

#ifndef _KINECT_H_
#define _KINECT_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define KINECT_NUM_PLAYERS 2

struct KinectHandPosition
{
    float x[KINECT_NUM_PLAYERS];
    float y[KINECT_NUM_PLAYERS];
    float z[KINECT_NUM_PLAYERS];

    bool PlayerReady[KINECT_NUM_PLAYERS];
    bool PlayerRightHanded[KINECT_NUM_PLAYERS];
    int  PlayerSkeleton[KINECT_NUM_PLAYERS];

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

#endif
