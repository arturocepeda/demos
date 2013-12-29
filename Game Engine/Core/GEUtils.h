
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


//
//  Random functions
//
int random(int Min, int Max);
bool eventOccurs(int Percentage);
