
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda Pérez
//  July 2012
//
////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

#include "modus.h"

struct SGlobal
{
    bool bRecordingMode;
    MCTimer* mTimer;
    MCScore* mScore;
};

void TimerTick(const MSTimePosition& mTimePosition, void* pData);
void CALLBACK MidiInProc(HMIDIIN mHandle, UINT iMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

void PianoPlayNote(unsigned int iInstrID, const MSNote& mNote, void* pData);
void PianoReleaseNote(unsigned int iInstrID, const MSNote& mNote, void* pData);
void PianoDamper(unsigned int iInstrID, bool bDamperState, void* pData);

#endif
