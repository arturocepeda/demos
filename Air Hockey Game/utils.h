
/*
    Arturo Cepeda Pérez

    --- utils.h ---
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <cmath>

// -------------------------------------------
// class CLine
// -------------------------------------------

class CLine
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
    CLine(double x0, double y0, double x1, double y1);
    ~CLine();

    bool can_calculate_y();
    double y(double x);

    bool can_calculate_x();
    double x(double y);
};


// -------------------------------------------
// some simple numerical functions
// -------------------------------------------

int max(int a, int b);
int min(int a, int b);

float max(float a, float b);
float min(float a, float b);

bool positive(float number);
bool same_sign(float a, float b);
int round(float Value);


// ------------------------------------------
// random functions
// ------------------------------------------

int random(int Min, int Max);
bool eventOccurs(int Percentage);


#endif
