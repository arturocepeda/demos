
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- sceneMenu.cpp ---
*/

#include "sceneMenu.h"
#include <stdio.h>

#define PI 3.141592f

#ifdef _KINECT_OPENNI_
char CSceneMenu::sKinectInfo[] = {"KINECT SENSOR INFORMATION\n\n"
                                  "1) When the message \"Waiting for player\" appears, wave to\n"
                                  "the sensor with the hand you want to use to play, moving it\n"
                                  "slowly to the right and left.\n\n"
                                  "2) Try to keep the hand separated from your body while you\n"
                                  "are playing.\n\n"
                                  "3) Don't move your hand up and down to play, but forward and\n"
                                  "backward: just imagine that you are playing on a real air\n"
                                  "hockey table!\n\n"
                                  "Press any key to start..."};
#else
char CSceneMenu::sKinectInfo[] = {"KINECT SENSOR INFORMATION\n\n"
                                  "1) When the message \"Waiting for player\" appears, place\n"
                                  "yourself in front of the sensor.\n\n"
                                  "2) Remember to select the appropriate hand for each player\n"
                                  "in the Settings menu (right-handed or left-handed).\n\n"
                                  "3) Try to keep the hand separated from your body while you\n"
                                  "are playing.\n\n"
                                  "4) Don't move your hand up and down to play, but forward and\n"
                                  "backward: just imagine that you are playing on a real air\n"
                                  "hockey table!\n\n"
                                  "Press any key to start..."};
#endif

CSceneMenu::CSceneMenu(GERendering* Render, GEAudio* Audio, void* GlobalData) : 
            GEScene(Render, Audio, GlobalData)
{
    cRender = Render;
    cAudio = Audio;
    sGlobal = (SGlobal*)GlobalData;
}

CSceneMenu::~CSceneMenu()
{
}

void CSceneMenu::init()
{
    iRegionOption = new unsigned int[MAX_OPTIONS];

    for(int i = 0; i < MAX_OPTIONS; i++)
        sOption[i] = new char[64];

    initRenderObjects();
    initSoundObjects();

    // settings values
    calculateDifficultyPercentages();
    _itoa(sGlobal->iPort, sPort, 10);

    // go to the main menu
    enterMenu(MENU_MAIN);
}

void CSceneMenu::initRenderObjects()
{
    // lighting
    cRender->setAmbientLight(GEColor((byte)100, 100, 100));

    // camera
    cRender->createCamera(&cCamera);
    fCameraOrbitTheta = (float)(rand() % 314);

    // meshes (background)
    cRender->createMesh(&mMeshMallet1);
    cRender->createMesh(&mMeshMallet2);
    cRender->createMesh(&mMeshTable);
    cRender->createMesh(&mMeshRoom);

    mMeshMallet1->loadFromFile("meshes\\mallet.x");
    mMeshMallet2->loadFromFile("meshes\\mallet.x");
    mMeshTable->loadFromFile("meshes\\table.x", "textures\\");
    mMeshRoom->loadFromFile("meshes\\room.x", "textures\\");

    // fonts
    iFontText = 0;
    iFontOption = 1;
    iFontSelected = 2;

    cRender->defineFont(iFontText, 36, 0, true, false, "Courier New");
    cRender->defineFont(iFontOption, 60, 0, true, false, "Courier New");
    cRender->defineFont(iFontSelected, 65, 0, true, false, "Courier New");

    // colors
    cColorOption.set((byte)140, 33, 27);
    cColorSelected.set((byte)252, 200, 200);
    bColorSelectedInc = false;

    // regions
    for(int i = 0; i < MAX_OPTIONS; i++)
    {
        iRegionOption[i] = i;

        cRender->defineRegion(iRegionOption[i], sGlobal->ScreenSizeY / 8 + (i * (sGlobal->ScreenSizeY / 12)), 
                                                sGlobal->ScreenSizeY / 8 + ((i + 1) * (sGlobal->ScreenSizeY / 12)),
                                                0, 
                                                sGlobal->ScreenSizeX);
    }

    iRegionFullScreen = MAX_OPTIONS;
    cRender->defineRegion(iRegionFullScreen, 0, sGlobal->ScreenSizeY, 0, sGlobal->ScreenSizeX);
}

void CSceneMenu::initSoundObjects()
{
}

void CSceneMenu::update()
{
    fCameraOrbitTheta += 0.002f;
    render();
}

void CSceneMenu::render()
{
    cRender->renderBegin();

    // background
    GEVector3 vRefPoint(0.0f, -0.5f, 0.0f);
    cCamera->orbit(vRefPoint, 6.0f, fCameraOrbitTheta, (PI * 0.5f) + 0.5f);
    cCamera->use();

    mMeshRoom->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshRoom);

    mMeshTable->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshTable);

    // selected color animation
    cColorSelected.R += (bColorSelectedInc)? 5: -5;

    if(cColorSelected.R > 250)
        bColorSelectedInc = false;
    else if(cColorSelected.R < 150)
        bColorSelectedInc = true;

    // menu options
    int iFirstRegion = (MAX_OPTIONS - iNumOptions) / 2;

    for(int i = 0; i < iNumOptions; i++)
    {
        cRender->renderText(sOption[i], 
                            (i == iSelectedOption)? iFontSelected: iFontOption,
                            (i == iSelectedOption)? cColorSelected: cColorOption, 
                            iRegionOption[iFirstRegion + i], GEAlignment::CenterCenter);
    }

#ifdef _KINECT_
    // kinect text info
    if(iCurrentMenu == MENU_KINECT_INFO)
        cRender->renderText(sKinectInfo, iFontText, cColorOption, iRegionFullScreen, GEAlignment::CenterCenter);
#endif

    cRender->renderEnd();
}

void CSceneMenu::release()
{
    for(int i = 0; i < MAX_OPTIONS; i++)
        delete[] sOption[i];

    delete[] iRegionOption;

    releaseSoundObjects();
    releaseRenderObjects();
}

void CSceneMenu::releaseRenderObjects()
{
    // camera
    cRender->releaseCamera(&cCamera);

    // fonts
    cRender->releaseFont(iFontText);
    cRender->releaseFont(iFontOption);
    cRender->releaseFont(iFontSelected);

    // regions
    for(int i = 0; i < MAX_OPTIONS; i++)
        cRender->releaseRegion(i);

    cRender->releaseRegion(iRegionFullScreen);

    // meshes
    delete mMeshMallet1;
    delete mMeshMallet2;
    delete mMeshTable;
    delete mMeshRoom;
}

void CSceneMenu::releaseSoundObjects()
{
}

void CSceneMenu::inputKey(char Key)
{
#ifdef _KINECT_
    if(iCurrentMenu == MENU_KINECT_INFO)
        sceneChange(SCENE_MATCH);
#endif

    switch(Key)
    {
    case KEY_ESC:

        switch(iCurrentMenu)
        {
        case MENU_MAIN:
            sceneChange(0);
            break;

        case MENU_PLAY:
            enterMenu(MENU_MAIN);
            break;

        case MENU_SETTINGS:
            enterMenu(MENU_MAIN);
            iSelectedOption = 1;
            break;

        case MENU_MULTIPLAYER:
            enterMenu(MENU_PLAY);
            iSelectedOption = 1;
            break;

        case MENU_MULTIPLAYER_SERVER:
            enterMenu(MENU_MULTIPLAYER);
            break;

        case MENU_MULTIPLAYER_CLIENT:
            enterMenu(MENU_MULTIPLAYER);
            iSelectedOption = 1;
            break;

        case MENU_SETTINGS_DIFFICULTY:
            enterMenu(MENU_SETTINGS);
            iSelectedOption = 5;
            break;

        case MENU_SETTINGS_KINECT:
            enterMenu(MENU_SETTINGS);
            iSelectedOption = 6;
            break;
        }

        break;
        
    case KEY_ENTER:

        selectOption(iSelectedOption);        

        break;

    case KEY_BACKSPACE:

        // port
        if((iCurrentMenu == MENU_MULTIPLAYER_SERVER && iSelectedOption == 0) ||
           (iCurrentMenu == MENU_MULTIPLAYER_CLIENT && iSelectedOption == 1))
        {
            if(strlen(sPort) > 0)
                sPort[strlen(sPort) - 1] = '\0';

            sGlobal->iPort = (strlen(sPort) > 0)? atoi(sPort): 0;

            refreshSettings();        
        }

        // server IP
        else if(iCurrentMenu == MENU_MULTIPLAYER_CLIENT && iSelectedOption == 0)
        {
            if(strlen(sGlobal->sIPServer) > 0)
            {
                sGlobal->sIPServer[strlen(sGlobal->sIPServer) - 1] = '\0';
                refreshSettings();
            }
        }

        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':

        // port
        if((iCurrentMenu == MENU_MULTIPLAYER_SERVER && iSelectedOption == 0) ||
           (iCurrentMenu == MENU_MULTIPLAYER_CLIENT && iSelectedOption == 1))
        {
            if(strlen(sPort) < 5)
            {
                sPort[strlen(sPort) + 1] = '\0';
                sPort[strlen(sPort)] = Key;
                sGlobal->iPort = atoi(sPort);
                refreshSettings();
            }
        }

        // server IP
        else if(iCurrentMenu == MENU_MULTIPLAYER_CLIENT && iSelectedOption == 0)
        {
            if(strlen(sGlobal->sIPServer) < 15)
            {
                sGlobal->sIPServer[strlen(sGlobal->sIPServer) + 1] = '\0';
                sGlobal->sIPServer[strlen(sGlobal->sIPServer)] = Key;
                refreshSettings();
            }
        }

        break;

    case -66:   // '.'
        
        // server IP
        if(iCurrentMenu == MENU_MULTIPLAYER_CLIENT && iSelectedOption == 0)
        {
            if(strlen(sGlobal->sIPServer) < 14)
            {
                sGlobal->sIPServer[strlen(sGlobal->sIPServer) + 1] = '\0';
                sGlobal->sIPServer[strlen(sGlobal->sIPServer)] = '.';
                refreshSettings();
            }
        }
        
        break;
        
    case KEY_UP:

        iSelectedOption--;

        if(iSelectedOption < 0)
            iSelectedOption = iNumOptions - 1;

        break;
        
    case KEY_DOWN:

        iSelectedOption++;

        if(iSelectedOption == iNumOptions)
            iSelectedOption = 0;

        break;

    case KEY_LEFT:

        switch(iCurrentMenu)
        {
        // settings
        case MENU_SETTINGS:

            switch(iSelectedOption)
            {
            // goals to win
            case 0:
                if(sGlobal->iGameMaxGoals > 1)
                    sGlobal->iGameMaxGoals--;
                break;

            // maximum time
            case 1:
                if(sGlobal->iGameMaxMinutes > 1)
                    sGlobal->iGameMaxMinutes--;
                break;

            // table air
            case 2:
                sGlobal->bTableAir = !sGlobal->bTableAir;
                break;

            // automatic replay
            case 3:
                sGlobal->bAutomaticReplay = !sGlobal->bAutomaticReplay;
                break;

            // split screen
            case 4:
                sGlobal->bSplit = !sGlobal->bSplit;
                break;
            }

            break;

        // multiplayer
        case MENU_MULTIPLAYER:

            // play with kinect
            if(iSelectedOption == 2)
                sGlobal->bKinect = !sGlobal->bKinect;

            break;

        // AI difficulty settings
        case MENU_SETTINGS_DIFFICULTY:

            switch(iSelectedOption)
            {
            // speed for shooting
            case 0:
                if(iPercentShooting > 0)
                    iPercentShooting--;
                break;

            // speed for defending
            case 1:
                if(iPercentDefending > 0)
                    iPercentDefending--;
                break;

            // speed for clearing
            case 2:
                if(iPercentClearing > 0)
                    iPercentClearing--;
                break;

            // attack/defense balance
            case 3:
                if(iPercentAttack > 0)
                    iPercentAttack--;
                break;
            }

            calculateDifficultyValues();

            break;

        // kinect settings
        case MENU_SETTINGS_KINECT:

            switch(iSelectedOption)
            {
            // table width
            case 0:
                if(sGlobal->fKinectWidth > (KINECT_WIDTH_MIN / 2))
                    sGlobal->fKinectWidth -= 5.0f;
                break;

            // minimum distance
            case 1:
                if(sGlobal->fKinectMinimumDistance > KINECT_MINDIST_MIN)
                    sGlobal->fKinectMinimumDistance -= 10.0f;
                break;

            // maximum distance
            case 2:
                if(sGlobal->fKinectMaximumDistance > KINECT_MAXDIST_MIN)
                {
                    sGlobal->fKinectMaximumDistance -= 10.0f;

                    if(sGlobal->fKinectMaximumDistance < sGlobal->fKinectMinimumDistance)
                        sGlobal->fKinectMinimumDistance = sGlobal->fKinectMaximumDistance;
                }
                break;

            // player 1 hand
            case 3:
                sGlobal->bPlayer1RightHanded = !sGlobal->bPlayer1RightHanded;
                break;

            // player 2 hand
            case 4:
                sGlobal->bPlayer2RightHanded = !sGlobal->bPlayer2RightHanded;
                break;
            }

            break;
        }

        refreshSettings();

        break;

    case KEY_RIGHT:

        switch(iCurrentMenu)
        {
        // settings
        case MENU_SETTINGS:

            switch(iSelectedOption)
            {
            // goals to win
            case 0:
                if(sGlobal->iGameMaxGoals < GAME_MAX_GOALS)
                    sGlobal->iGameMaxGoals++;
                break;

            // maximum time
            case 1:
                if(sGlobal->iGameMaxMinutes < GAME_MAX_MINUTES)
                    sGlobal->iGameMaxMinutes++;
                break;

            // table air
            case 2:
                sGlobal->bTableAir = !sGlobal->bTableAir;
                break;

            // automatic replay
            case 3:
                sGlobal->bAutomaticReplay = !sGlobal->bAutomaticReplay;
                break;

            // split screen
            case 4:
                sGlobal->bSplit = !sGlobal->bSplit;
                break;
            }

            break;

        // multiplayer
        case MENU_MULTIPLAYER:

            // play with kinect
            if(iSelectedOption == 2)
                sGlobal->bKinect = !sGlobal->bKinect;

            break;

        // AI difficulty settings
        case MENU_SETTINGS_DIFFICULTY:

            switch(iSelectedOption)
            {
            // speed for shooting
            case 0:
                if(iPercentShooting < 100)
                    iPercentShooting++;
                break;

            // speed for defending
            case 1:
                if(iPercentDefending < 100)
                    iPercentDefending++;
                break;

            // speed for clearing
            case 2:
                if(iPercentClearing < 100)
                    iPercentClearing++;
                break;

            // attack/defense balance
            case 3:
                if(iPercentAttack < 100)
                    iPercentAttack++;
                break;
            }

            calculateDifficultyValues();

            break;

        // kinect settings
        case MENU_SETTINGS_KINECT:

            switch(iSelectedOption)
            {
            // table width
            case 0:
                if(sGlobal->fKinectWidth < (KINECT_WIDTH_MAX / 2))
                    sGlobal->fKinectWidth += 5.0f;
                break;

            // minimum distance
            case 1:
                if(sGlobal->fKinectMinimumDistance < KINECT_MINDIST_MAX)
                {
                    sGlobal->fKinectMinimumDistance += 10.0f;

                    if(sGlobal->fKinectMinimumDistance > sGlobal->fKinectMaximumDistance)
                        sGlobal->fKinectMaximumDistance = sGlobal->fKinectMinimumDistance;
                }
                break;

            // maximum distance
            case 2:
                if(sGlobal->fKinectMaximumDistance < KINECT_MAXDIST_MAX)
                    sGlobal->fKinectMaximumDistance += 10.0f;
                break;

            // player 1 hand
            case 3:
                sGlobal->bPlayer1RightHanded = !sGlobal->bPlayer1RightHanded;
                break;

            // player 2 hand
            case 4:
                sGlobal->bPlayer2RightHanded = !sGlobal->bPlayer2RightHanded;
                break;
            }

            break;
        }

        refreshSettings();

        break;
    }
}

void CSceneMenu::inputMouseLeftButton()
{
}

void CSceneMenu::inputMouseRightButton()
{
}

void CSceneMenu::enterMenu(int iMenu)
{
    iCurrentMenu = iMenu;
    iSelectedOption = 0;

    switch(iMenu)
    {
    case MENU_MAIN:
        iNumOptions = 3;
        strcpy(sOption[0], "Play");
        strcpy(sOption[1], "Settings");
        strcpy(sOption[2], "Exit");
        break;

    case MENU_PLAY:
        iNumOptions = 2;
        strcpy(sOption[0], "Player vs CPU");
        strcpy(sOption[1], "Multiplayer");
#ifdef _KINECT_
        iNumOptions = 4;
        strcpy(sOption[2], "Kinect: Player vs CPU");
        strcpy(sOption[3], "Kinect: Player vs Player");
#endif
        break;

    case MENU_SETTINGS:
        iNumOptions = 6;
        refreshSettings();
        strcpy(sOption[5], "CPU difficulty...");
#ifdef _KINECT_
        iNumOptions = 7;
        strcpy(sOption[6], "Kinect settings...");
#endif
        break;

    case MENU_MULTIPLAYER:
        iNumOptions = 2;
        strcpy(sOption[0], "Create game");
        strcpy(sOption[1], "Join game");
#ifdef _KINECT_
        iNumOptions = 3;
        refreshSettings();
#endif
        break;

    case MENU_MULTIPLAYER_CLIENT:
        iNumOptions = 3;
        strcpy(sOption[2], "Join game");
        refreshSettings();
        break;

    case MENU_MULTIPLAYER_SERVER:
        iNumOptions = 2;
        strcpy(sOption[1], "Create game");
        refreshSettings();
        break;

    case MENU_SETTINGS_DIFFICULTY:
        iNumOptions = 4;
        refreshSettings();
        break;

    case MENU_SETTINGS_KINECT:
        iNumOptions = 3;
#ifndef _KINECT_OPENNI_
        iNumOptions = 5;
#endif
        refreshSettings();
        break;

    case MENU_KINECT_INFO:
        iNumOptions = 0;
        break;
    }
}

void CSceneMenu::selectOption(int iOption)
{
    switch(iCurrentMenu)
    {
    // MAIN
    case MENU_MAIN:

        switch(iOption)
        {
        // Play
        case 0:        
            enterMenu(MENU_PLAY);
            break;

        // Settings
        case 1:
            enterMenu(MENU_SETTINGS);
            break;

        // Exit
        case 2:
            sceneChange(0);
            break;
        }

        break;

    // PLAY
    case MENU_PLAY:

        switch(iOption)
        {
        // Player vs CPU
        case 0:        
            sGlobal->iHumanPlayers = 1;
            sGlobal->bMultiplayer = false;
            sGlobal->bKinect = false;
            sceneChange(SCENE_MATCH);
            break;

        // Multiplayer
        case 1:
            enterMenu(MENU_MULTIPLAYER);
            break;

#ifdef _KINECT_
        // Kinect: Player vs CPU
        case 2:
            sGlobal->iHumanPlayers = 1;
            sGlobal->bMultiplayer = false;
            sGlobal->bKinect = true;
            enterMenu(MENU_KINECT_INFO);
            break;

        // Kinect: Player vs Player
        case 3:
            sGlobal->iHumanPlayers = 2;
            sGlobal->bMultiplayer = false;
            sGlobal->bKinect = true;
            sGlobal->bSplit = true;
            enterMenu(MENU_KINECT_INFO);
            break;
#endif
        }

        break;

    // MULTIPLAYER
    case MENU_MULTIPLAYER:

        switch(iOption)
        {
        // create game
        case 0:
            enterMenu(MENU_MULTIPLAYER_SERVER);
            break;

        // join game
        case 1:
            enterMenu(MENU_MULTIPLAYER_CLIENT);
            break;
        }

        break;

    // MULTIPLAYER - CLIENT
    case MENU_MULTIPLAYER_CLIENT:
        
        // join game
        if(iOption == 2)
        {
            sGlobal->iHumanPlayers = 1;
            sGlobal->bMultiplayer = true;
            sGlobal->bServer = false;

            if(sGlobal->bKinect)
                enterMenu(MENU_KINECT_INFO);
            else
                sceneChange(SCENE_MATCH);
        }

        break;

    // MULTIPLAYER - SERVER
    case MENU_MULTIPLAYER_SERVER:

        // create game
        if(iOption == 1)
        {
            sGlobal->iHumanPlayers = 1;
            sGlobal->bMultiplayer = true;
            sGlobal->bServer = true;

            if(sGlobal->bKinect)
                enterMenu(MENU_KINECT_INFO);
            else
                sceneChange(SCENE_MATCH);
        }

        break;

    // SETTINGS
    case MENU_SETTINGS:

        // AI difficulty
        if(iOption == 5)
            enterMenu(MENU_SETTINGS_DIFFICULTY);

        // Kinect calibration
        else if(iOption == 6)
            enterMenu(MENU_SETTINGS_KINECT);

        break;
    }
}

void CSceneMenu::refreshSettings()
{
    switch(iCurrentMenu)
    {
    case MENU_SETTINGS:
        sprintf(sOption[0], "< Goals to win: %d >", sGlobal->iGameMaxGoals);
        sprintf(sOption[1], "< Match time: %d minute%s >", sGlobal->iGameMaxMinutes,
                                                           (sGlobal->iGameMaxMinutes > 1)? "s": "");
        sprintf(sOption[2], "< Table air: %s >", (sGlobal->bTableAir)? "yes": "no");
        sprintf(sOption[3], "< Automatic replay: %s >", (sGlobal->bAutomaticReplay)? "yes": "no");
        sprintf(sOption[4], "< Split screen: %s >", (sGlobal->bSplit)? "yes": "no");
        break;

    case MENU_MULTIPLAYER:
        sprintf(sOption[2], "< Play with Kinect: %s >", (sGlobal->bKinect)? "yes": "no");
        break;

    case MENU_MULTIPLAYER_SERVER:
        sprintf(sOption[0], "Port: %s_", sPort);
        break;

    case MENU_MULTIPLAYER_CLIENT:        
        sprintf(sOption[0], "Server IP: %s_", sGlobal->sIPServer);
        sprintf(sOption[1], "Port: %s_", sPort);
        break;

    case MENU_SETTINGS_DIFFICULTY:
        sprintf(sOption[0], "< Speed for shooting: %d%% >", iPercentShooting);
        sprintf(sOption[1], "< Speed for defending: %d%% >", iPercentDefending);
        sprintf(sOption[2], "< Speed for clearing: %d%% >", iPercentClearing);
        sprintf(sOption[3], "< Attack/Defense balance: %d/%d >", iPercentAttack, 100 - iPercentAttack);
        break;

    case MENU_SETTINGS_KINECT:
        sprintf(sOption[0], "< Table width: %.0f cm >", sGlobal->fKinectWidth / 10 * 2);
        sprintf(sOption[1], "< Minimum distance: %.0f cm >", sGlobal->fKinectMinimumDistance / 10);
        sprintf(sOption[2], "< Maximum distance: %.0f cm >", sGlobal->fKinectMaximumDistance / 10);
#ifndef _KINECT_OPENNI_
        sprintf(sOption[3], "< Player 1: %s >", (sGlobal->bPlayer1RightHanded)? "right-handed": "left-handed");
        sprintf(sOption[4], "< Player 2: %s >", (sGlobal->bPlayer2RightHanded)? "right-handed": "left-handed");
#endif
        break;
    }
}

void CSceneMenu::calculateDifficultyPercentages()
{
    iPercentShooting = (int)((sGlobal->cPlayer2AILevel.SpeedForShooting - AI_SHOOTING_MIN) / 
                            (AI_SHOOTING_MAX - AI_SHOOTING_MIN) * 100);
    iPercentDefending = (int)((sGlobal->cPlayer2AILevel.SpeedForDefending - AI_DEFENDING_MIN) / 
                             (AI_DEFENDING_MAX - AI_DEFENDING_MIN) * 100);
    iPercentClearing = (int)((sGlobal->cPlayer2AILevel.SpeedForClearing - AI_CLEARING_MIN) /
                            (AI_CLEARING_MAX - AI_CLEARING_MIN) * 100);
    iPercentAttack = (int)((sGlobal->cPlayer2AILevel.AttackThresholdSpeed - AI_ATTACK_MIN) /
                          (AI_ATTACK_MAX - AI_ATTACK_MIN) * 100);
}

void CSceneMenu::calculateDifficultyValues()
{
    sGlobal->cPlayer2AILevel.SpeedForShooting = (AI_SHOOTING_MAX - AI_SHOOTING_MIN) / 100 * iPercentShooting +
                                                AI_SHOOTING_MIN;

    sGlobal->cPlayer2AILevel.SpeedForDefending = (AI_DEFENDING_MAX - AI_DEFENDING_MIN) / 100 * iPercentDefending +
                                                 AI_DEFENDING_MIN;

    sGlobal->cPlayer2AILevel.SpeedForClearing = (AI_CLEARING_MAX - AI_CLEARING_MIN) / 100 * iPercentClearing +
                                                AI_CLEARING_MIN;

    sGlobal->cPlayer2AILevel.AttackThresholdSpeed = (AI_ATTACK_MAX - AI_ATTACK_MIN) / 100 * iPercentAttack +
                                                    AI_ATTACK_MIN;
}
