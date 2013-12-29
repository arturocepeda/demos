
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- kinect.openni.cpp ---
*/

#ifdef _KINECT_OPENNI_

#include "kinect.h"
#include <NiTE.h>

//  Include directory:  C:\Program Files (x86)\OpenNI2\Include
//  Include directory:  C:\Program Files (x86)\PrimeSense\NiTE2\Include

#pragma comment(lib, "C:\\Program Files (x86)\\OpenNI2\\Lib\\OpenNI2.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\PrimeSense\\NiTE2\\Lib\\NiTE2.lib")

KinectHandPosition* pHandTracking;

void HandCreate(const nite::HandData& hand)
{
    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
        if(pHandTracking->PlayerID[i] == 0)
        {
            pHandTracking->PlayerReady[i] = true;
			pHandTracking->PlayerID[i] = hand.getId();
            return;
        }
    }
}

void HandUpdate(const nite::HandData& hand)
{
    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
		if(pHandTracking->PlayerID[i] == hand.getId())
        {
			const nite::Point3f& pPosition = hand.getPosition();
            pHandTracking->x[i] = pPosition.x;
            pHandTracking->y[i] = pPosition.y;
            pHandTracking->z[i] = pPosition.z;
            return;
        }
    }
}

void HandDestroy(const nite::HandData& hand)
{
    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
		if(pHandTracking->PlayerID[i] == hand.getId())
        {
            pHandTracking->PlayerReady[i] = false;
            pHandTracking->PlayerID[i] = 0;
            return;
        }
    }
}



//
//    Thread function
//
DWORD WINAPI KinectTracking(LPVOID HandTracking)
{
    pHandTracking = (KinectHandPosition*)HandTracking;

#ifndef _KINECT_
    pHandTracking->Error = true;
#else
    pHandTracking->Exit = false;
    pHandTracking->Error = false;
    pHandTracking->KinectReady = false;

    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
        pHandTracking->PlayerReady[i] = false;
        pHandTracking->PlayerID[i] = 0;

        pHandTracking->x[i] = 0.0f;
        pHandTracking->y[i] = 0.0f;
        pHandTracking->z[i] = 0.0f;
    }

	nite::HandTracker handTracker;
	nite::Status niteRc;

	// NiTE initialization
	niteRc = nite::NiTE::initialize();
	if(niteRc != nite::STATUS_OK)
	{
        pHandTracking->Error = true;
        return -1;
	}

	niteRc = handTracker.create();
	if(niteRc != nite::STATUS_OK)
	{
        pHandTracking->Error = true;
        return -1;
	}

	// recognition gesture
	handTracker.startGestureDetection(nite::GESTURE_WAVE);

    // tracking loop
	nite::HandTrackerFrameRef handTrackerFrame;
	pHandTracking->KinectReady = true;

	while(!pHandTracking->Exit)
	{
		niteRc = handTracker.readFrame(&handTrackerFrame);
		if (niteRc != nite::STATUS_OK)
			continue;

		const nite::Array<nite::GestureData>& gestures = handTrackerFrame.getGestures();
		for(int i = 0; i < gestures.getSize(); i++)
		{
			if(gestures[i].isComplete())
			{
				nite::HandId newId;
				handTracker.startHandTracking(gestures[i].getCurrentPosition(), &newId);
			}
		}

		const nite::Array<nite::HandData>& hands = handTrackerFrame.getHands();
		for(int i = 0; i < hands.getSize(); i++)
		{
			if(hands[i].isTracking())
				HandUpdate(hands[i]);
		}
	}

    // kinect global settings
    pHandTracking->PlayerReady[0] = false;
    pHandTracking->PlayerReady[1] = false;
    pHandTracking->KinectReady = false;

    // shutdown NiTE
	nite::NiTE::shutdown();
#endif

    return 0;
}

#endif
