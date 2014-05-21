
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Audio System (OpenSL)
//
//  --- GEAudioOpenSL.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Audio/GEAudio.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define SOURCES   16
#define BUFFERS   256

class GEAudioOpenSL : public GEAudio
{
private:
   SLObjectItf slEngineObject;
   SLEngineItf slEngine;
   SLObjectItf slOutputMix;

   SLDataLocator_OutputMix slDataLocatorOut;
   SLDataSink slDataSink;

   struct OpenSLSource
   {
      SLObjectItf AudioPlayer;
      SLBufferQueueItf BufferQueue;
		SLPlayItf PlaybackState;
		SLVolumeItf VolumeController;
   };

   OpenSLSource slSources[SOURCES];

   struct OpenSLBuffer
   {
		short Channels;
		int SampleRate;
		char* AudioData;
		int AudioDataSize;

      OpenSLBuffer() : AudioData(NULL) {}
   };

   OpenSLBuffer slBuffers[BUFFERS];

   SLmillibel linearToMillibel(float fGain);
	SLpermille floatToPermille(float fPanning);
	void loadPcmAudioData(unsigned int iSoundIndex, const char* sData, unsigned int iSize);

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
