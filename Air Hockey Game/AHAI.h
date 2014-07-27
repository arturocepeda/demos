
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AHAI.h ---
*/

#pragma once

#include "AH.h"
#include "Core/GEUtils.h"

struct AHAILevel
{
    float DefaultSpeed;
    float SpeedForShooting;
    float SpeedForDefending;
    float SpeedForClearing;
    float AttackThresholdSpeed;
};

struct AHAIStates
{
    enum
    {
        Nothing,
        GoToPuck,
        BeAlert,
        GoToBack,
        AvoidGoal,
        PrepareShot,
        ShootToGoal,
        ClearPuck,
        ShootToClear,
        Count
    };
};

struct AHAIStateSteps
{
    enum
    {
        Enter,
        Execute,
        Exit
    };
};

struct AHAIData
{
    AHGame* cGame;
    AHAILevel cLevel;
    int iPlayer;

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

    float predictPositionX(float Y);
    bool targetPositionReached();
};

class AHAIState
{
protected:
    AHAIData* sData;
    int iTime;
    int iNextState;

public:
    AHAIState(AHAIData* Data) : sData(Data) { iTime = 0; iNextState = AHAIStates::Nothing; }
    virtual ~AHAIState() {}

    void select();
    void incrementTime();
    int getCurrentTime();
    int getNextState();

    virtual void enter() = 0;
    virtual void run() = 0;
};

class AHAI
{
private:
    AHAIData sData;
    AHAIState** cStates;
    int iState;
    int iStateStep;

    void initializeStates();
    void releaseStates();
    void selectState(int State);
    void chooseNewState();

    AHPoint go();
    AHPoint myPointToTable(AHPoint pPoint);

public:
    AHAI(AHGame* Game, const AHAILevel& Level, bool Player1);
    ~AHAI();

    AHPoint update();
    int getState();
};

class AHAIStateNothing : public AHAIState
{
public:
    AHAIStateNothing(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStateGoToPuck : public AHAIState
{
public:
    AHAIStateGoToPuck(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStateBeAlert : public AHAIState
{
public:
    AHAIStateBeAlert(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStateGoToBack : public AHAIState
{
public:
    AHAIStateGoToBack(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStateAvoidGoal : public AHAIState
{
private:
    float fTimeForThePuckToCome;
    float fDistanceToTarget;

    static float fSpeedSlow;

public:
    AHAIStateAvoidGoal(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStateClearPuck : public AHAIState
{
public:
    AHAIStateClearPuck(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStateShootToClear : public AHAIState
{
public:
    AHAIStateShootToClear(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStatePrepareShot : public AHAIState
{
private:
    AHPoint pPuckFuturePosition;
    GE::Core::Line* cShotLine;

    float fTimeForThePuckToCome;
    float fDistanceToTarget;

    int iChoice;

    AHPoint calculateGoalPoint();

public:
    AHAIStatePrepareShot(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};

class AHAIStateShootToGoal : public AHAIState
{
public:
    AHAIStateShootToGoal(AHAIData* Data) : AHAIState(Data) {}

    void enter();
    void run();
};
