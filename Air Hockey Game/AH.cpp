
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AH.cpp ---
*/

#include "AH.h"
#include "utils.h"
#include <cmath>
#include <stdlib.h>


//
//  AHVector functions
//
float getVectorLength(AHVector* vVector)
{
    return sqrt(vVector->x * vVector->x + vVector->y * vVector->y);
}

void setVectorLength(AHVector* vVector, float fLength)
{
    normalizeVector(vVector);
    multiplyVector(vVector, fLength);
}

void normalizeVector(AHVector* vVector)
{
    float fLength = getVectorLength(vVector);

    if(fLength > 0.0)
    {
        vVector->x /= fLength;
        vVector->y /= fLength;
    }
}

void multiplyVector(AHVector* vVector, float fValue)
{
    vVector->x *= fValue;
    vVector->y *= fValue;
}


//
//  AHGame
//
AHGame::AHGame()
{
    create(0, 0, AH_TABLE_WIDTH, AH_TABLE_HEIGHT);
    initialize();
}

AHGame::AHGame(float CenterX, float CenterY, float TableWidth, float TableHeight)
{
    create(CenterX, CenterY, TableWidth, TableHeight);
    initialize();
}

AHGame::~AHGame()
{
    delete ePuck;
    delete ePlayer1;
    delete ePlayer2;
}

void AHGame::create(float CenterX, float CenterY, float TableWidth, float TableHeight)
{
    // game entities
    ePuck = new CircularEntity(1.0, AH_PUCK_RADIUS);
    ePlayer1 = new CircularEntity(1.0, AH_MALLET_RADIUS);
    ePlayer2 = new CircularEntity(1.0, AH_MALLET_RADIUS);

    // table values
    fTableWidth = fRenderTableWidth = AH_TABLE_WIDTH;
    fTableHeight = fRenderTableHeight = AH_TABLE_HEIGHT;
    fGoalMinimumX = ePuck->getRadius() - (AH_TABLE_WIDTH / 6);
    fGoalMaximumX = (AH_TABLE_WIDTH / 6) - AH_PUCK_RADIUS;

    // render values
    pRenderCenter.x = CenterX;
    pRenderCenter.y = CenterY;

    if(TableWidth > 0.0f)
        fRenderTableWidth = TableWidth;

    if(TableHeight > 0.0f)
        fRenderTableHeight = TableHeight;

    fRenderRatioX = fRenderTableWidth / fTableWidth;
    fRenderRatioY = fRenderTableHeight / fTableHeight;
}

void AHGame::initialize()
{
    iStatus = AH_STATUS_PAUSED;
    iEvent = AH_EVENT_NONE;
    iLastPlayerWhoHitThePuck = 0;

    ePuck->setBounds(-AH_TABLE_HEIGHT, AH_TABLE_HEIGHT, -(AH_TABLE_WIDTH / 2), AH_TABLE_WIDTH / 2);
    ePlayer1->setBounds(AH_TABLE_HEIGHT / 2, 0, -(AH_TABLE_WIDTH / 2), AH_TABLE_WIDTH / 2);
    ePlayer2->setBounds(AH_TABLE_HEIGHT / 2, 0, -(AH_TABLE_WIDTH / 2), AH_TABLE_WIDTH / 2);

    reset();
}

void AHGame::start()
{
    iStatus = AH_STATUS_PLAYING;
}

void AHGame::reset()
{
    iGoalsPlayer1 = 0;
    iGoalsPlayer2 = 0;

    placeEntities(rand() % 2 == 1);
}

void AHGame::pause()
{
    iStatus = AH_STATUS_PAUSED;
}

void AHGame::placeEntities(bool bTurnPlayer1)
{
    AHPoint pPosition;
    AHVector vStopped;

    // players
    if(iEvent != AH_EVENT_GOAL_P1 && iEvent != AH_EVENT_GOAL_P2)
    {
        pPosition.x = 0.0f;
        pPosition.y = AH_TABLE_HEIGHT / 4;
        ePlayer1->setPosition(pPosition);
        ePlayer2->setPosition(pPosition);
    }

    // puck
    if(bTurnPlayer1)
    {
        do
        {
            pPosition.x = (float)random(-AH_TABLE_WIDTH / 4, AH_TABLE_WIDTH / 4);
            pPosition.y = (float)random(AH_TABLE_HEIGHT / 8, AH_TABLE_HEIGHT / 2 - AH_TABLE_HEIGHT / 8);

        } while(abs(pPosition.x - getTablePositionPlayer1().x) <= AH_PUCK_MAXSPEED || 
                abs(pPosition.y - getTablePositionPlayer1().y) <= AH_PUCK_MAXSPEED);
    }
    else
    {
        do
        {
            pPosition.x = (float)random(-AH_TABLE_WIDTH / 4, AH_TABLE_WIDTH / 4);
            pPosition.y = -(float)random(AH_TABLE_HEIGHT / 8, AH_TABLE_HEIGHT / 2 - AH_TABLE_HEIGHT / 8);

        } while(abs(pPosition.x - getTablePositionPlayer2().x) <= AH_PUCK_MAXSPEED || 
                abs(pPosition.y - getTablePositionPlayer2().y) <= AH_PUCK_MAXSPEED);
    }

    ePuck->setPosition(pPosition);

    vStopped.x = 0.0;
    vStopped.y = 0.0;

    ePuck->setVelocity(vStopped);
    ePlayer1->setVelocity(vStopped);
    ePlayer2->setVelocity(vStopped);
}

void AHGame::update()
{
    // status
    if(iStatus != AH_STATUS_PLAYING)
        return;

    if(iEvent == AH_EVENT_GOAL_P1)
    {
        placeEntities(false);
        iEvent = AH_EVENT_NONE;
        return;
    }

    if(iEvent == AH_EVENT_GOAL_P2)
    {
        placeEntities(true);
        iEvent = AH_EVENT_NONE;
        return;
    }

    // puck behaviour
    ePuck->update();

    // events management
    iEvent = AH_EVENT_NONE;

    // check for collisions
    checkCollision();

    // check for goal
    checkGoal();
}

int AHGame::getGoalsPlayer1()
{
    return iGoalsPlayer1;
}

int AHGame::getGoalsPlayer2()
{
    return iGoalsPlayer2;
}

int AHGame::checkEvent()
{
    return iEvent;
}

void AHGame::checkGoal()
{
    AHPoint pPositionA = ePuck->getPosition();

    if(pPositionA.y <= (-AH_TABLE_HEIGHT / 2 - ePuck->getRadius() * 2))
    {
        if(pPositionA.x >= fGoalMinimumX && pPositionA.x <= fGoalMaximumX)
        {
            iEvent = AH_EVENT_GOAL_P1;
            iGoalsPlayer1++;
            return;
        }
    }

    if(pPositionA.y >= (AH_TABLE_HEIGHT / 2 + ePuck->getRadius() * 2))
    {
        if(pPositionA.x >= fGoalMinimumX && pPositionA.x <= fGoalMaximumX)
        {
            iEvent = AH_EVENT_GOAL_P2;
            iGoalsPlayer2++;
            return;
        }
    }
}

void AHGame::checkCollision()
{
    AHPoint pPositionA;
    AHVector vVelocity;

    // puck - table
    pPositionA = ePuck->getPosition();

    if(pPositionA.x <= (-AH_TABLE_WIDTH / 2 + ePuck->getRadius()) && ePuck->getVelocity().x < 0.0)
    {
        pPositionA.x = -AH_TABLE_WIDTH / 2 + ePuck->getRadius();
        ePuck->setPosition(pPositionA);

        vVelocity = ePuck->getVelocity();
        vVelocity.x = -vVelocity.x;
        ePuck->setVelocity(vVelocity);

        iEvent = AH_EVENT_PUCK_TABLE_LEFT_COLLISION;
        return;
    }

    if(pPositionA.x >= (AH_TABLE_WIDTH / 2 - ePuck->getRadius()) && ePuck->getVelocity().x > 0.0)
    {
        pPositionA.x = AH_TABLE_WIDTH / 2 - ePuck->getRadius();
        ePuck->setPosition(pPositionA);

        vVelocity = ePuck->getVelocity();
        vVelocity.x = -vVelocity.x;
        ePuck->setVelocity(vVelocity);

        iEvent = AH_EVENT_PUCK_TABLE_RIGHT_COLLISION;
        return;
    }

    if(pPositionA.y <= (-AH_TABLE_HEIGHT / 2 + ePuck->getRadius()) && ePuck->getVelocity().y < 0.0)
    {
        // not goal
        if(pPositionA.x >= fGoalMinimumX && pPositionA.x <= fGoalMaximumX)
            return;

        pPositionA.y = -AH_TABLE_HEIGHT / 2 + ePuck->getRadius();
        ePuck->setPosition(pPositionA);

        vVelocity = ePuck->getVelocity();
        vVelocity.y = -vVelocity.y;
        ePuck->setVelocity(vVelocity);

        iEvent = AH_EVENT_PUCK_TABLE_TOP_COLLISION;
        return;
    }

    if(pPositionA.y >= (AH_TABLE_HEIGHT / 2 - ePuck->getRadius()) && ePuck->getVelocity().y > 0.0)
    {
        // not goal
        if(pPositionA.x >= fGoalMinimumX && pPositionA.x <= fGoalMaximumX)
            return;

        pPositionA.y = AH_TABLE_HEIGHT / 2 - ePuck->getRadius();
        ePuck->setPosition(pPositionA);

        vVelocity = ePuck->getVelocity();
        vVelocity.y = -vVelocity.y;
        ePuck->setVelocity(vVelocity);

        iEvent = AH_EVENT_PUCK_TABLE_BOTTOM_COLLISION;
        return;
    }

    // puck - player1
    float fMinimumDistanceSquared;
    float fDeltaXSquared;
    float fDeltaYSquared;
    float fDistanceSquared;

    fMinimumDistanceSquared = ePuck->getRadius() + ePlayer1->getRadius();
    fMinimumDistanceSquared *= fMinimumDistanceSquared;

    fDistanceSquared = 0.0;
    fDeltaXSquared = ePuck->getPosition().x - getTablePositionPlayer1().x;
    fDeltaXSquared *= fDeltaXSquared;
    fDistanceSquared += fDeltaXSquared;
    fDeltaYSquared = ePuck->getPosition().y - getTablePositionPlayer1().y;
    fDeltaYSquared *= fDeltaYSquared;
    fDistanceSquared += fDeltaYSquared;

    if(fDistanceSquared <= fMinimumDistanceSquared)
    {
        iLastPlayerWhoHitThePuck = 1;
        iEvent = AH_EVENT_PUCK_MALLET1_COLLISION;
        makeCollision(ePuck, ePlayer1, getTablePositionPlayer1());
        return;
    }

    // puck - player2
    fMinimumDistanceSquared = ePuck->getRadius() + ePlayer2->getRadius();
    fMinimumDistanceSquared *= fMinimumDistanceSquared;

    fDistanceSquared = 0.0;
    fDeltaXSquared = ePuck->getPosition().x - getTablePositionPlayer2().x;
    fDeltaXSquared *= fDeltaXSquared;
    fDistanceSquared += fDeltaXSquared;
    fDeltaYSquared = ePuck->getPosition().y - getTablePositionPlayer2().y;
    fDeltaYSquared *= fDeltaYSquared;
    fDistanceSquared += fDeltaYSquared;

    if(fDistanceSquared <= fMinimumDistanceSquared)
    {
        iLastPlayerWhoHitThePuck = 2;
        iEvent = AH_EVENT_PUCK_MALLET2_COLLISION;        
        makeCollision(ePuck, ePlayer2, getTablePositionPlayer2());
        return;
    }
}

void AHGame::makeCollision(CircularEntity* cePuck, CircularEntity* cePlayer, AHPoint pPlayerTablePosition)
{
    float fPuckSpeed;
    float fPlayerSpeed;

    AHVector vPlayerVelocity = cePlayer->getVelocity();
    AHVector vPuckVelocity = cePuck->getVelocity();
    AHPoint pPuckPosition = cePuck->getPosition();

    AHVector vSeparation;
    vSeparation.x = pPuckPosition.x - pPlayerTablePosition.x;
    vSeparation.y = pPuckPosition.y - pPlayerTablePosition.y;

    fPuckSpeed = getVectorLength(&vPuckVelocity);
    fPlayerSpeed = getVectorLength(&vPlayerVelocity);

    // check the direction of the separation vector: if the speed of the puck is too high,
    // it can occurs that the puck has gone through the mallet
    if(fPlayerSpeed > AH_PUCK_MAXSPEED)
    {
        if((vPlayerVelocity.x > 0 && pPuckPosition.x < pPlayerTablePosition.x) ||
            (vPlayerVelocity.x < 0 && pPuckPosition.x > pPlayerTablePosition.x))
        {
            vSeparation.x -= vPuckVelocity.x / 2;
            vSeparation.x += vPlayerVelocity.x / 2;
        }

        if((vPlayerVelocity.y > 0 && pPuckPosition.y < pPlayerTablePosition.y) ||
            (vPlayerVelocity.y < 0 && pPuckPosition.y > pPlayerTablePosition.y))
        {
            vSeparation.y -= vPuckVelocity.y / 2;
            vSeparation.y += vPlayerVelocity.y / 2;
        }
    }

    // make sure that the objects stops colliding
    AHPoint pSeparatedPosition;
    AHVector vSeparatedPosition = vSeparation;
    
    setVectorLength(&vSeparatedPosition, cePuck->getRadius() + cePlayer->getRadius() + 1.0f);
    pSeparatedPosition.x = pPlayerTablePosition.x + vSeparatedPosition.x;
    pSeparatedPosition.y = pPlayerTablePosition.y + vSeparatedPosition.y;
    cePuck->setPosition(pSeparatedPosition);

    setVectorLength(&vSeparation, min(fPuckSpeed + fPlayerSpeed, AH_PUCK_MAXSPEED));
    cePuck->setVelocity(vSeparation);
}

AHPoint AHGame::getTablePositionPuck()
{
    return ePuck->getPosition();
}

AHPoint AHGame::getTablePositionPlayer1()
{
    AHPoint pPosition = ePlayer1->getPosition();

    pPosition.y = fTableHeight / 2 - pPosition.y;

    return pPosition;
}

AHPoint AHGame::getTablePositionPlayer2()
{
    AHPoint pPosition = ePlayer2->getPosition();

    pPosition.y -= fTableHeight / 2;

    return pPosition;
}

AHPoint AHGame::getRenderPositionPlayer1()
{
    AHPoint pPosition = getTablePositionPlayer1();

    pPosition.x *= fRenderRatioX;
    pPosition.y *= fRenderRatioY;

    pPosition.x += pRenderCenter.x;
    pPosition.y += pRenderCenter.y;

    return pPosition;
}

AHPoint AHGame::getRenderPositionPlayer2()
{
    AHPoint pPosition = getTablePositionPlayer2();

    pPosition.x *= fRenderRatioX;
    pPosition.y *= fRenderRatioY;

    pPosition.x += pRenderCenter.x;
    pPosition.y += pRenderCenter.y;

    return pPosition;
}


AHPoint AHGame::getRenderPositionPuck()
{
    AHPoint pPosition = ePuck->getPosition();

    pPosition.x *= fRenderRatioX;
    pPosition.y *= fRenderRatioY;

    pPosition.x += pRenderCenter.x;
    pPosition.y += pRenderCenter.y;

    return pPosition;
}

AHVector AHGame::getVelocityPuck()
{
    return ePuck->getVelocity();
}

AHVector AHGame::getVelocityPlayer1()
{
    return ePlayer1->getVelocity();
}

AHVector AHGame::getVelocityPlayer2()
{
    return ePlayer2->getVelocity();
}

float AHGame::getSquareSpeedPuck()
{
    return ePuck->getSquareSpeed();
}

float AHGame::getSquareSpeedPlayer1()
{
    return ePlayer1->getSquareSpeed();
}

float AHGame::getSquareSpeedPlayer2()
{
    return ePlayer2->getSquareSpeed();
}

float AHGame::getSpeedPuck()
{
    return ePuck->getSpeed();
}

float AHGame::getSpeedPlayer1()
{
    return ePlayer1->getSpeed();
}

float AHGame::getSpeedPlayer2()
{
    return ePlayer2->getSpeed();
}

int AHGame::getLastPlayerWhoHitThePuck()
{
    return iLastPlayerWhoHitThePuck;
}

void AHGame::setGoalsPlayer1(int Goals)
{
    iGoalsPlayer1 = Goals;
}

void AHGame::setGoalsPlayer2(int Goals)
{
    iGoalsPlayer2 = Goals;
}

void AHGame::setTableFriction(float Friction)
{
    ePuck->setFriction(Friction);
}

void AHGame::setPositionPuck(const AHPoint& Position)
{
    AHVector vVelocity;
    AHPoint pNewPosition = Position;

    if(pNewPosition.x <= (-AH_TABLE_WIDTH / 2 + ePuck->getRadius()))
        pNewPosition.x = -AH_TABLE_WIDTH / 2 + ePuck->getRadius();
    else if(pNewPosition.x >= (AH_TABLE_WIDTH / 2 - ePuck->getRadius()))
        pNewPosition.x = AH_TABLE_WIDTH / 2 - ePuck->getRadius();

    vVelocity.x = pNewPosition.x - ePuck->getPosition().x;
    vVelocity.y = ePuck->getPosition().y - pNewPosition.y;

    ePuck->setPosition(pNewPosition);
    ePuck->setVelocity(vVelocity);
}

void AHGame::setPositionPlayer1(const AHPoint& Position)
{
    AHVector vVelocity;
    AHPoint pNewPosition = Position;
    AHPoint pLastPosition;

    if(pNewPosition.x <= (-AH_TABLE_WIDTH / 2 + ePlayer1->getRadius()))
        pNewPosition.x = -AH_TABLE_WIDTH / 2 + ePlayer1->getRadius();
    else if(pNewPosition.x >= (AH_TABLE_WIDTH / 2 - ePlayer1->getRadius()))
        pNewPosition.x = AH_TABLE_WIDTH / 2 - ePlayer1->getRadius();

    if(pNewPosition.y <= (-AH_TABLE_HEIGHT / 2 + ePlayer1->getRadius()))
        pNewPosition.y = -AH_TABLE_HEIGHT / 2 + ePlayer1->getRadius();
    else if(pNewPosition.y >= (AH_TABLE_HEIGHT / 2 - ePlayer1->getRadius()))
        pNewPosition.y = AH_TABLE_HEIGHT / 2 - ePlayer1->getRadius();

    pLastPosition = ePlayer1->getPosition();
    ePlayer1->setPosition(pNewPosition);    // setPosition checks the bounds and sets the right position

    vVelocity.x = ePlayer1->getPosition().x - pLastPosition.x;
    vVelocity.y = pLastPosition.y - ePlayer1->getPosition().y;

    ePlayer1->setVelocity(vVelocity);
}

void AHGame::setPositionPlayer2(const AHPoint& Position)
{
    AHVector vVelocity;
    AHPoint pNewPosition = Position;
    AHPoint pLastPosition;

    if(pNewPosition.x <= (-AH_TABLE_WIDTH / 2 + ePlayer2->getRadius()))
        pNewPosition.x = -AH_TABLE_WIDTH / 2 + ePlayer2->getRadius();
    else if(pNewPosition.x >= (AH_TABLE_WIDTH / 2 - ePlayer2->getRadius()))
        pNewPosition.x = AH_TABLE_WIDTH / 2 - ePlayer2->getRadius();

    if(pNewPosition.y <= (-AH_TABLE_HEIGHT / 2 + ePlayer2->getRadius()))
        pNewPosition.y = -AH_TABLE_HEIGHT / 2 + ePlayer2->getRadius();
    else if(pNewPosition.y >= (AH_TABLE_HEIGHT / 2 - ePlayer2->getRadius()))
        pNewPosition.y = AH_TABLE_HEIGHT / 2 - ePlayer2->getRadius();

    pLastPosition = ePlayer2->getPosition();
    ePlayer2->setPosition(pNewPosition);    // setPosition checks the bounds and sets the right position

    vVelocity.x = ePlayer2->getPosition().x - pLastPosition.x;
    vVelocity.y = ePlayer2->getPosition().y - pLastPosition.y;

    ePlayer2->setVelocity(vVelocity);
}

void AHGame::setTablePositionPuck(const AHPoint& Position)
{
    setPositionPuck(Position);
}

void AHGame::setTablePositionPlayer1(const AHPoint& Position)
{
    AHPoint pNewPosition = Position;
    pNewPosition.y = fTableHeight / 2 - pNewPosition.y;
    setPositionPlayer1(pNewPosition);
}

void AHGame::setTablePositionPlayer2(const AHPoint& Position)
{
    AHPoint pNewPosition = Position;
    pNewPosition.y = fTableHeight / 2 + pNewPosition.y;
    setPositionPlayer2(pNewPosition);
}

void AHGame::setMouseRatio(float MouseMinimumX, float MouseMaximumX, float MouseMinimumY, float MouseMaximumY)
{
    // X axis
    float fPlayerMinimumX = ePlayer1->getBoundsLeft() + ePlayer1->getRadius();
    float fPlayerMaximumX = ePlayer1->getBoundsRight() - ePlayer1->getRadius();

    float fMouseDeltaX = MouseMaximumX - MouseMinimumX;
    float fPlayerDeltaX = fPlayerMaximumX - fPlayerMinimumX;

    vMouseRatio.x = fPlayerDeltaX / fMouseDeltaX;
    pMouseDelta.x = fPlayerMinimumX - (vMouseRatio.x * MouseMinimumX);

    // Y axis
    float fPlayerMinimumY = ePlayer1->getBoundsTop() - ePlayer1->getRadius();
    float fPlayerMaximumY = ePlayer1->getBoundsBottom() + ePlayer1->getRadius();

    float fMouseDeltaY = MouseMaximumY - MouseMinimumY;
    float fPlayerDeltaY = fPlayerMaximumY - fPlayerMinimumY;

    vMouseRatio.y = fPlayerDeltaY / fMouseDeltaY;
    pMouseDelta.y = fPlayerMinimumY - (vMouseRatio.y * MouseMinimumY);
}

void AHGame::setMousePositionPlayer1(float MouseX, float MouseY)
{
    AHPoint pPosition;

    pPosition.x = vMouseRatio.x * MouseX + pMouseDelta.x;
    pPosition.y = vMouseRatio.y * MouseY + pMouseDelta.y;

    setPositionPlayer1(pPosition);
}

void AHGame::setMousePositionPlayer2(float MouseX, float MouseY)
{
    AHPoint pPosition;

    pPosition.x = vMouseRatio.x * MouseX + pMouseDelta.x;
    pPosition.y = vMouseRatio.y * MouseY + pMouseDelta.y;

    setPositionPlayer2(pPosition);
}
