
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- stateMatch.cpp ---
*/

#include "stateMatch.h"
#include "AHAI.h"
#include "Multiplayer/Win32/GEMultiplayerWinSock2.h"
#include <stdio.h>
#include <climits>

CStateMatch::CStateMatch(GERendering* Render, GEAudio* Audio, void* GlobalData)
    : GEState(Render, Audio, GlobalData)
{
    sGlobal = (SGlobal*)GlobalData;
    iSleepFrame = UINT_MAX - SLEEP_FRAMES;
    bPlayAgain = false;
}

CStateMatch::~CStateMatch()
{
}

void CStateMatch::internalInit()
{
    iCurrentFrame = 0;

    initRenderObjects();
    initSoundObjects();

    // messages
    sprintf(sMessage, "");
    cPlayer2AI = NULL;

    // time
    iTimeMin = sGlobal->iGameMaxMinutes;
    iTimeSec = 0;

    // multiplayer
    cServer = NULL;
    cClient = NULL;

    // replay mode
    bReplayMode = false;
    iCurrentReplayRecordFrame = 0;

    // game object
    cGame = new AHGame(0.0f, 0.0f, AH_TABLE_WIDTH / 800.0f, AH_TABLE_HEIGHT / 800.0f);
    cGame->setTableFriction((sGlobal->bTableAir)? AH_FRICTION_AIR: AH_FRICTION_NO_AIR);
    cGame->setMouseRatio(0.0f, (float)sGlobal->ScreenSizeX, 0.0f, (float)sGlobal->ScreenSizeY);
    cGame->start();

    // fill initial replay data
    for(int i = 0; i < REPLAY_FRAMES; i++)
    {
        sReplayFrame[i].PositionPlayer1 = cGame->getRenderPositionPlayer1();
        sReplayFrame[i].PositionPlayer2 = cGame->getRenderPositionPlayer2();
        sReplayFrame[i].PositionPuck = cGame->getRenderPositionPuck();
        sReplayFrame[i].PuckVisible = true;
        sReplayFrame[i].Interpolate = false;
    }

    // AI object for the CPU player
    if(sGlobal->iHumanPlayers == 1)
    {
        // player 2 --> remote user
        if(sGlobal->bMultiplayer)
        {
            // I am the server
            if(sGlobal->bServer)
            {
                cServer = new GEServerWinSock2();
                cServer->activeServer(sGlobal->iPort);
                bClientReady = false;
            }

            // I am the client
            else
            {
                cClient = new GEClientWinSock2();
                cClient->connectToServer(sGlobal->sIPServer, sGlobal->iPort);
                bServerReady = false;

                // request to the server
                sUDPBuffer[0] = MSG_REQ;
                cClient->sendMessage(sUDPBuffer, MSG_SIZE);
            }
        }

        // player 2 --> CPU
        else
        {
            cPlayer2AI = new AHAI(cGame, sGlobal->cPlayer2AILevel, false);
        }
    }

    // Kinect management
    if(sGlobal->bKinect)
    {
#ifndef _KINECT_OPENNI_
        kHandPosition.PlayerRightHanded[0] = sGlobal->bPlayer1RightHanded;
        kHandPosition.PlayerRightHanded[1] = sGlobal->bPlayer2RightHanded;
#endif

        // create a new thread to track the hand position
        hKinectThread = CreateThread(NULL, 0, KinectTracking, &kHandPosition, 0, NULL);

        sprintf(sMessage, "Starting Kinect...");
        render();

        while(!kHandPosition.KinectReady && !kHandPosition.Error)
            Sleep(1);

        if(!kHandPosition.Error)
        {
            // Kinect calibration
            kCalibration.Width = sGlobal->fKinectWidth;
            kCalibration.MinimumDistance = sGlobal->fKinectMinimumDistance;
            kCalibration.MaximumDistance = sGlobal->fKinectMaximumDistance;

            // set Kinect ratio (X, Z)
            cGame->setMouseRatio(-kCalibration.Width, kCalibration.Width, 
                                 kCalibration.MinimumDistance, kCalibration.MaximumDistance);
        }
        else
        {
            sGlobal->bKinect = false;
        }
    }
}

void CStateMatch::initRenderObjects()
{
    // lighting
    cRender->setAmbientLightColor(GEColor((byte)100, 100, 100));
    iLightRoom = cRender->createDirectionalLight(GEColor(0.3f, 0.3f, 0.3f), 1.0f, GEVector3(0.3f, -1.0f, 0.4f));
    cRender->switchLight(iLightRoom, true);

    // cameras
    cRender->createCamera(&mCameraPlayer1);
    cRender->createCamera(&mCameraPlayer2);

    for(int i = 0; i < REPLAY_CAMERAS; i++)
        cRender->createCamera(&mCameraReplay[i]);

    mCameraPlayer1->setPosition(0.0f, 2.5f, -3.7f);
    mCameraPlayer1->lookAt(0.0f, -3.5f, 4.0f);
    mCameraPlayer1->use();

    mCameraPlayer2->setPosition(0.0f, 2.5f, 3.7f);
    mCameraPlayer2->lookAt(0.0f, -3.5f, -4.0f);

    mCameraReplay[0]->setPosition(3.2f, 2.8f, -3.7f);
    mCameraReplay[0]->lookAt(0.0f, -0.75f, 0.0f);

    mCameraReplay[1]->setPosition(0.0f, 7.5f, 0.0f);
    mCameraReplay[1]->lookAt(0.0f, 0.0f, 0.0001f);

    // view ports
    iPortFullScreen = 0;

    if(sGlobal->bSplit)
    {
        iPortPlayer1 = 1;
        iPortPlayer2 = 2;
        iPortMessages = 3;
        iPortSpaceBelow = 4;

        cRender->defineViewPort(iPortPlayer1, 0, sGlobal->ScreenSizeY / 6, 
                                              sGlobal->ScreenSizeX / 2, sGlobal->ScreenSizeY / 2);
        cRender->defineViewPort(iPortPlayer2, sGlobal->ScreenSizeX / 2, sGlobal->ScreenSizeY / 6, 
                                              sGlobal->ScreenSizeX / 2, sGlobal->ScreenSizeY / 2);
        cRender->defineViewPort(iPortMessages, 0, 0, sGlobal->ScreenSizeX, sGlobal->ScreenSizeY / 4);
        cRender->defineViewPort(iPortSpaceBelow, 0, sGlobal->ScreenSizeY * 3 / 4, 
                                                 sGlobal->ScreenSizeX, sGlobal->ScreenSizeY / 4);
    }

    // meshes
    cRender->createMesh(&mMeshMallet1);
    cRender->createMesh(&mMeshMallet2);
    cRender->createMesh(&mMeshPuck);
    cRender->createMesh(&mMeshTable);
    cRender->createMesh(&mMeshRoom);

    mMeshMallet1->loadFromFile("meshes\\mallet.x");
    mMeshMallet2->loadFromFile("meshes\\mallet.x");
    mMeshPuck->loadFromFile("meshes\\puck.x");
    mMeshTable->loadFromFile("meshes\\table.x", "textures\\");
    mMeshRoom->loadFromFile("meshes\\room.x", "textures\\");

    bPuckVisible = true;

    // sprites
    char sFilename[32];

    for(unsigned int i = 0; i < 10; i++)
    {
        cRender->createSprite(&sDisplay[i]);
        sprintf(sFilename, "sprites\\display%d.png", i);
        sDisplay[i]->loadFromFile(sFilename);
    }

    cRender->createSprite(&sDisplaySeparator);
    strcpy(sFilename, "sprites\\displaysep.png");
    sDisplaySeparator->loadFromFile(sFilename);

    cRender->createSprite(&sFrame);
    strcpy(sFilename, (sGlobal->bSplit)? "sprites\\hframe.png": "sprites\\vframe.png");
    sFrame->loadFromFile(sFilename);

    // fonts
    iFontText = 1;
    iFontDebug = 2;

    cRender->defineFont(iFontText, "Courier New", 50.0f, true, true);
    cRender->defineFont(iFontDebug, "Courier New", 20.0f);

    // colors
    cColorMessage.set((byte)140, 33, 27);
    cColorDebug.set((byte)255, 255, 255);

    // labels
    cRender->createLabel(&lLabelMessage, iFontText, GEAlignment::CenterCenter,
                         GEVector2((float)sGlobal->ScreenSizeX, (float)120));
    lLabelMessage->setColor(cColorMessage);

    cRender->createLabel(&lLabelDebug, iFontDebug, GEAlignment::TopLeft,
                         GEVector2((float)sGlobal->ScreenSizeX, (float)sGlobal->ScreenSizeY));
    lLabelDebug->setColor(cColorDebug);
}

void CStateMatch::releaseRenderObjects()
{
    // light
    cRender->releaseLight(iLightRoom);

    // cameras
    cRender->releaseCamera(&mCameraPlayer1);
    cRender->releaseCamera(&mCameraPlayer2);

    for(int i = 0; i < REPLAY_CAMERAS; i++)
        cRender->releaseCamera(&mCameraReplay[i]);

    // view ports
    if(sGlobal->bSplit)
    {
        cRender->releaseViewPort(iPortPlayer1);
        cRender->releaseViewPort(iPortPlayer2);
        cRender->releaseViewPort(iPortMessages);
        cRender->releaseViewPort(iPortSpaceBelow);
    }

    // meshes
    mMeshMallet1->unload();
    mMeshMallet2->unload();
    mMeshPuck->unload();
    mMeshTable->unload();
    mMeshRoom->unload();

    cRender->releaseMesh(&mMeshMallet1);
    cRender->releaseMesh(&mMeshMallet2);
    cRender->releaseMesh(&mMeshPuck);
    cRender->releaseMesh(&mMeshTable);
    cRender->releaseMesh(&mMeshRoom);

    // sprites
    for(unsigned int i = 0; i < 10; i++)
    {
        sDisplay[i]->unload();
        cRender->releaseSprite(&sDisplay[i]);
    }

    sDisplaySeparator->unload();
    sFrame->unload();

    cRender->releaseSprite(&sDisplaySeparator);
    cRender->releaseSprite(&sFrame);

    // labels
    cRender->releaseLabel(&lLabelMessage);
    cRender->releaseLabel(&lLabelDebug);

    // fonts
    cRender->releaseFont(iFontText);
    cRender->releaseFont(iFontDebug);
}

void CStateMatch::initSoundObjects()
{
    iChannelMallet1 = 0;
    iChannelMallet2 = 1;
    iChannelPuck = 2;

    // load samples
    char sSoundFile[32];
    unsigned int iSoundCounter = 0;
    unsigned int i;

    for(i = 0; i < SOUNDS_PUCK; i++)
    {
        iSoundPuckMallet[i] = iSoundCounter++;
        sprintf(sSoundFile, "sounds\\ph%d", i);
        cAudio->loadSound(iSoundPuckMallet[i], sSoundFile, "ogg");
    }

    for(i = 0; i < SOUNDS_TABLE; i++)
    {
        iSoundPuckTable[i] = iSoundCounter++;
        sprintf(sSoundFile, "sounds\\pt%d", i);
        cAudio->loadSound(iSoundPuckTable[i], sSoundFile, "ogg");
    }

    for(i = 0; i < SOUNDS_GOAL; i++)
    {
        iSoundGoal[i] = iSoundCounter++;
        sprintf(sSoundFile, "sounds\\goal%d", i);
        cAudio->loadSound(iSoundGoal[i], sSoundFile, "ogg");
    }

    // listener position (player 1)
    cAudio->setListenerPosition(GEVector3(0.0f, 2.5f / AUDIO_RATIO, -3.7f / AUDIO_RATIO));
}

void CStateMatch::releaseSoundObjects()
{
    // stop channels
    cAudio->stop(iChannelMallet1);
    cAudio->stop(iChannelMallet2);
    cAudio->stop(iChannelPuck);

    // unload samples
    unsigned int i;

    for(i = 0; i < SOUNDS_PUCK; i++)
        cAudio->unloadSound(iSoundPuckMallet[i]);

    for(i = 0; i < SOUNDS_TABLE; i++)
        cAudio->unloadSound(iSoundPuckTable[i]);

    for(i = 0; i < SOUNDS_GOAL; i++)
        cAudio->unloadSound(iSoundGoal[i]);
}

void CStateMatch::update(float DeltaTime)
{
    iCurrentFrame++;

    // sleep mode
    if((iCurrentFrame - iSleepFrame) < SLEEP_FRAMES)
        return;

    // restart game
    if(bPlayAgain)
    {
        playAgain();
        bPlayAgain = false;
    }

    // replay mode
    if(bReplayMode)
        return;

    // multiplayer: wait for connection
    if(sGlobal->bMultiplayer)
    {
        // wait for client
        if(sGlobal->bServer && !bClientReady)
        {
            if(cServer->receiveMessage(&sClientAddress, sUDPBuffer, MSG_SIZE) > 0 && sUDPBuffer[0] == MSG_REQ)
            {
                sUDPBuffer[0] = MSG_ACK;
                cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);

                bClientReady = true;
                return;
            }

            sprintf(sMessage, "Waiting for client...");
            return;
        }

        // wait for server
        if(!sGlobal->bServer && !bServerReady)
        {
            if(cClient->receiveMessage(sUDPBuffer, MSG_SIZE) > 0 && sUDPBuffer[0] == MSG_ACK)
            {
                bServerReady = true;
                return;
            }

            // new request to the server
            if(iCurrentFrame % sGlobal->iFPS == 0)
            {
                sUDPBuffer[0] = MSG_REQ;
                cClient->sendMessage(sUDPBuffer, MSG_SIZE);
            }

            sprintf(sMessage, "Waiting for server...");
            return;
        }
    }

    sprintf(sMessage, "");

    // set player 1 position
    if(sGlobal->bKinect)
    {
        // check Kinect status
        // two players
        if(sGlobal->iHumanPlayers == 2 && (!kHandPosition.PlayerReady[0] || !kHandPosition.PlayerReady[1]))
        {
            if(!kHandPosition.PlayerReady[0] || !kHandPosition.PlayerReady[1])
            {
                if(!kHandPosition.PlayerReady[0] && !kHandPosition.PlayerReady[1])
                    sprintf(sMessage, "Kinect: waiting for the players...");
                else if(!kHandPosition.PlayerReady[0])
                    sprintf(sMessage, "Kinect: waiting for player 1...");
                else
                    sprintf(sMessage, "Kinect: waiting for player 2...");

                return;
            }
        }

        // single player
        else if(!kHandPosition.PlayerReady[0])
        {
            sprintf(sMessage, "Kinect: waiting for player...");

            if(!kHandPosition.PlayerReady[0])
                return;
        }

        // Kinect (two players)
        if(sGlobal->iHumanPlayers == 2)
        {
            cGame->setMouseRatio(-kCalibration.Width * 2 - 50, -50, 
                                 kCalibration.MinimumDistance, kCalibration.MaximumDistance);
            cGame->setMousePositionPlayer1(kHandPosition.x[0], kHandPosition.z[0]);

            cGame->setMouseRatio(kCalibration.Width * 2 + 50, 50, 
                                 kCalibration.MinimumDistance, kCalibration.MaximumDistance);
            cGame->setMousePositionPlayer2(kHandPosition.x[1], kHandPosition.z[1]);
        }

        // Kinect (single player)
        else
        {
            cGame->setMousePositionPlayer1(kHandPosition.x[0], kHandPosition.z[0]);
        }
    }
    else
    {
        // mouse
        cGame->setMousePositionPlayer1((float)iMouseX, (float)iMouseY);
    }

    // multiplayer: client
    if(sGlobal->bMultiplayer && !sGlobal->bServer)
    {
        updateClient();
        return;
    }

    // game time
    if(iCurrentFrame % sGlobal->iFPS == 0)
    {
        iTimeSec--;

        if(iTimeSec < 0)
        {
            iTimeSec = 59;
            iTimeMin--;
        }

        if(iTimeMin < 0)
        {
            iTimeMin = 0;
            iTimeSec = 0;
            endOfTheMatch();
        }

        // send time to client
        if(sGlobal->bMultiplayer)
        {
            sUDPBuffer[0] = MSG_TIME;
            sUDPBuffer[1] = (char)iTimeMin;
            sUDPBuffer[2] = (char)iTimeSec;
            cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);
        }
    }

    // set player 2 position (CPU)
    if(sGlobal->iHumanPlayers == 1 && !sGlobal->bMultiplayer)
        cGame->setTablePositionPlayer2(cPlayer2AI->update());
    
    // check goals
    bPuckVisible = true;

    if(cGame->checkEvent() == AH_EVENT_GOAL_P1 || cGame->checkEvent() == AH_EVENT_GOAL_P2)
    {
        bPuckVisible = false;

        // send "goal" to client
        if(sGlobal->bMultiplayer)
        {
            sUDPBuffer[0] = MSG_GOAL;
            sUDPBuffer[1] = (char)cGame->getGoalsPlayer2();
            sUDPBuffer[2] = (char)cGame->getGoalsPlayer1();
            cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);
        }

        // automatic replay
        if(sGlobal->bAutomaticReplay && !sGlobal->bMultiplayer)
        {
            // only goals without rebounds
            if((cGame->checkEvent() == AH_EVENT_GOAL_P1 && cGame->getLastPlayerWhoHitThePuck() == 1) ||
               (cGame->checkEvent() == AH_EVENT_GOAL_P2 && cGame->getLastPlayerWhoHitThePuck() == 2))
            {
                replayMode();
                return;
            }
        }

        // pause and then back to the game
        render();
        iSleepFrame = iCurrentFrame;
    }

    // end of the match?
    if(cGame->getGoalsPlayer1() == sGlobal->iGameMaxGoals || cGame->getGoalsPlayer2() == sGlobal->iGameMaxGoals)
    {
        endOfTheMatch();
        return;
    }

    // replay data
    sReplayFrame[iCurrentReplayRecordFrame].PositionPlayer1 = cGame->getRenderPositionPlayer1();
    sReplayFrame[iCurrentReplayRecordFrame].PositionPlayer2 = cGame->getRenderPositionPlayer2();
    sReplayFrame[iCurrentReplayRecordFrame].PositionPuck = cGame->getRenderPositionPuck();
    sReplayFrame[iCurrentReplayRecordFrame].PuckVisible = bPuckVisible;
    sReplayFrame[iCurrentReplayRecordFrame].Interpolate = !(cGame->checkEvent() == AH_EVENT_GOAL_P1 || 
                                                            cGame->checkEvent() == AH_EVENT_GOAL_P2);
    iCurrentReplayRecordFrame++;

    if(iCurrentReplayRecordFrame == REPLAY_FRAMES)
        iCurrentReplayRecordFrame = 0;

    // game status update
    cGame->update();

    // send game data to client
    if(sGlobal->bMultiplayer && (iCurrentFrame % MULTIPLAYER_DATA_RATE) == 0)
        sendDataToClient();

    // video and audio
    playSounds(cGame->checkEvent());
}

void CStateMatch::sendDataToClient()
{
    AHPoint pPosition;
    UFloatValue uX;
    UFloatValue uY;

    // get client position
    while(cServer->receiveMessage(&sClientAddress, sUDPBuffer, MSG_SIZE) > 0)
    {
        memcpy(uX.CharValue, sUDPBuffer + 1, 4);
        memcpy(uY.CharValue, sUDPBuffer + 5, 4);

        pPosition.x = -uX.FloatValue;
        pPosition.y = -uY.FloatValue;

        cGame->setTablePositionPlayer2(pPosition);
    }

    // send player position
    uX.FloatValue = cGame->getTablePositionPlayer1().x;
    uY.FloatValue = cGame->getTablePositionPlayer1().y;
        
    sUDPBuffer[0] = MSG_MALLET;
    memcpy(sUDPBuffer + 1, uX.CharValue, 4);
    memcpy(sUDPBuffer + 5, uY.CharValue, 4);
    cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);

    // send puck position
    uX.FloatValue = cGame->getTablePositionPuck().x;
    uY.FloatValue = cGame->getTablePositionPuck().y;

    sUDPBuffer[0] = MSG_PUCK;
    memcpy(sUDPBuffer + 1, uX.CharValue, 4);
    memcpy(sUDPBuffer + 5, uY.CharValue, 4);
    cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);

    // send event
    if(cGame->checkEvent() != AH_EVENT_NONE)
    {
        sUDPBuffer[0] = MSG_EVENT;
        sUDPBuffer[1] = (char)cGame->checkEvent();
        cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);
    }
}

void CStateMatch::updateClient()
{
    // send position
    UFloatValue uX;
    UFloatValue uY;

    uX.FloatValue = cGame->getTablePositionPlayer1().x;
    uY.FloatValue = cGame->getTablePositionPlayer1().y;

    sUDPBuffer[0] = MSG_MALLET;
    memcpy(sUDPBuffer + 1, uX.CharValue, 4);
    memcpy(sUDPBuffer + 5, uY.CharValue, 4);

    cClient->sendMessage(sUDPBuffer, MSG_SIZE);

    // get game status
    AHPoint pNewPosition;
    int iEvent = 0;

    while(cClient->receiveMessage(sUDPBuffer, MSG_SIZE) > 0)
    {
        switch(sUDPBuffer[0])
        {
        // player position
        case MSG_MALLET:

            memcpy(uX.CharValue, sUDPBuffer + 1, 4);
            memcpy(uY.CharValue, sUDPBuffer + 5, 4);

            pNewPosition.x = -uX.FloatValue;
            pNewPosition.y = -uY.FloatValue;
            cGame->setTablePositionPlayer2(pNewPosition);

            break;

        // puck position
        case MSG_PUCK:

            bPuckVisible = true;

            memcpy(uX.CharValue, sUDPBuffer + 1, 4);
            memcpy(uY.CharValue, sUDPBuffer + 5, 4);

            pNewPosition.x = -uX.FloatValue;
            pNewPosition.y = -uY.FloatValue;
            cGame->setTablePositionPuck(pNewPosition);

            break;

        // time left
        case MSG_TIME:

            iTimeMin = sUDPBuffer[1];
            iTimeSec = sUDPBuffer[2];

            break;

        // event
        case MSG_EVENT:

            iEvent = sUDPBuffer[1];

            break;

        // goal
        case MSG_GOAL:

            bPuckVisible = false;

            cGame->setGoalsPlayer1(sUDPBuffer[1]);
            cGame->setGoalsPlayer2(sUDPBuffer[2]);

            break;

        // end of the match
        case MSG_END:

            endOfTheMatch();
            return;
        }
    }

    // audio
    playSounds(iEvent);
}

void CStateMatch::release()
{
    if(sGlobal->bKinect)
    {
        kHandPosition.Exit = true;
        WaitForSingleObject(hKinectThread, INFINITE);
        CloseHandle(hKinectThread);
    }

    if(sGlobal->bMultiplayer)
    {
        if(sGlobal->bServer)
        {
            if(cServer)
                delete cServer;
        }
        else
        {
            if(cClient)
                delete cClient;
        }
    }

    if(cPlayer2AI)
        delete cPlayer2AI;

    delete cGame;

    releaseSoundObjects();
    releaseRenderObjects();
}

void CStateMatch::inputKeyPress(char Key)
{
    if(bReplayMode && !sGlobal->bMultiplayer)
    {
        bReplayMode = false;
        return;
    }

    switch(Key)
    {
    case KEY_ESC:
        endOfTheMatch();
        break;

    case 'R': 
        replayMode();
        break;
    }
}

void CStateMatch::render()
{
    // replay mode
    if(bReplayMode)
    {
        renderReplay();
        return;
    }

    mCameraPlayer1->use();

    if(sGlobal->bSplit)
        cRender->useViewPort(iPortPlayer1);
    else
        cRender->useViewPort(iPortFullScreen);

    // room
    mMeshRoom->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshRoom);

    // table
    mMeshTable->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshTable);
    
    // player 1
    pPlayer1Position = cGame->getRenderPositionPlayer1();
    mMeshMallet1->setPosition(pPlayer1Position.x, 0.0f, -pPlayer1Position.y);
    cRender->renderMesh(mMeshMallet1);

#ifdef _DEBUG
    GEVector3 vPlayer1(pPlayer1Position.x, 0.0f, -pPlayer1Position.y);
    GEVector3 vDebug;
    cRender->worldToScreen(&vPlayer1, &vDebug);
    lLabelDebug->setPosition(vDebug.X, vDebug.Y);
    sprintf(sBuffer, "(%.2f, %.2f, %.2f)", vPlayer1.X, vPlayer1.Y, vPlayer1.Z);
    lLabelDebug->setText(sBuffer);
    cRender->renderLabel(lLabelDebug);
#endif

    // player 2
    pPlayer2Position = cGame->getRenderPositionPlayer2();
    mMeshMallet2->setPosition(pPlayer2Position.x, 0.0f, -pPlayer2Position.y);
    cRender->renderMesh(mMeshMallet2);

#ifdef _DEBUG
    GEVector3 vPlayer2(pPlayer2Position.x, 0.0f, -pPlayer2Position.y);
    cRender->worldToScreen(&vPlayer2, &vDebug);
    lLabelDebug->setPosition(vDebug.X, vDebug.Y);
    sprintf(sBuffer, "(%.2f, %.2f, %.2f)", vPlayer2.X, vPlayer2.Y, vPlayer2.Z);
    lLabelDebug->setText(sBuffer);
    cRender->renderLabel(lLabelDebug);
#endif

    // puck
    if(bPuckVisible)
    {
        pPuckPosition = cGame->getRenderPositionPuck();
        mMeshPuck->setPosition(pPuckPosition.x, 0.0f, -pPuckPosition.y);
        cRender->renderMesh(mMeshPuck);

#ifdef _DEBUG
        GEVector3 vPuck(pPuckPosition.x, 0.0f, -pPuckPosition.y);
        cRender->worldToScreen(&vPuck, &vDebug);
        lLabelDebug->setPosition(vDebug.X, vDebug.Y);
        sprintf(sBuffer, "(%.2f, %.2f, %.2f)", vPuck.X, vPuck.Y, vPuck.Z);
        lLabelDebug->setText(sBuffer);
        cRender->renderLabel(lLabelDebug);
#endif
    }

    if(sGlobal->bSplit)
    {
        // meshes
        cRender->useViewPort(iPortPlayer2);
        mCameraPlayer2->use();

        cRender->renderMesh(mMeshRoom);
        cRender->renderMesh(mMeshTable);
        cRender->renderMesh(mMeshMallet1);
        cRender->renderMesh(mMeshMallet2);
        cRender->renderMesh(mMeshPuck);

        cRender->useViewPort(iPortSpaceBelow);

        // frame
        sFrame->setScale(1.0f, 0.6f, 1.0f);
        fPosX = sGlobal->ScreenSizeX / 2.0f;
        fPosY = sGlobal->ScreenSizeY * 3 / 4.0f + 15.0f;

        sFrame->setPosition(fPosX, fPosY, 0.0f);
        cRender->renderSprite(sFrame);

        // score
        fPosY -= 5.0f;

        sDisplay[cGame->getGoalsPlayer1() / 10]->setScale(0.25f, 0.25f, 1.0f);
        sDisplay[cGame->getGoalsPlayer1() / 10]->setPosition(fPosX - 215.0f, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer1() / 10]);

        sDisplay[cGame->getGoalsPlayer1() % 10]->setScale(0.25f, 0.25f, 1.0f);
        sDisplay[cGame->getGoalsPlayer1() % 10]->setPosition(fPosX - 180.0f, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer1() % 10]);

        sDisplay[cGame->getGoalsPlayer2() / 10]->setScale(0.25f, 0.25f, 1.0f);
        sDisplay[cGame->getGoalsPlayer2() / 10]->setPosition(fPosX + 120.0f, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer2() / 10]);

        sDisplay[cGame->getGoalsPlayer2() % 10]->setScale(0.25f, 0.25f, 1.0f);
        sDisplay[cGame->getGoalsPlayer2() % 10]->setPosition(fPosX + 155.0f, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer2() % 10]);

        // time
        sDisplay[iTimeSec % 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeSec % 10]->setPosition(fPosX, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeSec % 10]);

        sDisplay[iTimeSec / 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeSec / 10]->setPosition(fPosX - 20, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeSec / 10]);

        sDisplay[iTimeMin % 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeMin % 10]->setPosition(fPosX - 50, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeMin % 10]);

        sDisplay[iTimeMin / 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeMin / 10]->setPosition(fPosX - 70, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeMin / 10]);

        sDisplaySeparator->setScale(0.15f, 0.15f, 1.0f);
        sDisplaySeparator->setPosition(fPosX - 15.0f, fPosY, 0.0f);
        cRender->renderSprite(sDisplaySeparator);

        mCameraPlayer1->use();
    }
    else
    {
        fPosX = sGlobal->ScreenSizeX - 135.0f;
        fPosY = sGlobal->ScreenSizeY / 2.0f - 175.0f;

        // frame
        sFrame->setScale(0.525f, 0.8f, 1.0f);
        sFrame->setPosition(fPosX, fPosY, 0.0f);
        cRender->renderSprite(sFrame);

        // score
        fPosX += 50.0f;

        sDisplay[cGame->getGoalsPlayer2() / 10]->setScale(0.22f, 0.22f, 1.0f);
        sDisplay[cGame->getGoalsPlayer2() / 10]->setPosition(fPosX - 39.0f, fPosY - 125.0f, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer2() / 10]);

        sDisplay[cGame->getGoalsPlayer2() % 10]->setScale(0.22f, 0.22f, 1.0f);
        sDisplay[cGame->getGoalsPlayer2() % 10]->setPosition(fPosX - 9.0f, fPosY - 125.0f, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer2() % 10]);

        sDisplay[cGame->getGoalsPlayer1() / 10]->setScale(0.22f, 0.22f, 1.0f);
        sDisplay[cGame->getGoalsPlayer1() / 10]->setPosition(fPosX - 39.0f, fPosY + 140.0f, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer1() / 10]);

        sDisplay[cGame->getGoalsPlayer1() % 10]->setScale(0.22f, 0.22f, 1.0f);
        sDisplay[cGame->getGoalsPlayer1() % 10]->setPosition(fPosX - 9.0f, fPosY + 140.0f, 0.0f);
        cRender->renderSprite(sDisplay[cGame->getGoalsPlayer1() % 10]);

        // time
        fPosX += 10.0f;
        fPosY += 20.0f;

        sDisplay[iTimeSec % 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeSec % 10]->setPosition(fPosX, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeSec % 10]);

        sDisplay[iTimeSec / 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeSec / 10]->setPosition(fPosX - 20, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeSec / 10]);

        sDisplay[iTimeMin % 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeMin % 10]->setPosition(fPosX - 50, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeMin % 10]);

        sDisplay[iTimeMin / 10]->setScale(0.15f, 0.15f, 1.0f);
        sDisplay[iTimeMin / 10]->setPosition(fPosX - 70, fPosY, 0.0f);
        cRender->renderSprite(sDisplay[iTimeMin / 10]);

        sDisplaySeparator->setScale(0.15f, 0.15f, 1.0f);
        sDisplaySeparator->setPosition(fPosX - 15.0f, fPosY, 0.0f);
        cRender->renderSprite(sDisplaySeparator);
    }

    // message
    if(sMessage[0] == '\0')
        return;

    if(sGlobal->bSplit)
        cRender->useViewPort(iPortMessages);
    else
        cRender->useViewPort(iPortFullScreen);

    lLabelMessage->setText(sMessage);
    cRender->renderLabel(lLabelMessage);
}

void CStateMatch::renderReplay()
{
    float fDiffX;
    float fDiffY;

    int iNextReplayPlayFrame = iCurrentReplayPlayFrame + 1;

    if(iNextReplayPlayFrame == REPLAY_FRAMES)
        iNextReplayPlayFrame = 0;

    // maths (interpolation)
    pPlayer1Position = sReplayFrame[iCurrentReplayPlayFrame].PositionPlayer1;

    if(sReplayFrame[iCurrentReplayPlayFrame].Interpolate)
    {
        fDiffX = (sReplayFrame[iNextReplayPlayFrame].PositionPlayer1.x - pPlayer1Position.x) / REPLAY_INTERPOL;
        fDiffY = (sReplayFrame[iNextReplayPlayFrame].PositionPlayer1.y - pPlayer1Position.y) / REPLAY_INTERPOL;
        pPlayer1Position.x += fDiffX * iCurrentReplayPlayFramePoint;
        pPlayer1Position.y += fDiffY * iCurrentReplayPlayFramePoint;
    }
    
    pPlayer2Position = sReplayFrame[iCurrentReplayPlayFrame].PositionPlayer2;

    if(sReplayFrame[iCurrentReplayPlayFrame].Interpolate)
    {
        fDiffX = (sReplayFrame[iNextReplayPlayFrame].PositionPlayer2.x - pPlayer2Position.x) / REPLAY_INTERPOL;
        fDiffY = (sReplayFrame[iNextReplayPlayFrame].PositionPlayer2.y - pPlayer2Position.y) / REPLAY_INTERPOL;
        pPlayer2Position.x += fDiffX * iCurrentReplayPlayFramePoint;
        pPlayer2Position.y += fDiffY * iCurrentReplayPlayFramePoint;
    }

    pPuckPosition = sReplayFrame[iCurrentReplayPlayFrame].PositionPuck;

    if(sReplayFrame[iCurrentReplayPlayFrame].Interpolate)
    {
        fDiffX = (sReplayFrame[iNextReplayPlayFrame].PositionPuck.x - pPuckPosition.x) / REPLAY_INTERPOL;
        fDiffY = (sReplayFrame[iNextReplayPlayFrame].PositionPuck.y - pPuckPosition.y) / REPLAY_INTERPOL;
        pPuckPosition.x += fDiffX * iCurrentReplayPlayFramePoint;
        pPuckPosition.y += fDiffY * iCurrentReplayPlayFramePoint;
    }

    // rendering
    cRender->useViewPort(iPortFullScreen);

    // room
    mMeshRoom->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshRoom);

    // table
    mMeshTable->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshTable);
    
    // player 1
    mMeshMallet1->setPosition(pPlayer1Position.x, 0.0f, -pPlayer1Position.y);
    cRender->renderMesh(mMeshMallet1);

    // player 2
    mMeshMallet2->setPosition(pPlayer2Position.x, 0.0f, -pPlayer2Position.y);
    cRender->renderMesh(mMeshMallet2);

    // puck
    if(sReplayFrame[iCurrentReplayPlayFrame].PuckVisible)
    {
        mMeshPuck->setPosition(pPuckPosition.x, 0.0f, -pPuckPosition.y);
        cRender->renderMesh(mMeshPuck);
    }

    // "Replay" text
    lLabelMessage->setText("REPLAY");
    cRender->renderLabel(lLabelMessage);

    // next interpolated point
    iCurrentReplayPlayFramePoint++;

    if(iCurrentReplayPlayFramePoint == REPLAY_INTERPOL)
    {
        iCurrentReplayPlayFramePoint = 0;
        iCurrentReplayPlayFrame++;

        if(iCurrentReplayPlayFrame == REPLAY_FRAMES)
            iCurrentReplayPlayFrame = 0;

        if(iCurrentReplayPlayFrame == iReplayFrame)
            bReplayMode = false;
    }
}

void CStateMatch::playSounds(int iEvent)
{
    GEVector3 vPosition;

    switch(iEvent)
    {
    case AH_EVENT_GOAL_P1:
    case AH_EVENT_GOAL_P2:

        vPosition.set(cGame->getRenderPositionPuck().x / AUDIO_RATIO, 0.0f,
                      -cGame->getRenderPositionPuck().y / AUDIO_RATIO);
        cAudio->setPosition(iChannelPuck, vPosition);
        cAudio->playSound(iSoundGoal[rand() % SOUNDS_GOAL], iChannelPuck);

        break;

    case AH_EVENT_PUCK_MALLET1_COLLISION:

        if(cAudio->isPlaying(iChannelMallet1))
            break;

        fImpactSquareSpeed = cGame->getSquareSpeedPlayer1() + cGame->getSquareSpeedPuck();

        if(fImpactSquareSpeed < SQSPEED_VERYSLOW)
            iSoundPlay = 0;
        else if(fImpactSquareSpeed < SQSPEED_SLOW)
            iSoundPlay = 1;
        else
            iSoundPlay = 2 + rand() % (SOUNDS_PUCK - 2);
        
        vPosition.set(cGame->getRenderPositionPlayer1().x / AUDIO_RATIO, 0.0f,
                      -cGame->getRenderPositionPlayer1().y / AUDIO_RATIO);
        cAudio->setPosition(iChannelMallet1, vPosition);
        cAudio->playSound(iSoundPuckMallet[iSoundPlay], iChannelMallet1);

        break;

    case AH_EVENT_PUCK_MALLET2_COLLISION:

        if(cAudio->isPlaying(iChannelMallet2))
            break;

        fImpactSquareSpeed = cGame->getSquareSpeedPlayer2() + cGame->getSquareSpeedPuck();

        if(fImpactSquareSpeed < SQSPEED_VERYSLOW)
            iSoundPlay = 0;
        else if(fImpactSquareSpeed < SQSPEED_SLOW)
            iSoundPlay = 1;
        else
            iSoundPlay = 2 + rand() % (SOUNDS_PUCK - 2);

        vPosition.set(cGame->getRenderPositionPlayer2().x / AUDIO_RATIO, 0.0f,
                      -cGame->getRenderPositionPlayer2().y / AUDIO_RATIO);
        cAudio->setPosition(iChannelMallet2, vPosition);
        cAudio->playSound(iSoundPuckMallet[iSoundPlay], iChannelMallet2);

        break;

    case AH_EVENT_PUCK_TABLE_TOP_COLLISION:
    case AH_EVENT_PUCK_TABLE_BOTTOM_COLLISION:
    case AH_EVENT_PUCK_TABLE_LEFT_COLLISION:
    case AH_EVENT_PUCK_TABLE_RIGHT_COLLISION:

        fImpactSquareSpeed = cGame->getSquareSpeedPuck();

        if(fImpactSquareSpeed < SQSPEED_VERYSLOW)
            iSoundPlay = 0;
        else if(fImpactSquareSpeed < SQSPEED_SLOW)
            iSoundPlay = 1;
        else
            iSoundPlay = 2 + rand() % (SOUNDS_TABLE - 2);

        vPosition.set(cGame->getRenderPositionPuck().x / AUDIO_RATIO, 0.0f,
                      -cGame->getRenderPositionPuck().y / AUDIO_RATIO);
        cAudio->setPosition(iChannelPuck, vPosition);
        cAudio->playSound(iSoundPuckTable[iSoundPlay], iChannelPuck);

        break;
    }

    // update audio engine
    if(iCurrentFrame % AUDIO_UPDATE == 0)
        cAudio->update();
}

void CStateMatch::replayMode()
{
    if(sGlobal->bMultiplayer)
        return;

    cGame->update();
    mCameraReplay[rand() % REPLAY_CAMERAS]->use();

    bReplayMode = true;
    iReplayFrame = iCurrentReplayRecordFrame;
    iCurrentReplayPlayFrame = iReplayFrame;
    iCurrentReplayPlayFramePoint = 0;

    // don't interpolate the last frame
    int iPreviousReplayRecordFrame = iCurrentReplayRecordFrame - 1;

    if(iPreviousReplayRecordFrame < 0)
        iPreviousReplayRecordFrame = REPLAY_FRAMES - 1;

    sReplayFrame[iPreviousReplayRecordFrame].Interpolate = false;
}

void CStateMatch::playAgain()
{
    // the client has nothing to do here
    if(sGlobal->bMultiplayer && !sGlobal->bServer)
        return;

    // reset goals
    cGame->update();
    cGame->setGoalsPlayer1(0);
    cGame->setGoalsPlayer2(0);

    // reset time
    iTimeMin = sGlobal->iGameMaxMinutes;
    iTimeSec = 0;

    // server: send goals and time to the client
    if(sGlobal->bMultiplayer)
    {
        // score
        sUDPBuffer[0] = MSG_GOAL;
        sUDPBuffer[1] = 0;
        sUDPBuffer[2] = 0;
        cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);

        // time
        sUDPBuffer[0] = MSG_TIME;
        sUDPBuffer[1] = sGlobal->iGameMaxMinutes;
        sUDPBuffer[2] = 0;
        cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);
    }

    // fill initial replay data
    for(int i = 0; i < REPLAY_FRAMES; i++)
    {
        sReplayFrame[i].PositionPlayer1 = cGame->getRenderPositionPlayer1();
        sReplayFrame[i].PositionPlayer2 = cGame->getRenderPositionPlayer2();
        sReplayFrame[i].PositionPuck = cGame->getRenderPositionPuck();
        sReplayFrame[i].PuckVisible = true;
        sReplayFrame[i].Interpolate = false;
    }
}

void CStateMatch::endOfTheMatch()
{
    if(sGlobal->bMultiplayer && sGlobal->bServer)
    {
        sUDPBuffer[0] = MSG_END;
        cServer->sendMessage(sClientAddress, sUDPBuffer, MSG_SIZE);
    }

    // player 1 wins
    if(cGame->getGoalsPlayer1() == sGlobal->iGameMaxGoals)
    {
        sprintf(sMessage, "Player 1 wins!");
        iSleepFrame = iCurrentFrame;
        bPlayAgain = true;
    }

    // player 2 wins
    else if(cGame->getGoalsPlayer2() == sGlobal->iGameMaxGoals)
    {
        sprintf(sMessage, "Player 2 wins!");
        iSleepFrame = iCurrentFrame;
        bPlayAgain = true;
    }

    // time over
    else if(iTimeMin == 0 && iTimeSec == 0)
    {
        if(cGame->getGoalsPlayer1() > cGame->getGoalsPlayer2())
            sprintf(sMessage, "Player 1 wins!");
        else if(cGame->getGoalsPlayer2() > cGame->getGoalsPlayer1())
            sprintf(sMessage, "Player 2 wins!");
        else
            sprintf(sMessage, "Draw");

        iSleepFrame = iCurrentFrame;
        bPlayAgain = true;
    }

    // ESC key
    else
    {
        stateChange(STATE_MENU);
    }
}
