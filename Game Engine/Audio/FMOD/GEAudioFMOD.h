
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Audio (FMOD)
//
//  --- GEAudioFMOD.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "../GEAudio.h"
#include "fmod.hpp"
#include "fmod_errors.h"

class GEAudioFMOD : public GEAudio
{
private:
    FMOD::System* sSystem;
    FMOD::Channel* sChannels[CHANNELS];
    FMOD::Sound* sSounds[SOUNDS];

    FMOD_VECTOR pPosition[CHANNELS];

    void ERRCHECK(FMOD_RESULT result);

public:
    GEAudioFMOD();
    ~GEAudioFMOD();

    virtual void init();
    virtual void update();
    virtual void release();

    virtual void loadSound(unsigned int Sound, const char* Filename);
    virtual void unloadSound(unsigned int Sound);

    virtual void playSound(unsigned int Sound, unsigned int Channel);
    virtual void stop(unsigned int Channel);

    virtual bool isPlaying(unsigned int Channel);

    virtual void setListenerPosition(float X, float Y, float Z);
    virtual void setVolume(unsigned int Channel, float Volume);
    virtual void setPosition(unsigned int Channel, float X, float Y, float Z);
};
