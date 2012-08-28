
/*
    Arturo Cepeda Prez

    --- Utils.cpp ---
*/

#include "utils.h"


// -------------------------------------------
// class Line
// -------------------------------------------

CLine::CLine(double x0, double y0, double x1, double y1)
{
	Dx = x1 - x0;
	Dy = y1 - y0;

	vertical = false;
	horizontal = false;

	if(Dx == 0.0)
	{
		vertical = true;
		px0 = x0;
	}
	else if(Dy == 0.0)
	{
		horizontal = true;
		py0 = y0;
	}
	else
	{
		m = Dy / Dx;
		b = y0 - (m * x0);
	}
}

CLine::~CLine()
{
}

bool CLine::can_calculate_y()
{
	// it doesn't exist any solution if Dx = 0 (no solution or infinite)
	if(vertical)
		return false;
	else
		return true;
}

double CLine::y(double x)
{
	if(horizontal)
		return py0;

	return (m * x) + b;
}

bool CLine::can_calculate_x()
{
	// it doesn't exist any solution if Dy = 0 (no solution or infinite)
	if(horizontal)
		return false;
	else
		return true;
}

double CLine::x(double y)
{
	if(vertical)
		return px0;

	return (y - b) / m;
}


// -------------------------------------------
// some simple numerical functions
// -------------------------------------------

int max(int a, int b)
{
	return (a >= b)? a: b;
}

int min(int a, int b)
{
	return (a <= b)? a: b;
}

float max(float a, float b)
{
	return (a >= b)? a: b;
}

float min(float a, float b)
{
	return (a <= b)? a: b;
}

bool positive(float number)
{
	return (number >= 0)? true: false;
}

bool same_sign(float a, float b)
{
	if(a >= 0.0f && b >= 0.0f)
		return true;
	else if(a < 0.0f && b < 0.0f)
		return true;
	else
		return false;
}

int round(float Value)
{
	return (int)floor(Value + 0.5f);
}


// ------------------------------------------
// random functions
// ------------------------------------------

int random(int Min, int Max)
{
    return (rand() % (Max - Min + 1) + Min);
}

bool eventOccurs(int Percentage)
{
	return ((rand() % 100) < Percentage);
}
