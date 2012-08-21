
/*
    Arturo Cepeda Pérez

    Audio Engine (FMOD)

    --- GEAudio.cpp ---
*/

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "GEAudio.h"

GEAudio::GEAudio()
{
    memset(sSounds, 0, sizeof(FMOD::Sound*) * SOUNDS);
    memset(sChannels, 0, sizeof(FMOD::Channel*) * CHANNELS);
    memset(pPosition, 0, sizeof(FMOD_VECTOR) * CHANNELS);

    for(unsigned int i = 0; i < CHANNELS; i++)
        fVolume[i] = 1.0f;
}

GEAudio::~GEAudio()
{
}

void GEAudio::ERRCHECK(FMOD_RESULT result)
{
    char sString[256];

    sprintf(sString, "FMOD error! (%d)", result, FMOD_ErrorString(result));

    if(result != FMOD_OK)
    {
        MessageBox(NULL, sString, "FMOD error", 0);
        exit(-1);
    }
}

void GEAudio::init()
{
    FMOD_RESULT result;
    unsigned int version;
    int numdrivers;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS caps;
    char name[256];

    // Create a System object and initialize.
    result = FMOD::System_Create(&sSystem);
    ERRCHECK(result);
    result = sSystem->getVersion(&version);
    ERRCHECK(result);

    if(version < FMOD_VERSION)
    {
        sprintf(name, "Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
        MessageBox(NULL, name, "FMOD error", 0);
        return;
    }

    result = sSystem->getNumDrivers(&numdrivers);
    ERRCHECK(result);

    if(numdrivers == 0)
    {
        result = sSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ERRCHECK(result);
    }
    else
    {
        result = sSystem->getDriverCaps(0, &caps, 0, &speakermode);
        ERRCHECK(result);
    
        // Set the user selected speaker mode.
        result = sSystem->setSpeakerMode(speakermode);
        ERRCHECK(result);

        if(caps & FMOD_CAPS_HARDWARE_EMULATED)
        {
            // The user has the 'Acceleration' slider set to off! This is really bad for latency! You might want to warn the user about this.
            result = sSystem->setDSPBufferSize(1024, 10);
            ERRCHECK(result);
        }

        result = sSystem->getDriverInfo(0, name, 256, 0);
        ERRCHECK(result);

        if(strstr(name, "SigmaTel"))
        {
            // Sigmatel sound devices crackle for some reason if the format is PCM 16bit. PCM floating point output seems to solve it.
            result = sSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
            ERRCHECK(result);
        }
    }

    // set a small DSP buffer size to achieve a low latency
    result = sSystem->setDSPBufferSize(512, 4);
    ERRCHECK(result);

    result = sSystem->init(100, FMOD_INIT_NORMAL, 0);

    if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
        // Ok, the speaker mode selected isn't supported by this soundcard. Switch it back to stereo...
        result = sSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);

        // ...and re-init.
        result = sSystem->init(100, FMOD_INIT_NORMAL, 0);
        ERRCHECK(result);
    }
}

void GEAudio::update()
{
    sSystem->update();
}

void GEAudio::release()
{    
    sSystem->close();
    sSystem->release();
}

void GEAudio::loadSound(unsigned int Sound, const char* Filename)
{
    if(Sound >= SOUNDS)
        return;

    unloadSound(Sound);

    sSystem->createSound(Filename, FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_3D, NULL, &sSounds[Sound]);
}

void GEAudio::unloadSound(unsigned int Sound)
{
    if(Sound >= SOUNDS || !sSounds[Sound])
        return;

    //sSounds[Sound]->release();
    sSounds[Sound] = NULL;
}

void GEAudio::playSound(unsigned int Sound, unsigned int Channel)
{
    sSystem->playSound(FMOD_CHANNEL_FREE, sSounds[Sound], true, &sChannels[Channel]);
    sChannels[Channel]->set3DAttributes(&pPosition[Channel], NULL);
    sChannels[Channel]->setVolume(fVolume[Channel]);
    sChannels[Channel]->setPaused(false);
}

void GEAudio::stop(unsigned int Channel)
{
    sChannels[Channel]->stop();
}

bool GEAudio::isPlaying(unsigned int Channel)
{
    bool bPlaying;
    sChannels[Channel]->isPlaying(&bPlaying);

    return bPlaying;
}

void GEAudio::setListenerPosition(float X, float Y, float Z)
{    
    FMOD_VECTOR fmodListenerPosition = {X, Y, Z};
    sSystem->set3DListenerAttributes(0, &fmodListenerPosition, NULL, NULL, NULL);
}

void GEAudio::setVolume(unsigned int Channel, float Volume)
{
    if(!sChannels[Channel])
        return;

    sChannels[Channel]->setVolume(Volume);
}

void GEAudio::setPosition(unsigned int Channel, float X, float Y, float Z)
{
    pPosition[Channel].x = X;
    pPosition[Channel].y = Y;
    pPosition[Channel].z = Z;

    if(!sChannels[Channel])
        return;

    bool bPlaying;
    sChannels[Channel]->isPlaying(&bPlaying);

    if(bPlaying)
        sChannels[Channel]->set3DAttributes(&pPosition[Channel], NULL);
}
