
/*
    Arturo Cepeda Pérez

	Timer (Win32)

    --- GETimer.h ---
*/

#ifndef _GETIMER_H_
#define _GETIMER_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class GETimer
{
private:
    double dStartTime;
    double dEndTime;
    bool bRunning;

    LARGE_INTEGER iFrequency;
    LARGE_INTEGER iStartCount;
    LARGE_INTEGER iEndCount;

public:
    GETimer();
    ~GETimer();

    void start();
    void stop();
    double getTime();	// microseconds
};

#endif
