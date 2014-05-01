
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  iOS Game Engine
//
//  Audio System (OpenAL)
//
//  --- GEAudioOpenAL.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Audio/GEAudio.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/ExtendedAudioFile.h>

#define SOURCES   32
#define BUFFERS   256

struct SOpenALSource
{
   ALuint Source;
   unsigned int EntityID;
   unsigned int Assignment;
   bool Free;
   
   SOpenALSource()
   {
      Source = 0;
      EntityID = 0;
      Assignment = 0;
      Free = true;
   }
};

class GEAudioOpenAL : public GEAudio
{
private:
   ALCdevice* alDevice;
   ALCcontext* alContext;
   
   ALuint alSourcesAlloc[SOURCES];
   SOpenALSource alSources[SOURCES];
   ALuint alBuffers[BUFFERS];
   
   unsigned int iCurrentSourceAssignment;
   
   void assignSource(unsigned int iSourceIndex, unsigned int iEntityID);
   bool loadAudioFile(CFURLRef cfURL, ALenum* alFormat, ALvoid** alData, ALsizei* alSize, 
                      ALsizei* alFreq);

public:
   void init();
   void release();
   
   int assignSource(unsigned int EntityID);
   void releaseSource(unsigned int SourceIndex);
   void cleanSources();
   
   void loadSound(unsigned int SoundIndex, const char* SoundFile);
   void unloadSound(unsigned int SoundIndex);
   void unloadAllSounds();
   
   void playSound(unsigned int SoundIndex, unsigned int SourceIndex);
   void stop(unsigned int SourceIndex);

   bool isPlaying(unsigned int SourceIndex);
   
   void moveListener(const GEVector3& Delta);
   void moveSource(unsigned int SourceIndex, const GEVector3& Delta);
   
   unsigned int getEntityID(unsigned int SourceIndex);
   
   void setListenerPosition(const GEVector3& Position);
   void setVolume(unsigned int SourceIndex, float Volume);
   void setPosition(unsigned int SourceIndex, const GEVector3& Position);
   void setDirection(unsigned int SourceIndex, const GEVector3& Direction);
};
