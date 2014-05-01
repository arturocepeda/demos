
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

    void init() override;
    void update() override;
    void release() override;

    void loadSound(unsigned int Sound, const char* Filename) override;
    void unloadSound(unsigned int Sound) override;
    void unloadAllSounds() override;

    void playSound(unsigned int Sound, unsigned int Channel) override;
    void stop(unsigned int Channel) override;

    bool isPlaying(unsigned int Channel) override;

    void setListenerPosition(const GEVector3& Position) override;
    void setVolume(unsigned int Channel, float Volume) override;
    void setPosition(unsigned int Channel, const GEVector3& Position) override;
};
