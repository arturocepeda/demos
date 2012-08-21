
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- kinect.openni.cpp ---
*/

#include "kinect.openni.h"

KinectHandPosition *pHandTracking;

#ifdef _KINECT_
xn::GestureGenerator kGestureGenerator;
xn::HandsGenerator kHandsGenerator;

//
//    Callback functions
//
void XN_CALLBACK_TYPE Gesture_Recognized(xn::GestureGenerator& generator, const XnChar* strGesture, 
                                         const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition, 
                                         void* pCookie) 
{ 
    kHandsGenerator.StartTracking(*pEndPosition); 
}

void XN_CALLBACK_TYPE Gesture_Progress(xn::GestureGenerator& generator, const XnChar* strGesture, 
                                       const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie) 
{
}

void XN_CALLBACK_TYPE Hand_Create(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, 
                                  XnFloat fTime, void* pCookie)
{
    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
        if(pHandTracking->PlayerID[i] == 0)
        {
            pHandTracking->PlayerReady[i] = true;
            pHandTracking->PlayerID[i] = nId;
            return;
        }
    }
}

void XN_CALLBACK_TYPE Hand_Update(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, 
                                  XnFloat fTime, void* pCookie)
{
    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
        if(pHandTracking->PlayerID[i] == nId)
        {
            pHandTracking->x[i] = pPosition->X;
            pHandTracking->y[i] = pPosition->Y;
            pHandTracking->z[i] = pPosition->Z;
            return;
        }
    }
}

void XN_CALLBACK_TYPE Hand_Destroy(xn::HandsGenerator& generator, XnUserID nId, XnFloat fTime, void* pCookie)
{
    for(int i = 0; i < KINECT_NUM_PLAYERS; i++)
    {
        if(pHandTracking->PlayerID[i] == nId)
        {
            pHandTracking->PlayerReady[i] = false;
            pHandTracking->PlayerID[i] = 0;
            return;
        }
    }
}
#endif


//
//    Thread function
//
DWORD WINAPI KinectTracking(LPVOID HandTracking)
{
    pHandTracking = (KinectHandPosition*)HandTracking;

#ifndef _KINECT_
    pHandTracking->Error = true;
#else
    xn::Context kContext;

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

    XnStatus rc = XN_STATUS_OK;
    XnCallbackHandle chandle1, chandle2;
    
    // initialization
    rc = kContext.Init();
    kContext.SetGlobalMirror(true);

    if(rc != XN_STATUS_OK)
    {
        pHandTracking->Error = true;
        return -1;
    }

    // gesture generator
    rc = kGestureGenerator.Create(kContext);

    if(rc != XN_STATUS_OK)
    {
        pHandTracking->Error = true;
        return -1;
    }

    // hands generator
    rc = kHandsGenerator.Create(kContext);

    if(rc != XN_STATUS_OK)
    {
        pHandTracking->Error = true;
        return -1;
    }

    // callback functions
    rc = kGestureGenerator.RegisterGestureCallbacks(Gesture_Recognized, Gesture_Progress, NULL, chandle1);

    if(rc != XN_STATUS_OK)
    {
        pHandTracking->Error = true;
        return -1;
    }

    rc = kHandsGenerator.RegisterHandCallbacks(Hand_Create, Hand_Update, Hand_Destroy, NULL, chandle2);

    if(rc != XN_STATUS_OK)
    {
        pHandTracking->Error = true;
        return -1;
    }

    // make the context start to generate events
    rc = kContext.StartGeneratingAll();

    if(rc != XN_STATUS_OK)
    {
        pHandTracking->Error = true;
        return -1;
    }

    // recognition gesture
    kGestureGenerator.AddGesture("Wave", NULL);
    pHandTracking->KinectReady = true;

    // tracking loop
    while(!pHandTracking->Exit)
        rc = kContext.WaitAndUpdateAll();

    // kinect global settings
    pHandTracking->PlayerReady[0] = false;
    pHandTracking->PlayerReady[1] = false;
    pHandTracking->KinectReady = false;

    // release objects
    kGestureGenerator.Release();
    kHandsGenerator.Release();
    kContext.Release();
#endif

    return 0;
}
