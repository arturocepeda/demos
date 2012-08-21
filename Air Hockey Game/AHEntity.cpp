
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AHEntity.cpp ---
*/

#include <cfloat>
#include <cmath>
#include "AHEntity.h"

Entity::Entity(float Mass)
{
    pPosition.x = 0.0f;
    pPosition.y = 0.0f;

    vVelocity.x = 0.0f;
    vVelocity.y = 0.0f;

    fMass = 0.0f;
    fFriction = 0.0f;

    if(Mass > 0.0f)
        fMass = Mass;

    fBoundsTop = -FLT_MAX;
    fBoundsBottom = FLT_MAX;
    fBoundsLeft = -FLT_MAX;
    fBoundsRight = FLT_MAX;
}

Entity::~Entity()
{
}

void Entity::update()
{
    // update position
    pPosition.x += vVelocity.x;
    pPosition.y += vVelocity.y;

    // update velocty
    AHVector vFriction;

    vFriction.x = vVelocity.x * fFriction * -1;
    vFriction.y = vVelocity.y * fFriction * -1;

    vVelocity.x += vFriction.x;
    vVelocity.y += vFriction.y;

    if(vVelocity.x < 0.25f && vVelocity.x > -0.25f)
        vVelocity.x = 0.0f;

    if(vVelocity.y < 0.25f && vVelocity.y > -0.25f)
        vVelocity.y = 0.0f;
}

void Entity::stop()
{
    vVelocity.x = 0.0f;
    vVelocity.y = 0.0f;
}

void Entity::applyForce(const AHVector& Force)
{
    vVelocity.x += Force.x;
    vVelocity.y += Force.y;
}

void Entity::setPosition(const AHPoint& Position)
{
    pPosition = Position;

    if(pPosition.x < fBoundsLeft)
        pPosition.x = fBoundsLeft;
    else if(pPosition.x > fBoundsRight)
        pPosition.x = fBoundsRight;

    // "y" grows towards bottom
    if(fBoundsTop < fBoundsBottom)
    {
        if(pPosition.y > fBoundsBottom)
            pPosition.y = fBoundsBottom;
        else if(pPosition.y < fBoundsTop)
            pPosition.y = fBoundsTop;
    }

    // "y" grows towards top
    else
    {
        if(pPosition.y < fBoundsBottom)
            pPosition.y = fBoundsBottom;
        else if(pPosition.y > fBoundsTop)
            pPosition.y = fBoundsTop;
    }
}

void Entity::setVelocity(const AHVector& Velocity)
{
    vVelocity = Velocity;
}

void Entity::setFriction(float Friction)
{
    fFriction = Friction;

    if(Friction > 1.0f)
        fFriction = 1.0f;

    if(Friction < 0.0001f)
        fFriction = 0.0001f;
}

void Entity::setBounds(float BoundsTop, float BoundsBottom, float BoundsLeft, float BoundsRight)
{
    fBoundsTop = BoundsTop;
    fBoundsBottom = BoundsBottom;
    fBoundsLeft = BoundsLeft;
    fBoundsRight = BoundsRight;
}

float Entity::getBoundsTop()
{
    return fBoundsTop;
}

float Entity::getBoundsBottom()
{
    return fBoundsBottom;
}

float Entity::getBoundsLeft()
{
    return fBoundsLeft;
}

float Entity::getBoundsRight()
{
    return fBoundsRight;
}

AHPoint Entity::getPosition()
{
    return pPosition;
}

AHVector Entity::getVelocity()
{
    return vVelocity;
}

float Entity::getSquareSpeed()
{
    return (vVelocity.x * vVelocity.x + vVelocity.y * vVelocity.y);
}

float Entity::getSpeed()
{
    return sqrt(vVelocity.x * vVelocity.x + vVelocity.y * vVelocity.y);
}

CircularEntity::CircularEntity(float Mass, float Radius) : Entity(Mass)
{
    fRadius = 1;

    if(Radius > 1)
        fRadius = Radius;
}

CircularEntity::~CircularEntity()
{
}

void CircularEntity::setPosition(const AHPoint& Position)
{
    AHPoint pNewPosition = Position;

    if((pNewPosition.x - fRadius) < fBoundsLeft)
        pNewPosition.x = fBoundsLeft + fRadius;
    else if((pNewPosition.x + fRadius) > fBoundsRight)
        pNewPosition.x = fBoundsRight - fRadius;

    // y grows towards bottom
    if(fBoundsTop < fBoundsBottom)
    {
        if((pNewPosition.y + fRadius) > fBoundsBottom)
            pNewPosition.y = fBoundsBottom - fRadius;
        else if((pNewPosition.y - fRadius) < fBoundsTop)
            pNewPosition.y = fBoundsTop + fRadius;
    }

    // y grows towards top
    else
    {
        if((pNewPosition.y - fRadius) < fBoundsBottom)
            pNewPosition.y = fBoundsBottom + fRadius;
        else if((pNewPosition.y + fRadius) > fBoundsTop)
            pNewPosition.y = fBoundsTop - fRadius;
    }

    pPosition = pNewPosition;
}

float CircularEntity::getRadius()
{
    return fRadius;
}
