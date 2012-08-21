
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- kinect.openni.h ---
*/

#ifndef _KINECT_H_
#define _KINECT_H_

#include "main.h"

#ifdef _KINECT_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XnCppWrapper.h>
#endif

//  C:\Program Files (x86)\OpenNI\Include
//  C:\Program Files (x86)\OpenNI\Lib\OpenNI.lib

#define KINECT_NUM_PLAYERS 2

struct KinectHandPosition
{
    float x[KINECT_NUM_PLAYERS];
    float y[KINECT_NUM_PLAYERS];
    float z[KINECT_NUM_PLAYERS];

    bool PlayerReady[KINECT_NUM_PLAYERS];
    unsigned int PlayerID[KINECT_NUM_PLAYERS];

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
