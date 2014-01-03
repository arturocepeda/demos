
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Audio
//
//  --- GEAudio.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GETypes.h"

#define CHANNELS 256
#define SOUNDS 256

class GEAudio
{
protected:
    float fVolume[CHANNELS];

public:
    GEAudio();
    virtual ~GEAudio();

    virtual void init() = 0;
    virtual void update();
    virtual void release() = 0;

    virtual void loadSound(unsigned int Sound, const char* Filename) = 0;
    virtual void unloadSound(unsigned int Sound) = 0;
    virtual void unloadAllSounds() = 0;

    virtual void playSound(unsigned int Sound, unsigned int Channel) = 0;
    virtual void stop(unsigned int Channel) = 0;

    virtual bool isPlaying(unsigned int Channel) = 0;

    virtual void setListenerPosition(const GEVector3& Position) = 0;
    virtual void setVolume(unsigned int Channel, float Volume) = 0;
    virtual void setPosition(unsigned int Channel, const GEVector3& Position) = 0;
};
