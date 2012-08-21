
/*
    Arturo Cepeda Pérez

    Air Hockey - Game Library (2011)

    --- AHEntity.h ---
*/

#ifndef _AHENTITY_H_
#define _AHENTITY_H_

struct AHPoint
{
    float x;
    float y;
};

struct AHVector
{
    float x;
    float y;
};

int round(float Value);

class Entity
{
protected:
    float fMass;
    float fFriction;

    AHPoint pPosition;
    AHVector vVelocity;

    float fBoundsTop;
    float fBoundsBottom;
    float fBoundsLeft;
    float fBoundsRight;

public:
    Entity(float Mass);
    ~Entity();

    void update();

    void applyForce(const AHVector& Force);
    void stop();
    void setFriction(float FrictionCoefficient);
    void setPosition(const AHPoint& Position);
    void setVelocity(const AHVector& Velocity);
    void setBounds(float BoundsTop, float BoundsBottom, float BoundsLeft, float BoundsRight);

    AHPoint getPosition();
    AHVector getVelocity();
    float getSquareSpeed();
    float getSpeed();

    float getBoundsTop();
    float getBoundsBottom();
    float getBoundsLeft();
    float getBoundsRight();
};

class CircularEntity : public Entity
{
private:
    float fRadius;

public:
    CircularEntity(float Mass, float Radius);
    ~CircularEntity();

    void setPosition(const AHPoint& Position);

    float getRadius();
};

#endif
