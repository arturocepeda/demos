
/*
    Arturo Cepeda Pérez

    --- car.cpp ---
*/

#include "car.h"
#include "Core/GEUtils.h"
#include <algorithm>

CCar::CCar()
    : vPosition(GEVector3(0.0f, 0.0f, 0.0f))
    , fAngle(0.0f)
    , fSpeed(0.0f)
    , fRotation(0.0f)
    , fAcceleration(0.0f)
    , bReverse(false)
{
}

CCar::~CCar()
{
}

void CCar::update(float DeltaTime, GEVector3& TargetPoint)
{
    calculateSteering(DeltaTime, TargetPoint);
    updateStatus(DeltaTime);
}

void CCar::updateStatus(float fDeltaTime)
{
    fSpeed += fAcceleration;
    fAngle += fRotation;

    vForward = GEVector3(sinf(fAngle), 0.0f, cosf(fAngle));
    vPosition += vForward * fDeltaTime * fSpeed;
}

void CCar::calculateSteering(float fDeltaTime, GEVector3& vTargetPoint)
{
    const float CloseEnough = 0.2f;
    const float AngleDifferenceWeight = 2.5f;
    const float MaxRotationSpeed = 0.02f;
    const float MaxAcceleration = 0.02f;
    const float MaxBreak = -0.03f;
    const float BreakWeight = 1.5f;
    const float MaxSpeed = 2.0f;
    const float MaxReverseSpeed = 0.5f;

    // target vector and distance
    GEVector3 vTargetVector = vTargetPoint - vPosition;
    float fDistance = vTargetVector.getLength();

    if(fDistance < CloseEnough)
    {
        fAcceleration = 0.0f;
        fSpeed = 0.0f;
        fRotation = 0.0f;
        return;
    }

    vTargetVector.normalize();

    // rotation
    float fDesiredAngle = getSimplifiedAngle(atan2(vTargetVector.X, vTargetVector.Z));
    float fAngleDifference = getSimplifiedAngle(fDesiredAngle - fAngle);

    if(fAngleDifference < 0.0f)
        fRotation = max(fAngleDifference * fSpeed * fDeltaTime, -MaxRotationSpeed);
    else
        fRotation = min(fAngleDifference * fSpeed * fDeltaTime, MaxRotationSpeed);

    if(!bReverse)
    {
        if(fabs(fAngleDifference) > HALFPI)
            bReverse = true;
    }
    else
    {
        if(fabs(fAngleDifference) < (HALFPI * 0.33f))
        {
            fSpeed = 0.0f;
            bReverse = false;
        }
    }

    // speed
    float fDesiredSpeed = fDistance - fSpeed;
    float fSpeedDifference = fDesiredSpeed - fSpeed;

    if(fSpeedDifference < 0.0f)
        fAcceleration = max(fSpeedDifference * fDeltaTime * (BreakWeight / fSpeed), MaxBreak);
    else if(fSpeed < MaxSpeed)
        fAcceleration = min(fSpeedDifference * fDeltaTime, MaxAcceleration);
    else
        fAcceleration = 0.0f;

    if(bReverse)
    {
        fRotation *= -1.0f;
        fAcceleration *= -1.0f;

        if(fSpeed < -MaxReverseSpeed)
            fAcceleration = 0.0f;
    }
}

const GEVector3& CCar::getPosition()
{
    return vPosition;
}

float CCar::getAngle()
{
    return fAngle;
}
