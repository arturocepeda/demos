
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Core (Win32)
//
//  --- GETimerWin32.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GETimerWin32.h"

GETimerWin32::GETimerWin32()
{
    QueryPerformanceFrequency(&iFrequency);
    iStartCount.QuadPart = 0;
    iEndCount.QuadPart = 0;
}

void GETimerWin32::start()
{
    bRunning = true;
    QueryPerformanceCounter(&iStartCount);
}

void GETimerWin32::stop()
{
    bRunning = false;
    QueryPerformanceCounter(&iEndCount);
}

double GETimerWin32::getTime()
{
    if(bRunning)
        QueryPerformanceCounter(&iEndCount);

    dStartTime = iStartCount.QuadPart * (1000000.0 / iFrequency.QuadPart);
    dEndTime = iEndCount.QuadPart * (1000000.0 / iFrequency.QuadPart);

    return (dEndTime - dStartTime);
}
