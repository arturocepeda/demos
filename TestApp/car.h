
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
    GE::Vector3 vPosition;
    float fAngle;

    GE::Vector3 vForward;
    float fSpeed;

    float fRotation;
    float fAcceleration;

    bool bReverse;

    void updateStatus(float fDeltaTime);
    void calculateSteering(float fDeltaTime, GE::Vector3& vTargetPoint);

public:
    CCar();
    ~CCar();

    void update(float DeltaTime, GE::Vector3& TargetPoint);

    const GE::Vector3& getPosition();
    float getAngle();
};
