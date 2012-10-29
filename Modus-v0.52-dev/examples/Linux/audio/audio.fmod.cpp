
////////////////////////////////////////////////////////////////////////
//
//  Modus
//  C++ Music Library
//  (Examples)
//
//  Arturo Cepeda
//  July 2012
//
//  --- audio.fmod.cpp ---
//
//  --------------------------------------------------------------------
//
//  Audio engine: 
//
//  FMOD Sound System
//  Copyright (c) Firelight Technologies Pty, Ltd., 1994-2012
//
////////////////////////////////////////////////////////////////////////

#include "audio.fmod.h"
#include <stdio.h>
#include <stdlib.h>

FMOD::System* CAudio::fSystem;

void CAudio::ERRCHECK(FMOD_RESULT result)
{
   if(result != FMOD_OK)
   {
      printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      exit(-1);
   }
}

void CAudio::init(unsigned int DSPBufferSize)
{
    FMOD_RESULT result;
   
    // create a system object and initialize.
    result = FMOD::System_Create(&fSystem);
    ERRCHECK(result);

    // set a small DSP buffer size to achieve low latency
    fSystem->setDSPBufferSize(DSPBufferSize, 4);

    // set ALSA as output type
    fSystem->setOutput(FMOD_OUTPUTTYPE_ALSA);

    // initialize FMOD system
    result = fSystem->init(32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);
}

void CAudio::update()
{
    fSystem->update();
}

void CAudio::release()
{    
    fSystem->close();
    fSystem->release();
}

FMOD::System* CAudio::getSoundSystem()
{
    return fSystem;
}
