
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Audio (FMOD)
//
//  --- GEAudioFMOD.cpp ---
//
//////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include "GEAudioFMOD.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

GEAudioFMOD::GEAudioFMOD()
{
    memset(sSounds, 0, sizeof(FMOD::Sound*) * SOUNDS);
    memset(sChannels, 0, sizeof(FMOD::Channel*) * CHANNELS);
    memset(pPosition, 0, sizeof(FMOD_VECTOR) * CHANNELS);
}

GEAudioFMOD::~GEAudioFMOD()
{
}

void GEAudioFMOD::ERRCHECK(FMOD_RESULT result)
{
    char sString[256];

    sprintf(sString, "FMOD error! (%d)", result, FMOD_ErrorString(result));

    if(result != FMOD_OK)
    {
        MessageBox(NULL, sString, "FMOD error", 0);
        exit(-1);
    }
}

void GEAudioFMOD::init()
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

void GEAudioFMOD::update()
{
    sSystem->update();
}

void GEAudioFMOD::release()
{    
    sSystem->close();
    sSystem->release();
}

void GEAudioFMOD::loadSound(unsigned int Sound, const char* Filename)
{
    if(Sound >= SOUNDS)
        return;

    unloadSound(Sound);

    sSystem->createSound(Filename, FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_3D, NULL, &sSounds[Sound]);
}

void GEAudioFMOD::unloadSound(unsigned int Sound)
{
    if(Sound >= SOUNDS || !sSounds[Sound])
        return;

    //sSounds[Sound]->release();
    sSounds[Sound] = NULL;
}

void GEAudioFMOD::unloadAllSounds()
{
}

void GEAudioFMOD::playSound(unsigned int Sound, unsigned int Channel)
{
    sSystem->playSound(FMOD_CHANNEL_FREE, sSounds[Sound], true, &sChannels[Channel]);
    sChannels[Channel]->set3DAttributes(&pPosition[Channel], NULL);
    sChannels[Channel]->setVolume(fVolume[Channel]);
    sChannels[Channel]->setPaused(false);
}

void GEAudioFMOD::stop(unsigned int Channel)
{
    sChannels[Channel]->stop();
}

bool GEAudioFMOD::isPlaying(unsigned int Channel)
{
    bool bPlaying;
    sChannels[Channel]->isPlaying(&bPlaying);

    return bPlaying;
}

void GEAudioFMOD::setListenerPosition(const GEVector3& Position)
{    
    FMOD_VECTOR fmodListenerPosition = { Position.X, Position.Y, Position.Z };
    sSystem->set3DListenerAttributes(0, &fmodListenerPosition, NULL, NULL, NULL);
}

void GEAudioFMOD::setVolume(unsigned int Channel, float Volume)
{
    if(!sChannels[Channel])
        return;

    sChannels[Channel]->setVolume(Volume);
}

void GEAudioFMOD::setPosition(unsigned int Channel, const GEVector3& Position)
{
    pPosition[Channel].x = Position.X;
    pPosition[Channel].y = Position.Y;
    pPosition[Channel].z = Position.Z;

    if(!sChannels[Channel])
        return;

    bool bPlaying;
    sChannels[Channel]->isPlaying(&bPlaying);

    if(bPlaying)
        sChannels[Channel]->set3DAttributes(&pPosition[Channel], NULL);
}
