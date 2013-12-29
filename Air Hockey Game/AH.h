
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AH.h ---
*/

#pragma once

#include "AHEntity.h"

#define AH_TABLE_WIDTH      1990
#define AH_TABLE_HEIGHT     3400

#define AH_MALLET_RADIUS    110
#define AH_PUCK_RADIUS      55

#define AH_STATUS_PAUSED    0
#define AH_STATUS_PLAYING   1
#define AH_STATUS_P1_WINS   2
#define AH_STATUS_P2_WINS   3

#define AH_EVENT_NONE                           0
#define AH_EVENT_GOAL_P1                        1
#define AH_EVENT_GOAL_P2                        2
#define AH_EVENT_PUCK_MALLET1_COLLISION         3
#define AH_EVENT_PUCK_MALLET2_COLLISION         4
#define AH_EVENT_PUCK_TABLE_TOP_COLLISION       5 
#define AH_EVENT_PUCK_TABLE_BOTTOM_COLLISION    6
#define AH_EVENT_PUCK_TABLE_LEFT_COLLISION      7
#define AH_EVENT_PUCK_TABLE_RIGHT_COLLISION     8

#define AH_FRICTION_AIR         0.0045f
#define AH_FRICTION_NO_AIR      0.0300f
#define AH_PUCK_MAXSPEED        125.0000f

float getVectorLength(AHVector* vVector);
void setVectorLength(AHVector* vVector, float fLength);
void normalizeVector(AHVector* vVector);
void multiplyVector(AHVector* vVector, float fValue);

class AHGame
{
private:
    // game status
    int iStatus;
    int iEvent;
    int iGoalsPlayer1;
    int iGoalsPlayer2;
    int iLastPlayerWhoHitThePuck;

    // movable entities
    CircularEntity* ePuck;
    CircularEntity* ePlayer1;
    CircularEntity* ePlayer2;

    // table size
    float fTableWidth;
    float fTableHeight;
    float fGoalMinimumX;
    float fGoalMaximumX;

    // render values
    AHPoint pRenderCenter;
    float fRenderTableWidth;
    float fRenderTableHeight;
    float fRenderRatioX;
    float fRenderRatioY;

    // ratio between mouse coordinates and table positions
    AHPoint pMouseDelta;
    AHVector vMouseRatio;

    void create(float fCenterX, float fCenterY, float fTableWidth, float fTableHeight);
    void initialize();

    void setPositionPuck(const AHPoint& Position);
    void setPositionPlayer1(const AHPoint& Position);
    void setPositionPlayer2(const AHPoint& Position);

    void placeEntities(bool bTurnPlayer1);
    void checkGoal();
    void checkCollision();
    void makeCollision(CircularEntity* cePuck, CircularEntity* cePlayer, AHPoint pPlayerTablePosition);

public:
    AHGame();
    AHGame(float CenterX, float CenterY, float TableWidth, float TableHeight);
    ~AHGame();

    void start();
    void update();
    void reset();
    void pause();

    int checkEvent();
    void setTableFriction(float Friction);

    int getGoalsPlayer1();
    int getGoalsPlayer2();

    AHPoint getRenderPositionPuck();
    AHPoint getRenderPositionPlayer1();
    AHPoint getRenderPositionPlayer2();

    AHPoint getTablePositionPuck();
    AHPoint getTablePositionPlayer1();
    AHPoint getTablePositionPlayer2();

    AHVector getVelocityPuck();
    AHVector getVelocityPlayer1();
    AHVector getVelocityPlayer2();

    float getSquareSpeedPuck();
    float getSquareSpeedPlayer1();
    float getSquareSpeedPlayer2();

    float getSpeedPuck();
    float getSpeedPlayer1();
    float getSpeedPlayer2();

    int getLastPlayerWhoHitThePuck();

    void setGoalsPlayer1(int Goals);
    void setGoalsPlayer2(int Goals);

    void setTablePositionPuck(const AHPoint& Position);
    void setTablePositionPlayer1(const AHPoint& Position);
    void setTablePositionPlayer2(const AHPoint& Position);

    void setMouseRatio(float MouseMinimumX, float MouseMaximumX, float MouseMinimumY, float MouseMaximumY);
    void setMousePositionPlayer1(float MouseX, float MouseY);
    void setMousePositionPlayer2(float MouseX, float MouseY);
};
