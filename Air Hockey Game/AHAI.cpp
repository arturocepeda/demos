
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AHAI.cpp ---
*/

#include "AHAI.h"
#include "Core/GEUtils.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>

//
//  AHAIData
//
bool AHAIData::targetPositionReached()
{
    return ((abs(pTargetPosition.x - pMyPosition.x) + abs(pTargetPosition.y - pMyPosition.y)) <= fSpeed);
}

float AHAIData::predictPositionX(float Y)
{
    double dX;
    GELine cLine(pPuckPosition.x, pPuckPosition.y,
             pPuckPosition.x + vPuckVelocity.x,
             pPuckPosition.y + vPuckVelocity.y);

    if(!cLine.can_calculate_x())
        return pMyPosition.x;

    dX = cLine.x(Y);

    // check for bounce
    int iBounces = 0;

    while(dX < -(AH_TABLE_WIDTH / 2))
    {
        ++iBounces;
        dX += AH_TABLE_WIDTH;
    }

    while(dX > (AH_TABLE_WIDTH / 2))
    {
        ++iBounces;
        dX -= AH_TABLE_WIDTH;
    }

    // check if the number of bounces is odd
    if(iBounces % 2 == 1)
        dX = -dX;

    return (float)dX;
}


//
//  AHAIState
//
void AHAIState::select()
{
    iTime = 0;
    iNextState = -1;
}

void AHAIState::incrementTime()
{
    iTime++;
}

int AHAIState::getCurrentTime()
{
    return iTime;
}

int AHAIState::getNextState()
{
    return iNextState;
}


//
//  AHAI
//
AHAI::AHAI(AHGame* Game, const AHAILevel& Level, bool Player1)
{
    sData.cGame = Game;
    sData.cLevel = Level;
    iStateStep = AHAIStateSteps::Enter;

    cStates = new AHAIState*[AHAIStates::Count];
    initializeStates();
    selectState(AHAIStates::Nothing);

    if(Player1)
    {
        sData.iPlayer = 1;
        sData.pTargetPosition = sData.cGame->getTablePositionPlayer1();
        sData.pTargetPosition.y = (AH_TABLE_HEIGHT / 2) - sData.pTargetPosition.y;
    }
    else
    {
        sData.iPlayer = 2;
        sData.pTargetPosition = sData.cGame->getTablePositionPlayer2();
        sData.pTargetPosition.y = sData.pTargetPosition.y + (AH_TABLE_HEIGHT / 2);
    }

    sData.fMinimumGoalDistance = AH_TABLE_HEIGHT / 10;
    sData.fAlertZoneHeight = AH_TABLE_HEIGHT / 6;
    sData.fBackSpace = AH_TABLE_HEIGHT / 20;
    sData.fNearSideBorder = AH_TABLE_WIDTH / 8;
    sData.fNearBackBorder = AH_TABLE_WIDTH / 8;
    sData.fHoleToShoot = AH_TABLE_WIDTH / 5;

    sData.fStopped = 10.0;
    sData.fHighSpeed = 80.0;
    sData.fLowSpeed = 20.0;
}

AHAI::~AHAI()
{
    releaseStates();
    delete[] cStates;
}

void AHAI::initializeStates()
{
    cStates[AHAIStates::Nothing] = new AHAIStateNothing(&sData);
    cStates[AHAIStates::GoToPuck] = new AHAIStateGoToPuck(&sData);
    cStates[AHAIStates::BeAlert] = new AHAIStateBeAlert(&sData);
    cStates[AHAIStates::GoToBack] = new AHAIStateGoToBack(&sData);
    cStates[AHAIStates::AvoidGoal] = new AHAIStateAvoidGoal(&sData);
    cStates[AHAIStates::PrepareShot] = new AHAIStatePrepareShot(&sData);
    cStates[AHAIStates::ShootToGoal] = new AHAIStateShootToGoal(&sData);
    cStates[AHAIStates::ClearPuck] = new AHAIStateClearPuck(&sData);
    cStates[AHAIStates::ShootToClear] = new AHAIStateShootToClear(&sData);
}

void AHAI::releaseStates()
{
    for(int i = 0; i < AHAIStates::Count; i++)
        delete cStates[i];
}

AHPoint AHAI::update()
{
    // table positions --> player positions
    if(sData.iPlayer == 1)
    {
        sData.pMyPosition = sData.cGame->getTablePositionPlayer1();
        sData.pMyPosition.y = (AH_TABLE_HEIGHT / 2) - sData.pMyPosition.y;

        sData.pOpponentPosition = sData.cGame->getTablePositionPlayer2();
        sData.pOpponentPosition.y = AH_TABLE_HEIGHT - ((AH_TABLE_HEIGHT / 2) + sData.pOpponentPosition.y);

        sData.pPuckPosition = sData.cGame->getTablePositionPuck();
        sData.pPuckPosition.y -= AH_TABLE_HEIGHT / 2;
        sData.pPuckPosition.y = -sData.pPuckPosition.y;

        sData.vPuckVelocity = sData.cGame->getVelocityPuck();
        sData.vPuckVelocity.y = -sData.vPuckVelocity.y;
    }
    else
    {
        sData.pMyPosition = sData.cGame->getTablePositionPlayer2();
        sData.pMyPosition.x = -sData.pMyPosition.x;
        sData.pMyPosition.y = sData.pMyPosition.y + (AH_TABLE_HEIGHT / 2);

        sData.pOpponentPosition = sData.cGame->getTablePositionPlayer1();
        sData.pOpponentPosition.x = -sData.pOpponentPosition.x;
        sData.pOpponentPosition.y = AH_TABLE_HEIGHT - ((AH_TABLE_HEIGHT / 2) - sData.pOpponentPosition.y);

        sData.pPuckPosition = sData.cGame->getTablePositionPuck();
        sData.pPuckPosition.x = -sData.pPuckPosition.x;
        sData.pPuckPosition.y += AH_TABLE_HEIGHT / 2;

        sData.vPuckVelocity = sData.cGame->getVelocityPuck();
        sData.vPuckVelocity.x = -sData.vPuckVelocity.x;
    }

    if(sData.cGame->checkEvent() == AH_EVENT_GOAL_P1 || sData.cGame->checkEvent() == AH_EVENT_GOAL_P2)
    {
        selectState(AHAIStates::Nothing);
        sData.pShotPoint = sData.pMyPosition;
        return myPointToTable(sData.pMyPosition);
    }

    // default speed
    sData.fSpeed = sData.cLevel.DefaultSpeed;

    // state execution
    cStates[iState]->incrementTime();

    if(iState == AHAIStates::Nothing)
    {
        // timeout
        if(cStates[iState]->getCurrentTime() > 200)
            selectState(AHAIStates::GoToPuck);

        // select new state
        else
            chooseNewState();
    }

    switch(iStateStep)
    {
    case AHAIStateSteps::Execute:
        cStates[iState]->run();
        if(cStates[iState]->getNextState() > -1)
            selectState(cStates[iState]->getNextState());
        break;

    case AHAIStateSteps::Enter:
        cStates[iState]->enter();
        iStateStep = AHAIStateSteps::Execute;    
    }

    // go to the target position
    return myPointToTable(go());
}

void AHAI::selectState(int State)
{
    iState = State;
    iStateStep = AHAIStateSteps::Enter;
    cStates[iState]->select();
}

void AHAI::chooseNewState()
{
    // PUCK IN MY ZONE
    if(sData.pPuckPosition.y <= (AH_TABLE_HEIGHT / 2))
    {
        // high priority: if the puck is going to enter into my goal, I clear it
        if(sData.pPuckPosition.y < sData.fNearBackBorder &&
           abs(sData.pPuckPosition.x) < (AH_TABLE_WIDTH / 8) && sData.vPuckVelocity.y < 0.0)
        {
            selectState(AHAIStates::ClearPuck);
        }

        // puck is stopped or almost
        else if(abs(sData.vPuckVelocity.x) < sData.fStopped && abs(sData.vPuckVelocity.y) < sData.fStopped)
        {
            // there is enough space to shoot
            if(sData.pPuckPosition.y > sData.fNearBackBorder &&
               (sData.pPuckPosition.y - sData.pMyPosition.y) > AH_MALLET_RADIUS)
            {
                selectState(AHAIStates::PrepareShot);
            }

            // there's not enough space to shoot
            else
            {
                if(sData.pMyPosition.y > sData.fNearBackBorder)
                    selectState(AHAIStates::GoToBack);
            }
        }

        // puck is going towards the opponent
        else if(sData.vPuckVelocity.y >= 0.0)
        {
            // low speed
            if(sData.vPuckVelocity.y < sData.fLowSpeed &&
               abs(sData.pPuckPosition.x) < (AH_TABLE_WIDTH / 2 - sData.fNearSideBorder))
            {
                selectState(AHAIStates::PrepareShot);
            }

            // normal or high speed
            else
            {
                selectState(AHAIStates::BeAlert);
            }
        }

        // puck is coming towards my goal
        else if(sData.vPuckVelocity.y < 0.0)
        {
            // high speed
            if(abs(sData.vPuckVelocity.y) > sData.cLevel.AttackThresholdSpeed)
            {
                selectState(AHAIStates::AvoidGoal);
            }

            // normal or low speed
            else if(abs(sData.pPuckPosition.x) < (AH_TABLE_WIDTH / 2 - sData.fNearSideBorder) && 
                    (sData.pPuckPosition.y - sData.pMyPosition.y) > sData.fHoleToShoot)
            {
                selectState(AHAIStates::PrepareShot);
            }
        }
    }

    // PUCK OUT OF MY ZONE
    else
    {
        // puck is stopped or going towards the opponent
        if(sData.vPuckVelocity.y >= 0.0)
            selectState(AHAIStates::BeAlert);

        // puck is coming to me
        else 
        {
            // high speed
            if(abs(sData.vPuckVelocity.y) > sData.fHighSpeed)
                selectState(AHAIStates::AvoidGoal);

            // low speed
            else
                selectState(AHAIStates::BeAlert);
        }
    }
}

AHPoint AHAI::myPointToTable(AHPoint pPoint)
{
    AHPoint pTablePosition;

    pTablePosition.x = pPoint.x;

    // my positions --> table positions
    if(sData.iPlayer == 1)
    {
        pTablePosition.y = (AH_TABLE_HEIGHT / 2) - pPoint.y;
    }
    else
    {
        pTablePosition.x = -pTablePosition.x;
        pTablePosition.y = pPoint.y - (AH_TABLE_HEIGHT / 2);
    }

    return pTablePosition;
}

AHPoint AHAI::go()
{
    if(sData.targetPositionReached())
        return sData.pMyPosition;

    AHPoint pNewPosition;
    AHVector vTargetDirection;

    vTargetDirection.x = sData.pTargetPosition.x - sData.pMyPosition.x;
    vTargetDirection.y = sData.pTargetPosition.y - sData.pMyPosition.y;

    setVectorLength(&vTargetDirection, sData.fSpeed);

    pNewPosition = sData.pMyPosition;
    pNewPosition.x += vTargetDirection.x;
    pNewPosition.y += vTargetDirection.y;

    return pNewPosition;
}


//
//  AHAIStateNothing
//
void AHAIStateNothing::enter()
{
}

void AHAIStateNothing::run()
{
    sData->fSpeed = sData->cLevel.DefaultSpeed * 0.5f;
    sData->pTargetPosition.x = 0.0f;
    sData->pTargetPosition.y = sData->fNearBackBorder * 0.5f;    
}


//
//  AHAIStateGoToPuck
//
void AHAIStateGoToPuck::enter()
{
}

void AHAIStateGoToPuck::run()
{
    sData->pTargetPosition.x = sData->pPuckPosition.x;
    sData->pTargetPosition.y = sData->pPuckPosition.y;

    sData->fSpeed = sData->cLevel.DefaultSpeed * 2.0f;

    if(sData->pPuckPosition.y >= (AH_TABLE_HEIGHT * 0.5f) ||                // puck out of my zone
        sData->cGame->checkEvent() == AH_EVENT_PUCK_MALLET1_COLLISION ||    // puck reached
        sData->cGame->checkEvent() == AH_EVENT_PUCK_MALLET2_COLLISION)
    {
        iNextState = AHAIStates::Nothing;
    }
}


//
//  AHAIStateBeAlert
//
void AHAIStateBeAlert::enter()
{
    // average point between puck and goal in my zone
    sData->pTargetPosition.x = sData->pPuckPosition.x * 0.25f;
    sData->pTargetPosition.y = sData->fMinimumGoalDistance + rand() % (int)floor(sData->fAlertZoneHeight + 0.5f);
    sData->fSpeed = sData->cLevel.DefaultSpeed;
}

void AHAIStateBeAlert::run()
{
    if((sData->targetPositionReached() && (sData->vPuckVelocity.x != 0.0 || sData->vPuckVelocity.y != 0.0)) ||
        sData->vPuckVelocity.y < 0.0)
    {
        iNextState = AHAIStates::Nothing;
    }
}


//
//  AHAIStateGoToBack
//
void AHAIStateGoToBack::enter()
{
    sData->pTargetPosition.x = 0.0f;
    sData->pTargetPosition.y = sData->fNearBackBorder;
}

void AHAIStateGoToBack::run()
{
    sData->fSpeed = sData->cLevel.SpeedForDefending;

    if(sData->targetPositionReached() ||
       (sData->pPuckPosition.y < sData->pMyPosition.y && abs(sData->pPuckPosition.x) < (AH_TABLE_WIDTH / 6)))
    {
        iNextState = AHAIStates::Nothing;
    }
}


//
//  AHAIStateAvoidGoal
//
float AHAIStateAvoidGoal::fSpeedSlow = 5.0f;

void AHAIStateAvoidGoal::enter()
{
    // target position
    sData->pTargetPosition.y = std::max(sData->pMyPosition.y - sData->fBackSpace, sData->fMinimumGoalDistance);
    sData->pTargetPosition.x = sData->predictPositionX(sData->pTargetPosition.y);
        
    // error
    sData->pTargetPosition.x += rand() % 50;
    sData->pTargetPosition.y += rand() % 50;

    // speed
    fTimeForThePuckToCome = (sData->pTargetPosition.y - sData->pPuckPosition.y) / sData->vPuckVelocity.y;
    fTimeForThePuckToCome = abs(fTimeForThePuckToCome);
    fTimeForThePuckToCome -= 2.0;        // state change frames

    if(fTimeForThePuckToCome < 0.0)
    {
        sData->fCalculatedSpeed = sData->cLevel.SpeedForDefending;
    }
    else
    {
        fDistanceToTarget = (sData->pTargetPosition.x - sData->pMyPosition.x) * (sData->pTargetPosition.x - sData->pMyPosition.x);
        fDistanceToTarget += (sData->pTargetPosition.y - sData->pMyPosition.y) * (sData->pTargetPosition.y - sData->pMyPosition.y);
        fDistanceToTarget = sqrt(fDistanceToTarget);

        sData->fCalculatedSpeed = fDistanceToTarget / fTimeForThePuckToCome;
        sData->fCalculatedSpeed = std::min(sData->fCalculatedSpeed, sData->cLevel.SpeedForDefending);
    }
}

void AHAIStateAvoidGoal::run()
{
    sData->fSpeed = sData->fCalculatedSpeed;

    if(sData->pPuckPosition.y <= sData->pMyPosition.y || sData->vPuckVelocity.y >= -fSpeedSlow)
        iNextState = AHAIStates::Nothing;
}


//
//  AHAIStateClearPuck
//
void AHAIStateClearPuck::enter()
{
    sData->pTargetPosition.x = (sData->pMyPosition.x < 0)? -AH_TABLE_WIDTH / 6.0f: AH_TABLE_WIDTH / 6.0f;
    sData->pTargetPosition.y = AH_MALLET_RADIUS;
}

void AHAIStateClearPuck::run()
{
    if(sData->targetPositionReached() || 
        sData->pPuckPosition.y > sData->fNearBackBorder ||
        abs(sData->pPuckPosition.x) > (AH_TABLE_WIDTH / 6))
    {
        iNextState = AHAIStates::ShootToClear;
    }

    sData->fSpeed = sData->cLevel.SpeedForClearing;
}


//
//  AHAIStateShootToClear
//
void AHAIStateShootToClear::enter()
{
    sData->pTargetPosition.y = sData->pMyPosition.y;
    sData->pTargetPosition.x = (sData->pMyPosition.x > 0)? -AH_TABLE_WIDTH / 3.0f: AH_TABLE_WIDTH / 3.0f;
}

void AHAIStateShootToClear::run()
{
    sData->fSpeed = sData->cLevel.SpeedForClearing;
        
    if(sData->targetPositionReached())           
        iNextState = AHAIStates::Nothing;
}


//
//  AHAIStatePrepareShot
//
AHPoint AHAIStatePrepareShot::calculateGoalPoint()
{
    AHPoint pGoal;

    pGoal.x = (float)(rand() % (AH_TABLE_WIDTH / 3) - AH_TABLE_WIDTH / 6);
    pGoal.y = AH_TABLE_HEIGHT;

    return pGoal;
}

void AHAIStatePrepareShot::enter()
{
    // default: direct shot
    sData->pShotPoint = calculateGoalPoint();
    iChoice = rand() % 2;

    // with bounce
    if(iChoice == 1)
    {
        // against the left wall
        if(sData->pMyPosition.x >= 0.0 && sData->pPuckPosition.x < -sData->fHoleToShoot)
            sData->pShotPoint.x -= AH_TABLE_WIDTH * (1 + rand() % 2);    // one or two bounces

        // against the right wall
        else if(sData->pMyPosition.x < 0.0 && sData->pPuckPosition.x > sData->fHoleToShoot)
            sData->pShotPoint.x += AH_TABLE_WIDTH * (1 + rand() % 2);
    }

    // puck's future position
    sData->fImpulseDistance = sData->fBackSpace;
    pPuckFuturePosition.y = sData->pMyPosition.y + sData->fImpulseDistance;
    pPuckFuturePosition.x = sData->predictPositionX(pPuckFuturePosition.y);

    // shot line
    cShotLine = new GELine(sData->pShotPoint.x, sData->pShotPoint.y, pPuckFuturePosition.x, pPuckFuturePosition.y);
    sData->pTargetPosition = sData->pMyPosition;

    if(cShotLine->can_calculate_x())
        sData->pTargetPosition.x = (float)cShotLine->x(sData->pTargetPosition.y);

    // speed
    if(abs(sData->vPuckVelocity.y) > sData->fStopped)
    {
        fTimeForThePuckToCome = (pPuckFuturePosition.y - sData->pPuckPosition.y) / sData->vPuckVelocity.y;
        fTimeForThePuckToCome = abs(fTimeForThePuckToCome);
        fTimeForThePuckToCome -= 2.0;        // state change frames

        if(fTimeForThePuckToCome < 0.0)
        {
            sData->fCalculatedSpeed = sData->cLevel.SpeedForShooting;
        }
        else
        {
            fDistanceToTarget = (sData->pTargetPosition.x - sData->pMyPosition.x) * (sData->pTargetPosition.x - sData->pMyPosition.x);
            fDistanceToTarget += (sData->pTargetPosition.y - sData->pMyPosition.y) * (sData->pTargetPosition.y - sData->pMyPosition.y);
            fDistanceToTarget = sqrt(fDistanceToTarget);

            sData->fCalculatedSpeed = fDistanceToTarget / fTimeForThePuckToCome;
            sData->fCalculatedSpeed = std::min(sData->cLevel.SpeedForShooting, sData->fCalculatedSpeed);
        }
    }
    else
    {
        sData->fCalculatedSpeed = sData->cLevel.DefaultSpeed;
    }
}

void AHAIStatePrepareShot::run()
{
    sData->fSpeed = sData->fCalculatedSpeed;

    if(sData->targetPositionReached() ||                       // ready for the shot
        sData->pPuckPosition.y <= sData->pMyPosition.y ||             // the puck has escaped
        sData->pPuckPosition.y > (AH_TABLE_HEIGHT / 2) ||      // the puck is out of my zone
        abs(sData->vPuckVelocity.y) < sData->fStopped)                // the puck is stopped
    {
        delete cShotLine;

        if(sData->pPuckPosition.y > sData->pMyPosition.y)
            iNextState = AHAIStates::ShootToGoal;
        else
            iNextState = AHAIStates::Nothing;
    }    
}


//
//  AHAIStateShootToGoal
//
void AHAIStateShootToGoal::enter()
{
}

void AHAIStateShootToGoal::run()
{
    sData->pTargetPosition = sData->pPuckPosition;
    sData->fSpeed = sData->cLevel.SpeedForShooting;

    if(sData->pPuckPosition.y < sData->pMyPosition.y ||                                               // the puck is behind me
        (sData->cGame->checkEvent() == AH_EVENT_PUCK_MALLET2_COLLISION && sData->iPlayer == 2) ||     // the puck collides with the mallet
        (sData->cGame->checkEvent() == AH_EVENT_PUCK_MALLET1_COLLISION && sData->iPlayer == 1) ||
        sData->pPuckPosition.y > (AH_TABLE_HEIGHT / 2) ||                                             // the puck is out of my zone
        abs(sData->pPuckPosition.x) > (AH_TABLE_WIDTH / 2 + sData->fNearSideBorder) ||                // the puck is too near from a side border
        sData->pPuckPosition.y < sData->fNearBackBorder)                                              // the puck is too near from the back border
    {
        iNextState = AHAIStates::Nothing;
    }
}
