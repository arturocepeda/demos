
/*
    Arturo Cepeda Pérez

    Timer (Win32)

    --- GETimer.cpp ---
*/

#include "GETimer.h"
#include <stdlib.h>

GETimer::GETimer()
{
    QueryPerformanceFrequency(&iFrequency);
    iStartCount.QuadPart = 0;
    iEndCount.QuadPart = 0;

    bRunning = false;
    dStartTime = 0;
    dEndTime = 0;
}

GETimer::~GETimer()
{
}

void GETimer::start()
{
    bRunning = true;
    QueryPerformanceCounter(&iStartCount);
}

void GETimer::stop()
{
    bRunning = false;
    QueryPerformanceCounter(&iEndCount);
}

double GETimer::getTime()
{
    if(bRunning)
        QueryPerformanceCounter(&iEndCount);

    dStartTime = iStartCount.QuadPart * (1000000.0 / iFrequency.QuadPart);
    dEndTime = iEndCount.QuadPart * (1000000.0 / iFrequency.QuadPart);

    return (dEndTime - dStartTime);
}
