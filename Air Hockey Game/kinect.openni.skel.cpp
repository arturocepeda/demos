
/*
    Arturo Cepeda Pérez

	AirHockey2d - Using the Air Hockey Game Library

    --- kinect.cpp ---
*/

#include "kinect.h"
#include <XnCppWrapper.h>

xn::Context g_Context;
xn::ScriptNode g_scriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;

XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";

#define MAX_NUM_USERS 2

XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}


//
//	CALLBACK FUNCTIONS
//

void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
    if(g_bNeedPose)
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
    else
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
}

void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
    if(eStatus == XN_CALIBRATION_STATUS_OK)
    {
        // Calibration succeeded
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        // Calibration failed
        if(eStatus == XN_CALIBRATION_STATUS_MANUAL_ABORT)
            return;

        if(g_bNeedPose)
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
        else
            g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
}


//
//	THREAD FUNCTION
//

DWORD WINAPI KinectTracking(LPVOID HandTracking)
{
	KinectHandPosition *pHandTracking = (KinectHandPosition *)HandTracking;

	pHandTracking->kinectReady = false;
	pHandTracking->x = 0.0;
	pHandTracking->y = 0.0;
	pHandTracking->z = 0.0;

    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

    const char *fn = "SamplesConfig.xml";

    if(!fileExists(fn))
        return -1;

    nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);

    if(nRetVal != XN_STATUS_OK)
        return -1;

    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);

    if(nRetVal != XN_STATUS_OK)
		return -1;

    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);

	if(nRetVal != XN_STATUS_OK)
    {
        nRetVal = g_UserGenerator.Create(g_Context);

		if(nRetVal != XN_STATUS_OK)
			return -1;
    }

    nRetVal = g_UserGenerator.Create(g_Context);

    if(nRetVal != XN_STATUS_OK)
		return -1;

    XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;

    if(!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
        return -1;

    nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);

    if(nRetVal != XN_STATUS_OK)
		return -1;

    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);

    if(nRetVal != XN_STATUS_OK)
		return -1;
	
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);

    if(nRetVal != XN_STATUS_OK)
		return -1;

    if(g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        g_bNeedPose = TRUE;

        if(!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
            return -1;

        nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);

	    if(nRetVal != XN_STATUS_OK)
			return -1;
		
        g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
    }

    g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    nRetVal = g_Context.StartGeneratingAll();

    if(nRetVal != XN_STATUS_OK)
		return -1;

    XnUserID aUsers[MAX_NUM_USERS];
    XnUInt16 nUsers;
    XnUInt32 epochTime = 0;
	XnSkeletonJoint sJoint = (pHandTracking->rightHanded)? XN_SKEL_RIGHT_HAND: XN_SKEL_LEFT_HAND;
    XnSkeletonJointTransformation handJoint;

	pHandTracking->kinectReady = true;

    while(!pHandTracking->exit)
    {
        g_Context.WaitOneUpdateAll(g_UserGenerator);

        // print the torso information for the first user already tracking
        nUsers = MAX_NUM_USERS;
        g_UserGenerator.GetUsers(aUsers, nUsers);

        int numTracked = 0;
        int userToPrint = -1;

        for(XnUInt16 i = 0; i < nUsers; i++)
        {
            if(!g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
                continue;

            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i], sJoint, handJoint);

			pHandTracking->x = handJoint.position.position.X;
			pHandTracking->y = handJoint.position.position.Y;
			pHandTracking->z = handJoint.position.position.Z;
        }        
    }

    g_scriptNode.Release();
    g_DepthGenerator.Release();
    g_UserGenerator.Release();
    g_Context.Release();

	return 0;
}
