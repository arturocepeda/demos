
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda
//  July 2012
//
//  --- audio.openal.h ---
//
//  --------------------------------------------------------------------
//
//  Audio engine: 
//
//  OpenAL - Open Audio Library
//
////////////////////////////////////////////////////////////////////////

#ifndef _AUDIO_OPENAL_H_
#define _AUDIO_OPENAL_H_

#include "externals/OpenAL/include/al.h"
#include "externals/OpenAL/include/alc.h"

class CAudio
{
private:
    static ALCcontext* alContext;
    static ALCdevice* alDevice;

public:
    static void init();
    static void release();
};

#endif
