
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
#include <windows.h>

FMOD::System* CAudio::fSystem;

void CAudio::init(unsigned int DSPBufferSize)
{
    FMOD_RESULT result;
    unsigned int version;
    int numdrivers;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS caps;
    char name[256];

    // Create a System object and initialize.
    result = FMOD::System_Create(&fSystem);
    ERRCHECK(result);
    result = fSystem->getVersion(&version);
    ERRCHECK(result);

    if(version < FMOD_VERSION)
    {
        sprintf(name, "Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
        MessageBox(NULL, name, "FMOD error", 0);
        return;
    }

    result = fSystem->getNumDrivers(&numdrivers);
    ERRCHECK(result);

    if(numdrivers == 0)
    {
        result = fSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ERRCHECK(result);
    }
    else
    {
        result = fSystem->getDriverCaps(0, &caps, 0, &speakermode);
        ERRCHECK(result);
    
        // Set the user selected speaker mode.
        result = fSystem->setSpeakerMode(speakermode);
        ERRCHECK(result);

        if(caps & FMOD_CAPS_HARDWARE_EMULATED)
        {
            // The user has the 'Acceleration' slider set to off! This is really bad for latency! You might want to warn the user about this.
            result = fSystem->setDSPBufferSize(1024, 10);
            ERRCHECK(result);
        }

        result = fSystem->getDriverInfo(0, name, 256, 0);
        ERRCHECK(result);

        if(strstr(name, "SigmaTel"))
        {
            // Sigmatel sound devices crackle for some reason if the format is PCM 16bit. PCM floating point output seems to solve it.
            result = fSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
            ERRCHECK(result);
        }
    }

    // set a small DSP buffer size to achieve low latency
    result = fSystem->setDSPBufferSize(DSPBufferSize, 4);
    ERRCHECK(result);

    result = fSystem->init(100, FMOD_INIT_NORMAL, 0);

    if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
        // Ok, the speaker mode selected isn't supported by this soundcard. Switch it back to stereo...
        result = fSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);

        // ...and re-init.
        result = fSystem->init(100, FMOD_INIT_NORMAL, 0);
        ERRCHECK(result);
    }
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

void CAudio::ERRCHECK(FMOD_RESULT result)
{
    if(result != FMOD_OK)
    {
        char sString[32];
        sprintf(sString, "FMOD error! (%d)", result, FMOD_ErrorString(result));
        MessageBox(NULL, sString, "FMOD error", 0);
        exit(-1);
    }
}
