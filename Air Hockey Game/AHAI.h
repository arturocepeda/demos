
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AHAI.h ---
*/

#ifndef _AHAI_H_
#define _AHAI_H_

#include "AH.h"

#define AH_STATE_NOTHING            0
#define AH_STATE_GO_TO_PUCK            1
#define AH_STATE_BE_ALERT            2
#define AH_STATE_GO_TO_BACK            3
#define AH_STATE_AVOID_GOAL            4
#define AH_STATE_PREPARE_SHOOT        5
#define AH_STATE_SHOOT_TO_GOAL        6
#define AH_STATE_CLEAR_PUCK            7
#define AH_STATE_SHOOT_TO_CLEAR        8

#define AH_STATE_STEP_ENTER            0
#define AH_STATE_STEP_EXECUTE        1
#define AH_STATE_STEP_EXIT            2

struct AHAILevel
{
    float DefaultSpeed;
    float SpeedForShooting;
    float SpeedForDefending;
    float SpeedForClearing;
    float AttackThresholdSpeed;
};

class AHAIState
{
private:
    int iStep;
    int iTime;

public:
    virtual void enter() = 0;
    virtual void run() = 0;
    virtual void exit() = 0;
};

class AHAI
{
private:
    AHGame* cGame;
    AHAILevel cLevel;
    int iPlayer;
    int iState;
    int iStateStep;
    int iStateTime;

    float fSpeed;
    float fCalculatedSpeed;
    float fImpulseDistance;

    AHPoint pTargetPosition;
    AHPoint pMyPosition;
    AHPoint pOpponentPosition;
    AHPoint pPuckPosition;
    AHPoint pShotPoint;
    AHVector vPuckVelocity;

    float fMinimumGoalDistance;
    float fAlertZoneHeight;
    float fBackSpace;
    float fNearSideBorder;
    float fNearBackBorder;
    float fHoleToShoot;

    float fStopped;
    float fHighSpeed;
    float fLowSpeed;

    void selectState(int State);
    void chooseNewState();

    void executeNothing();
    void executeGoToPuck();
    void executeBeAlert();
    void executeGoToBack();
    void executeAvoidGoal();
    void executeClearPuck();
    void executeShootToClear();
    void executePrepareShoot();
    void executeShootToGoal();

    bool targetPositionReached();
    float predictPositionX(float Y);

    AHPoint go();
    AHPoint calculateGoalPoint();
    AHPoint myPointToTable(AHPoint pPoint);

public:
    AHAI(AHGame* Game, const AHAILevel& Level, bool Player1);
    ~AHAI();

    AHPoint update();
    int getState();
};

#endif
