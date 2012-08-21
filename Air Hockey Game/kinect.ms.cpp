
/*
    Arturo Cepeda Pérez

	AirHockey3d - Using the Air Hockey Game Library

    --- kinect.ms.cpp ---
*/

#include "kinect.ms.h"
#include <stdio.h>
#include "main.h"

#ifdef _KINECT_
#include <ole2.h>   // neccesary if WIN32_LEAN_AND_MEAN has been defined!
#include "NuiApi.h"
#endif

KinectHandPosition* pHandTracking;

//
//	Thread function
//
DWORD WINAPI KinectTracking(LPVOID HandTracking)
{
    pHandTracking = (KinectHandPosition*)HandTracking;

    pHandTracking->Exit = false;
    pHandTracking->Error = false;
	pHandTracking->KinectReady = false;

    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
	    pHandTracking->PlayerReady[i] = false;
        pHandTracking->PlayerSkeleton[i] = -1;

	    pHandTracking->x[i] = 0.0f;
	    pHandTracking->y[i] = 0.0f;
	    pHandTracking->z[i] = 0.0f;
    }

#ifndef _KINECT_
    pHandTracking->Error = true;
#else
    // start Kinect
    if(NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON) != S_OK)
    {
        pHandTracking->Error = true;
        return -1;
    }

    // enable skeleton tracking
    NUI_SKELETON_FRAME nSkeletonFrame;
    HANDLE hSkeletonEvent;
    DWORD iEvent;

    int i;
    int j;

    hSkeletonEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    NuiSkeletonTrackingEnable(hSkeletonEvent, 0);
    pHandTracking->KinectReady = true;

    // right or left-handed
    int iHandID[KINECT_NUM_PLAYERS];

    for(i = 0; i < KINECT_NUM_PLAYERS; i++)
        iHandID[i] = (pHandTracking->PlayerRightHanded[i])? NUI_SKELETON_POSITION_HAND_RIGHT: NUI_SKELETON_POSITION_HAND_LEFT;

    // temporary flags
    bool bPlayerReady[KINECT_NUM_PLAYERS];

    // tracking loop
    while(!pHandTracking->Exit)
    {
        iEvent = WaitForSingleObject(hSkeletonEvent, 100);

        // player ready flags --> false
        memset(bPlayerReady, 0, KINECT_NUM_PLAYERS);

        if(iEvent == WAIT_TIMEOUT)
            continue;

        if(NuiSkeletonGetNextFrame(0, &nSkeletonFrame) >= 0)
        {
            for(i = 0; i < NUI_SKELETON_COUNT; i++)
            {
                if(nSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED ||
                   nSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_POSITION_ONLY)
                {
                    for(j = 0; j < KINECT_NUM_PLAYERS; j++)
                    {
                        // skeleton already assigned
                        if(pHandTracking->PlayerSkeleton[j] == i)
                        {
                            bPlayerReady[j] = true;
                            break;
                        }
                        
                        // first detection
                        if(pHandTracking->PlayerSkeleton[j] == -1)
                        {
                            pHandTracking->PlayerSkeleton[j] = i;
                            bPlayerReady[j] = true;
                            break;
                        }
                    }
                }
            }
        }

        NuiTransformSmooth(&nSkeletonFrame, NULL);

        for(j = 0; j < KINECT_NUM_PLAYERS; j++)
        {
            pHandTracking->PlayerReady[j] = bPlayerReady[j];

            if(pHandTracking->PlayerReady[j])
            {
                i = pHandTracking->PlayerSkeleton[j];

                if(nSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED &&
                   nSkeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[iHandID[j]] != NUI_SKELETON_POSITION_NOT_TRACKED)
                {
                    pHandTracking->x[j] = nSkeletonFrame.SkeletonData[i].SkeletonPositions[iHandID[j]].x * 1000;
                    pHandTracking->y[j] = nSkeletonFrame.SkeletonData[i].SkeletonPositions[iHandID[j]].y * 1000;
                    pHandTracking->z[j] = nSkeletonFrame.SkeletonData[i].SkeletonPositions[iHandID[j]].z * 1000;
                }
            }
            else if(pHandTracking->PlayerSkeleton[j] > -1)
            {
                pHandTracking->PlayerSkeleton[j] = -1;
            }
        }
    }

	// release the objects
    CloseHandle(hSkeletonEvent);

    for(j = 0; j < KINECT_NUM_PLAYERS; j++)
	    pHandTracking->PlayerReady[j] = false;

	pHandTracking->KinectReady = false;

    // shut down Kinect
    NuiSkeletonTrackingDisable();
    NuiShutdown();
#endif
	return 0;
}
