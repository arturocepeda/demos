
/*
    Arturo Cepeda Pérez

	Audio engine (FMOD)

    --- GEAudio.h ---
*/

// Library links: fmodex_vc.lib

#ifndef _GEAUDIO_H_
#define _GEAUDIO_H_

#include "fmod.hpp"
#include "fmod_errors.h"

#define CHANNELS 256
#define SOUNDS 256

class GEAudio
{
private:
    FMOD::System* sSystem;
    FMOD::Channel* sChannels[CHANNELS];
    FMOD::Sound* sSounds[SOUNDS];

    float fVolume[CHANNELS];
    FMOD_VECTOR pPosition[CHANNELS];

    void ERRCHECK(FMOD_RESULT result);

public:
    GEAudio();
    ~GEAudio();

    void init();
    void update();
    void release();

    void loadSound(unsigned int Sound, const char* Filename);
    void unloadSound(unsigned int Sound);

    void playSound(unsigned int Sound, unsigned int Channel);
    void stop(unsigned int Channel);

    bool isPlaying(unsigned int Channel);

    void setListenerPosition(float X, float Y, float Z);
    void setVolume(unsigned int Channel, float Volume);
    void setPosition(unsigned int Channel, float X, float Y, float Z);
};

#endif
