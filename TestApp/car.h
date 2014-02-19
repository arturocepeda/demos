
/*
    Arturo Cepeda Pérez

    --- car.h ---
*/

#pragma once

#include "Core/GETypes.h"
#include "main.h"

class CCar
{
private:
    GEVector3 vPosition;
    float fAngle;

    GEVector3 vForward;
    float fSpeed;

    float fRotation;
    float fAcceleration;

    bool bReverse;

    void updateStatus(float fDeltaTime);
    void calculateSteering(float fDeltaTime, GEVector3& vTargetPoint);

public:
    CCar();
    ~CCar();

    void update(float DeltaTime, GEVector3& TargetPoint);

    const GEVector3& getPosition();
    float getAngle();
};
