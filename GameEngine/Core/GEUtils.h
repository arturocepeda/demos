
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Core
//
//  --- GEUtils.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include <stdlib.h>
#include <cmath>

#include "GEConstants.h"

//
//  GENonCopyable
//
class GENonCopyable
{
protected:
    GENonCopyable() {}
    ~GENonCopyable() {}

private:
    GENonCopyable(const GENonCopyable&);
    GENonCopyable& operator = (const GENonCopyable&);
};

//
//  GELine
//
class GELine
{
private:
	double px0;
	double py0;

	double Dx;
	double Dy;

	double m;
	double b;

	bool vertical;
	bool horizontal;

public:
	GELine(double x0, double y0, double x1, double y1);
	~GELine();

	bool can_calculate_y();
	double y(double x);

	bool can_calculate_x();
	double x(double y);
};


//
//  Some simple numerical functions
//
bool positive(float number);
bool same_sign(float a, float b);
int round(float Value);

inline float getSimplifiedAngle(float angle)
{
    if(angle > PI)
        angle -= DOUBLEPI;
    else if(angle < -PI)
        angle += DOUBLEPI;

    return angle;
}


//
//  Random functions
//
int random(int Min, int Max);
bool eventOccurs(int Percentage);
