
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Core (Win32)
//
//  --- GETimerWin32.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GETimer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class GETimerWin32 : public GETimer
{
private:
    LARGE_INTEGER iFrequency;
    LARGE_INTEGER iStartCount;
    LARGE_INTEGER iEndCount;

public:
    GETimerWin32();

    virtual void start();
    virtual void stop();
    virtual double getTime();
};
