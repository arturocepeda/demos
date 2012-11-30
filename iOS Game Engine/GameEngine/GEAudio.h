
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  iOS Game Engine
//
//  Audio System (OpenAL)
//
//  --- GEAudio.h ---
//
//////////////////////////////////////////////////////////////////


#ifndef _GEAUDIO_H_
#define _GEAUDIO_H_

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

class GEAudio
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
   GEAudio();
   ~GEAudio();
   
   int  assignSource(unsigned int EntityID);
   void releaseSource(unsigned int SourceIndex);
   void cleanSources();
   
   void loadSound(unsigned int SoundIndex, NSString* SoundFile);
   void unloadSound(unsigned int SoundIndex);
   void unloadAllSounds();
   
   void playSound(unsigned int SoundIndex, unsigned int SourceIndex);
   void stop(unsigned int SourceIndex);
   
   void moveListener(float dX, float dY, float dZ);
   void moveSource(unsigned int SourceIndex, float dX, float dY, float dZ);
   
   unsigned int getEntityID(unsigned int SourceIndex);
   
   void setListenerPosition(float X, float Y, float Z);
   void setSourceVolume(unsigned int SourceIndex, float Volume);
   void setSourcePosition(unsigned int SourceIndex, float X, float Y, float Z);
   void setSourceDirection(unsigned int SourceIndex, float X, float Y, float Z);
};

#endif
