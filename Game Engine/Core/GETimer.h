
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Core
//
//  --- GETimer.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

class GETimer
{
protected:
    double dStartTime;
    double dEndTime;
    bool bRunning;

public:
    GETimer();
    virtual ~GETimer();

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual double getTime() = 0;	// microseconds
};
