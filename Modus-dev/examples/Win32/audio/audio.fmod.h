
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda
//  July 2012
//
//  --- audio.fmod.h ---
//
//  --------------------------------------------------------------------
//
//  Audio engine: 
//
//  FMOD Sound System
//  Copyright (c) Firelight Technologies Pty, Ltd., 1994-2012
//
////////////////////////////////////////////////////////////////////////

#ifndef _AUDIO_FMOD_H_
#define _AUDIO_FMOD_H_

#include "externals/FMOD/include/fmod.hpp"
#include "externals/FMOD/include/fmod_errors.h"

class CAudio
{
private:
    static FMOD::System* fSystem;

    static void ERRCHECK(FMOD_RESULT result);

public:
    static void init(unsigned int DSPBufferSize);
    static void release();
    static void update();

    static FMOD::System* getSoundSystem();
};

#endif
