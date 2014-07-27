
/*
    Arturo Cepeda Pérez

    AirHockey3d - Using the Air Hockey Game Library

    --- stateMenu.cpp ---
*/

#include "stateMenu.h"
#include <stdio.h>

#define PI 3.141592f

using namespace GE;
using namespace GE::States;
using namespace GE::Rendering;
using namespace GE::Audio;

#ifdef _KINECT_OPENNI_
char CStateMenu::sKinectInfo[] = {"KINECT SENSOR INFORMATION\n\n"
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
char CStateMenu::sKinectInfo[] = {"KINECT SENSOR INFORMATION\n\n"
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

CStateMenu::CStateMenu(RenderSystem* Render, AudioSystem* Audio, void* GlobalData)
    : State(Render, Audio, GlobalData)
{
    cRender = Render;
    cAudio = Audio;
    sGlobal = (SGlobal*)GlobalData;
}

CStateMenu::~CStateMenu()
{
}

void CStateMenu::internalInit()
{
    initRenderObjects();
    initSoundObjects();

    // settings values
    calculateDifficultyPercentages();
    _itoa(sGlobal->iPort, sPort, 10);

    // go to the main menu
    enterMenu(MENU_MAIN);
}

void CStateMenu::initRenderObjects()
{
    // lighting
    cRender->setAmbientLightColor(Color((byte)100, 100, 100));

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

    cRender->defineFont(iFontText, "Courier New", 36.0f, true);
    cRender->defineFont(iFontOption, "Courier New", 60.0f, true);
    cRender->defineFont(iFontSelected, "Courier New", 65.0f, true);

    // colors
    cColorOption.set((byte)140, 33, 27);
    cColorSelected.set((byte)252, 200, 200);
    bColorSelectedInc = false;

    // labels
    cRender->createLabel(&lLabelKinectInfo, iFontText, Alignment::CenterCenter,
                         Vector2((float)sGlobal->ScreenSizeX, (float)sGlobal->ScreenSizeY), sKinectInfo);
    lLabelKinectInfo->setColor(cColorOption);

    for(int i = 0; i < MAX_OPTIONS; i++)
    {
        cRender->createLabel(&lLabelOption[i], iFontOption, Alignment::CenterCenter,
                             Vector2((float)sGlobal->ScreenSizeX, sGlobal->ScreenSizeY / 12.0f));
    }
}

void CStateMenu::initSoundObjects()
{
}

void CStateMenu::update(float DeltaTime)
{
    // camera orbit
    fCameraOrbitTheta += DeltaTime * 0.0002f;

    // selected color animation
    cColorSelected.R += (bColorSelectedInc ? DeltaTime : -DeltaTime) * 0.001f;

    if(cColorSelected.R > 0.95f)
        bColorSelectedInc = false;
    else if(cColorSelected.R < 0.6f)
        bColorSelectedInc = true;
}

void CStateMenu::render()
{
    // background
    Vector3 vRefPoint(0.0f, -0.5f, 0.0f);
    cCamera->orbit(vRefPoint, 6.0f, fCameraOrbitTheta, (PI * 0.5f) + 0.5f);
    cCamera->use();

    mMeshRoom->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshRoom);

    mMeshTable->setPosition(0.0f, -0.015f, 0.0f);
    cRender->renderMesh(mMeshTable);

    // menu options
    int iFirstRegion = (MAX_OPTIONS - iNumOptions) / 2;
    float fPositionY;

    for(int i = 0; i < iNumOptions; i++)
    {
        fPositionY = (float)sGlobal->ScreenSizeY / 8 + ((i + iFirstRegion) * (sGlobal->ScreenSizeY / 12));
        lLabelOption[i]->setPosition(0.0f, fPositionY);
        lLabelOption[i]->setColor(i == iSelectedOption ? cColorSelected : cColorOption);
        cRender->renderLabel(lLabelOption[i]);
    }

#ifdef _KINECT_
    // kinect text info
    if(iCurrentMenu == MENU_KINECT_INFO)
        cRender->renderLabel(lLabelKinectInfo);
#endif
}

void CStateMenu::release()
{
    releaseSoundObjects();
    releaseRenderObjects();
}

void CStateMenu::releaseRenderObjects()
{
    // camera
    cRender->releaseCamera(&cCamera);

    // fonts
    cRender->releaseFont(iFontText);
    cRender->releaseFont(iFontOption);
    cRender->releaseFont(iFontSelected);

    // labels
    cRender->releaseLabel(&lLabelKinectInfo);

    for(int i = 0; i < MAX_OPTIONS; i++)
        cRender->releaseLabel(&lLabelOption[i]);

    // meshes
    delete mMeshMallet1;
    delete mMeshMallet2;
    delete mMeshTable;
    delete mMeshRoom;
}

void CStateMenu::releaseSoundObjects()
{
}

void CStateMenu::inputKeyPress(char Key)
{
#ifdef _KINECT_
    if(iCurrentMenu == MENU_KINECT_INFO)
        stateChange(STATE_MATCH);
#endif

    switch(Key)
    {
    case KEY_ESC:

        switch(iCurrentMenu)
        {
        case MENU_MAIN:
            stateChange(0);
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

void CStateMenu::enterMenu(int iMenu)
{
    iCurrentMenu = iMenu;
    iSelectedOption = 0;

    switch(iMenu)
    {
    case MENU_MAIN:
        iNumOptions = 3;
        lLabelOption[0]->setText("Play");
        lLabelOption[1]->setText("Settings");
        lLabelOption[2]->setText("Exit");
        break;

    case MENU_PLAY:
        iNumOptions = 2;
        lLabelOption[0]->setText("Player vs CPU");
        lLabelOption[1]->setText("Multiplayer");
#ifdef _KINECT_
        iNumOptions = 4;
        lLabelOption[2]->setText("Kinect: Player vs CPU");
        lLabelOption[3]->setText("Kinect: Player vs Player");
#endif
        break;

    case MENU_SETTINGS:
        iNumOptions = 6;
        refreshSettings();
        lLabelOption[5]->setText("CPU difficulty...");
#ifdef _KINECT_
        iNumOptions = 7;
        lLabelOption[6]->setText("Kinect settings...");
#endif
        break;

    case MENU_MULTIPLAYER:
        iNumOptions = 2;
        lLabelOption[0]->setText("Create game");
        lLabelOption[1]->setText("Join game");
#ifdef _KINECT_
        iNumOptions = 3;
        refreshSettings();
#endif
        break;

    case MENU_MULTIPLAYER_CLIENT:
        iNumOptions = 3;
        lLabelOption[2]->setText("Join game");
        refreshSettings();
        break;

    case MENU_MULTIPLAYER_SERVER:
        iNumOptions = 2;
        lLabelOption[1]->setText("Create game");
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

void CStateMenu::selectOption(int iOption)
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
            stateChange(0);
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
            stateChange(STATE_MATCH);
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
                stateChange(STATE_MATCH);
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
                stateChange(STATE_MATCH);
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

void CStateMenu::refreshSettings()
{
    switch(iCurrentMenu)
    {
    case MENU_SETTINGS:
        sprintf(sOptionText, "< Goals to win: %d >", sGlobal->iGameMaxGoals);
        lLabelOption[0]->setText(sOptionText);
        sprintf(sOptionText, "< Match time: %d minute%s >", sGlobal->iGameMaxMinutes,
                                                           (sGlobal->iGameMaxMinutes > 1)? "s": "");
        lLabelOption[1]->setText(sOptionText);
        sprintf(sOptionText, "< Table air: %s >", (sGlobal->bTableAir)? "yes": "no");
        lLabelOption[2]->setText(sOptionText);
        sprintf(sOptionText, "< Automatic replay: %s >", (sGlobal->bAutomaticReplay)? "yes": "no");
        lLabelOption[3]->setText(sOptionText);
        sprintf(sOptionText, "< Split screen: %s >", (sGlobal->bSplit)? "yes": "no");
        lLabelOption[4]->setText(sOptionText);
        break;

    case MENU_MULTIPLAYER:
        sprintf(sOptionText, "< Play with Kinect: %s >", (sGlobal->bKinect)? "yes": "no");
        lLabelOption[2]->setText(sOptionText);
        break;

    case MENU_MULTIPLAYER_SERVER:
        sprintf(sOptionText, "Port: %s_", sPort);
        lLabelOption[0]->setText(sOptionText);
        break;

    case MENU_MULTIPLAYER_CLIENT:        
        sprintf(sOptionText, "Server IP: %s_", sGlobal->sIPServer);
        lLabelOption[0]->setText(sOptionText);
        sprintf(sOptionText, "Port: %s_", sPort);
        lLabelOption[1]->setText(sOptionText);
        break;

    case MENU_SETTINGS_DIFFICULTY:
        sprintf(sOptionText, "< Speed for shooting: %d%% >", iPercentShooting);
        lLabelOption[0]->setText(sOptionText);
        sprintf(sOptionText, "< Speed for defending: %d%% >", iPercentDefending);
        lLabelOption[1]->setText(sOptionText);
        sprintf(sOptionText, "< Speed for clearing: %d%% >", iPercentClearing);
        lLabelOption[2]->setText(sOptionText);
        sprintf(sOptionText, "< Attack/Defense balance: %d/%d >", iPercentAttack, 100 - iPercentAttack);
        lLabelOption[3]->setText(sOptionText);
        break;

    case MENU_SETTINGS_KINECT:
        sprintf(sOptionText, "< Table width: %.0f cm >", sGlobal->fKinectWidth / 10 * 2);
        lLabelOption[0]->setText(sOptionText);
        sprintf(sOptionText, "< Minimum distance: %.0f cm >", sGlobal->fKinectMinimumDistance / 10);
        lLabelOption[1]->setText(sOptionText);
        sprintf(sOptionText, "< Maximum distance: %.0f cm >", sGlobal->fKinectMaximumDistance / 10);
        lLabelOption[2]->setText(sOptionText);
#ifndef _KINECT_OPENNI_
        sprintf(sOptionText, "< Player 1: %s >", (sGlobal->bPlayer1RightHanded)? "right-handed": "left-handed");
        lLabelOption[3]->setText(sOptionText);
        sprintf(sOptionText, "< Player 2: %s >", (sGlobal->bPlayer2RightHanded)? "right-handed": "left-handed");
        lLabelOption[4]->setText(sOptionText);
#endif
        break;
    }
}

void CStateMenu::calculateDifficultyPercentages()
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

void CStateMenu::calculateDifficultyValues()
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
