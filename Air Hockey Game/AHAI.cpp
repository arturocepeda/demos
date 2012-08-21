
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AHAI.cpp ---
*/

#include "AHAI.h"
#include "utils.h"
#include <cmath>
#include <stdlib.h>

AHAI::AHAI(AHGame* Game, const AHAILevel& Level, bool Player1)
{
    cGame = Game;
    cLevel = Level;
    iState = AH_STATE_NOTHING;
    iStateStep = AH_STATE_STEP_ENTER;

    if(Player1)
    {
        iPlayer = 1;
        pTargetPosition = cGame->getTablePositionPlayer1();
        pTargetPosition.y = (AH_TABLE_HEIGHT / 2) - pTargetPosition.y;
    }
    else
    {
        iPlayer = 2;
        pTargetPosition = cGame->getTablePositionPlayer2();
        pTargetPosition.y = pTargetPosition.y + (AH_TABLE_HEIGHT / 2);
    }

    fMinimumGoalDistance = AH_TABLE_HEIGHT / 10;
    fAlertZoneHeight = AH_TABLE_HEIGHT / 6;
    fBackSpace = AH_TABLE_HEIGHT / 20;
    fNearSideBorder = AH_TABLE_WIDTH / 8;
    fNearBackBorder = AH_TABLE_WIDTH / 8;
    fHoleToShoot = AH_TABLE_WIDTH / 5;

    fStopped = 10.0;
    fHighSpeed = 80.0;
    fLowSpeed = 20.0;
}

AHAI::~AHAI()
{
}

AHPoint AHAI::update()
{
    // table positions --> player positions
    if(iPlayer == 1)
    {
        pMyPosition = cGame->getTablePositionPlayer1();
        pMyPosition.y = (AH_TABLE_HEIGHT / 2) - pMyPosition.y;

        pOpponentPosition = cGame->getTablePositionPlayer2();
        pOpponentPosition.y = AH_TABLE_HEIGHT - ((AH_TABLE_HEIGHT / 2) + pOpponentPosition.y);

        pPuckPosition = cGame->getTablePositionPuck();
        pPuckPosition.y -= AH_TABLE_HEIGHT / 2;
        pPuckPosition.y = -pPuckPosition.y;

        vPuckVelocity = cGame->getVelocityPuck();
        vPuckVelocity.y = -vPuckVelocity.y;
    }
    else
    {
        pMyPosition = cGame->getTablePositionPlayer2();
        pMyPosition.x = -pMyPosition.x;
        pMyPosition.y = pMyPosition.y + (AH_TABLE_HEIGHT / 2);

        pOpponentPosition = cGame->getTablePositionPlayer1();
        pOpponentPosition.x = -pOpponentPosition.x;
        pOpponentPosition.y = AH_TABLE_HEIGHT - ((AH_TABLE_HEIGHT / 2) - pOpponentPosition.y);

        pPuckPosition = cGame->getTablePositionPuck();
        pPuckPosition.x = -pPuckPosition.x;
        pPuckPosition.y += AH_TABLE_HEIGHT / 2;

        vPuckVelocity = cGame->getVelocityPuck();
        vPuckVelocity.x = -vPuckVelocity.x;
    }

    if(cGame->checkEvent() == AH_EVENT_GOAL_P1 || cGame->checkEvent() == AH_EVENT_GOAL_P2)
    {
        selectState(AH_STATE_NOTHING);
        pShotPoint = pMyPosition;
        return myPointToTable(pMyPosition);
    }

    // default speed
    fSpeed = cLevel.DefaultSpeed;

    // state execution
    ++iStateTime;

    // timeout
    if(iState == AH_STATE_NOTHING && iStateTime > 200)
        selectState(AH_STATE_GO_TO_PUCK);

    switch(iState)
    {
    case AH_STATE_NOTHING:
        executeNothing();
        break;
    case AH_STATE_GO_TO_PUCK:
        executeGoToPuck();
        break;
    case AH_STATE_BE_ALERT:
        executeBeAlert();
        break;
    case AH_STATE_GO_TO_BACK:
        executeGoToBack();
        break;
    case AH_STATE_AVOID_GOAL:
        executeAvoidGoal();
        break;
    case AH_STATE_CLEAR_PUCK:
        executeClearPuck();
        break;
    case AH_STATE_SHOOT_TO_CLEAR:
        executeShootToClear();
        break;
    case AH_STATE_PREPARE_SHOOT:
        executePrepareShoot();
        break;
    case AH_STATE_SHOOT_TO_GOAL:
        executeShootToGoal();
        break;
    default:
        break;
    }

    // go to the target position
    return myPointToTable(go());
}

int AHAI::getState()
{
    return iState;
}

void AHAI::selectState(int State)
{
    iState = State;
    iStateStep = AH_STATE_STEP_ENTER;
    iStateTime = 0;
}

void AHAI::chooseNewState()
{
    // PUCK IN MY ZONE
    if(pPuckPosition.y <= (AH_TABLE_HEIGHT / 2))
    {
        // high priority: if the puck is going to enter into my goal, I clear it
        if(pPuckPosition.y < fNearBackBorder && abs(pPuckPosition.x) < (AH_TABLE_WIDTH / 8) && vPuckVelocity.y < 0.0)
        {
            selectState(AH_STATE_CLEAR_PUCK);
        }

        // puck is stopped or almost
        else if(abs(vPuckVelocity.x) < fStopped && abs(vPuckVelocity.y) < fStopped)
        {
            // there is enough space to shoot
            if(pPuckPosition.y > fNearBackBorder && (pPuckPosition.y - pMyPosition.y) > AH_MALLET_RADIUS)
            {
                selectState(AH_STATE_PREPARE_SHOOT);
            }

            // there's not enough space to shoot
            else
            {
                if(pMyPosition.y > fNearBackBorder)
                    selectState(AH_STATE_GO_TO_BACK);
            }
        }

        // puck is going towards the opponent
        else if(vPuckVelocity.y >= 0.0)
        {
            // low speed
            if(vPuckVelocity.y < fLowSpeed && abs(pPuckPosition.x) < (AH_TABLE_WIDTH / 2 - fNearSideBorder))
            {
                selectState(AH_STATE_PREPARE_SHOOT);
            }

            // normal or high speed
            else
            {
                selectState(AH_STATE_BE_ALERT);
            }
        }

        // puck is coming towards my goal
        else if(vPuckVelocity.y < 0.0)
        {
            // high speed
            if(abs(vPuckVelocity.y) > cLevel.AttackThresholdSpeed)
            {
                selectState(AH_STATE_AVOID_GOAL);
            }

            // normal or low speed
            else if(abs(pPuckPosition.x) < (AH_TABLE_WIDTH / 2 - fNearSideBorder) && 
                    (pPuckPosition.y - pMyPosition.y) > fHoleToShoot)
            {
                selectState(AH_STATE_PREPARE_SHOOT);
            }
        }
    }

    // PUCK OUT OF MY ZONE
    else
    {
        // puck is stopped or going towards the opponent
        if(vPuckVelocity.y >= 0.0)
            selectState(AH_STATE_BE_ALERT);

        // puck is coming to me
        else 
        {
            // high speed
            if(abs(vPuckVelocity.y) > fHighSpeed)
                selectState(AH_STATE_AVOID_GOAL);

            // low speed
            else
                selectState(AH_STATE_BE_ALERT);
        }
    }
}

void AHAI::executeNothing()
{
    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        fSpeed = cLevel.DefaultSpeed / 2;
        pTargetPosition.x = 0.0;
        pTargetPosition.y = fNearBackBorder / 2;
        chooseNewState();

        break;

    default:
        break;
    }
}

void AHAI::executeGoToPuck()
{
    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        pTargetPosition.x = pPuckPosition.x;
        pTargetPosition.y = pPuckPosition.y;

        fSpeed = cLevel.DefaultSpeed * 2.0f;

        if(cGame->checkEvent() == AH_EVENT_PUCK_MALLET1_COLLISION ||
           cGame->checkEvent() == AH_EVENT_PUCK_MALLET2_COLLISION)
        {
            iStateStep = AH_STATE_STEP_EXIT;
        }

        break;

    case AH_STATE_STEP_EXIT:

        selectState(AH_STATE_NOTHING);

        break;

    default:
        break;
    }
}

void AHAI::executeBeAlert()
{
    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        iStateStep = AH_STATE_STEP_EXECUTE;

        // average point between puck and goal in my zone
        pTargetPosition.x = pPuckPosition.x / 4.0f;
        pTargetPosition.y = fMinimumGoalDistance + rand() % round(fAlertZoneHeight);
        fSpeed = cLevel.DefaultSpeed;

        break;

    case AH_STATE_STEP_EXECUTE:

        if((targetPositionReached() && (vPuckVelocity.x != 0.0 || vPuckVelocity.y != 0.0)) ||
            vPuckVelocity.y < 0.0)
        {
            iStateStep = AH_STATE_STEP_EXIT;
        }

        break;

    case AH_STATE_STEP_EXIT:

        selectState(AH_STATE_NOTHING);

        break;

    default:
        break;
    }
}

void AHAI::executeGoToBack()
{
    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        pTargetPosition.x = 0.0;
        pTargetPosition.y = fNearBackBorder;

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        fSpeed = cLevel.SpeedForDefending;

        if(targetPositionReached() ||
           (pPuckPosition.y < pMyPosition.y && abs(pPuckPosition.x) < (AH_TABLE_WIDTH / 6)))
        {
            iStateStep = AH_STATE_STEP_EXIT;
        }

        break;

    case AH_STATE_STEP_EXIT:

        selectState(AH_STATE_NOTHING);

        break;

    default:
        break;
    }
}

void AHAI::executeAvoidGoal()
{
    float fTimeForThePuckToCome;
    float fDistanceToTarget;
    float fSpeedSlow = 5.0f;

    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:
        
        // target position
        pTargetPosition.y = max(pMyPosition.y - fBackSpace, fMinimumGoalDistance);
        pTargetPosition.x = predictPositionX(pTargetPosition.y);
        
        // error
        pTargetPosition.x += rand() % 50;
        pTargetPosition.y += rand() % 50;

        // speed
        fTimeForThePuckToCome = (pTargetPosition.y - pPuckPosition.y) / vPuckVelocity.y;
        fTimeForThePuckToCome = abs(fTimeForThePuckToCome);
        fTimeForThePuckToCome -= 2.0;        // state change frames

        if(fTimeForThePuckToCome < 0.0)
        {
            fCalculatedSpeed = cLevel.SpeedForDefending;
        }
        else
        {
            fDistanceToTarget = (pTargetPosition.x - pMyPosition.x) * (pTargetPosition.x - pMyPosition.x);
            fDistanceToTarget += (pTargetPosition.y - pMyPosition.y) * (pTargetPosition.y - pMyPosition.y);
            fDistanceToTarget = sqrt(fDistanceToTarget);

            fCalculatedSpeed = fDistanceToTarget / fTimeForThePuckToCome;
            fCalculatedSpeed = min(fCalculatedSpeed, cLevel.SpeedForDefending);
        }

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        fSpeed = fCalculatedSpeed;

        if(pPuckPosition.y <= pMyPosition.y || vPuckVelocity.y >= -fSpeedSlow)
            iStateStep = AH_STATE_STEP_EXIT;

        break;

    case AH_STATE_STEP_EXIT:

        selectState(AH_STATE_NOTHING);

        break;

    default:
        break;
    }
}

void AHAI::executeClearPuck()
{
    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        pTargetPosition.x = (pMyPosition.x < 0)? -AH_TABLE_WIDTH / 6.0f: AH_TABLE_WIDTH / 6.0f;
        pTargetPosition.y = AH_MALLET_RADIUS;

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        if(targetPositionReached() || 
           pPuckPosition.y > fNearBackBorder ||
           abs(pPuckPosition.x) > (AH_TABLE_WIDTH / 6))
        {
            iStateStep = AH_STATE_STEP_EXIT;
        }

        fSpeed = cLevel.SpeedForClearing;

        break;

    case AH_STATE_STEP_EXIT:

        selectState(AH_STATE_SHOOT_TO_CLEAR);

        break;

    default:
        break;
    }
}

void AHAI::executeShootToClear()
{
    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        pTargetPosition.y = pMyPosition.y;
        pTargetPosition.x = (pMyPosition.x > 0)? -AH_TABLE_WIDTH / 3.0f: AH_TABLE_WIDTH / 3.0f;

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        fSpeed = cLevel.SpeedForClearing;
        
        if(targetPositionReached())           
        {
            iStateStep = AH_STATE_STEP_EXIT;
        }
        
        break;

    case AH_STATE_STEP_EXIT:

        selectState(AH_STATE_NOTHING);

        break;

    default:
        break;
    }
}

void AHAI::executePrepareShoot()
{
    AHPoint pPuckFuturePosition;
    CLine* cShotLine;

    float fTimeForThePuckToCome;
    float fDistanceToTarget;

    int iChoice;

    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        // default: direct shot
        pShotPoint = calculateGoalPoint();
        iChoice = rand() % 2;

        // with bounce
        if(iChoice == 1)
        {
            // against the left wall
            if(pMyPosition.x >= 0.0 && pPuckPosition.x < -fHoleToShoot)
                pShotPoint.x -= AH_TABLE_WIDTH * (1 + rand() % 2);    // one or two bounces

            // against the right wall
            else if(pMyPosition.x < 0.0 && pPuckPosition.x > fHoleToShoot)
                pShotPoint.x += AH_TABLE_WIDTH * (1 + rand() % 2);
        }

        // puck's future position
        fImpulseDistance = fBackSpace;
        pPuckFuturePosition.y = pMyPosition.y + fImpulseDistance;
        pPuckFuturePosition.x = predictPositionX(pPuckFuturePosition.y);

        // shot line
        cShotLine = new CLine(pShotPoint.x, pShotPoint.y, pPuckFuturePosition.x, pPuckFuturePosition.y);
        pTargetPosition = pMyPosition;

        if(cShotLine->can_calculate_x())
            pTargetPosition.x = (float)cShotLine->x(pTargetPosition.y);

        // speed
        if(abs(vPuckVelocity.y) > fStopped)
        {
            fTimeForThePuckToCome = (pPuckFuturePosition.y - pPuckPosition.y) / vPuckVelocity.y;
            fTimeForThePuckToCome = abs(fTimeForThePuckToCome);
            fTimeForThePuckToCome -= 2.0;        // state change frames

            if(fTimeForThePuckToCome < 0.0)
            {
                fCalculatedSpeed = cLevel.SpeedForShooting;
            }
            else
            {
                fDistanceToTarget = (pTargetPosition.x - pMyPosition.x) * (pTargetPosition.x - pMyPosition.x);
                fDistanceToTarget += (pTargetPosition.y - pMyPosition.y) * (pTargetPosition.y - pMyPosition.y);
                fDistanceToTarget = sqrt(fDistanceToTarget);

                fCalculatedSpeed = fDistanceToTarget / fTimeForThePuckToCome;
                fCalculatedSpeed = min(cLevel.SpeedForShooting, fCalculatedSpeed);
            }
        }
        else
        {
            fCalculatedSpeed = cLevel.DefaultSpeed;
        }

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        fSpeed = fCalculatedSpeed;

        if(targetPositionReached() ||                        // ready for the shot
           pPuckPosition.y <= pMyPosition.y ||                 // the puck has escaped
           pPuckPosition.y > (AH_TABLE_HEIGHT / 2) ||        // the puck is out of my zone
           abs(vPuckVelocity.y) < fStopped)                    // the puck is stopped
        {
            iStateStep = AH_STATE_STEP_EXIT;
        }

        break;

    case AH_STATE_STEP_EXIT:

        if(pPuckPosition.y > pMyPosition.y)
            selectState(AH_STATE_SHOOT_TO_GOAL);
        else
            selectState(AH_STATE_NOTHING);

        break;

    default:
        break;
    }
}

void AHAI::executeShootToGoal()
{
    switch(iStateStep)
    {
    case AH_STATE_STEP_ENTER:

        iStateStep = AH_STATE_STEP_EXECUTE;

        break;

    case AH_STATE_STEP_EXECUTE:

        pTargetPosition = pPuckPosition;
        fSpeed = cLevel.SpeedForShooting;

        if(pPuckPosition.y < pMyPosition.y ||                                            // the puck is behind me
           (cGame->checkEvent() == AH_EVENT_PUCK_MALLET2_COLLISION && iPlayer == 2) ||    // the puck collides with the mallet
           (cGame->checkEvent() == AH_EVENT_PUCK_MALLET1_COLLISION && iPlayer == 1) ||
           pPuckPosition.y > (AH_TABLE_HEIGHT / 2) ||                                    // the puck is out of my zone
           abs(pPuckPosition.x) > (AH_TABLE_WIDTH / 2 + fNearSideBorder) ||                // the puck is too near from a side border
           pPuckPosition.y < fNearBackBorder)                                            // the puck is too near from the back border
        {
            iStateStep = AH_STATE_STEP_EXIT;
        }
        
        break;

    case AH_STATE_STEP_EXIT:

        selectState(AH_STATE_NOTHING);

        break;

    default:
        break;
    }
}

bool AHAI::targetPositionReached()
{
    if((abs(pTargetPosition.x - pMyPosition.x) + abs(pTargetPosition.y - pMyPosition.y)) <= fSpeed)
    {
        return true;
    }
    else
    {
        return false;
    }
}

AHPoint AHAI::myPointToTable(AHPoint pPoint)
{
    AHPoint pTablePosition;

    pTablePosition.x = pPoint.x;

    // my positions --> table positions
    if(iPlayer == 1)
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

AHPoint AHAI::calculateGoalPoint()
{
    AHPoint pGoal;

    pGoal.x = (float)(rand() % (AH_TABLE_WIDTH / 3) - AH_TABLE_WIDTH / 6);
    pGoal.y = AH_TABLE_HEIGHT;

    return pGoal;
}

float AHAI::predictPositionX(float Y)
{
    double dX;
    CLine cLine(pPuckPosition.x, pPuckPosition.y, pPuckPosition.x + vPuckVelocity.x, pPuckPosition.y + vPuckVelocity.y);

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

AHPoint AHAI::go()
{
    if(targetPositionReached())
        return pMyPosition;

    AHPoint pNewPosition;
    AHVector vTargetDirection;

    vTargetDirection.x = pTargetPosition.x - pMyPosition.x;
    vTargetDirection.y = pTargetPosition.y - pMyPosition.y;

    setVectorLength(&vTargetDirection, fSpeed);

    pNewPosition = pMyPosition;
    pNewPosition.x += vTargetDirection.x;
    pNewPosition.y += vTargetDirection.y;

    return pNewPosition;
}
