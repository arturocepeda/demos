
/*
   Arturo Cepeda Pérez
 
   Audio System (OpenAL)
 
   --- GEAudio.mm ---
 */

#include "GEAudio.h"
#include <iostream>
#include <fstream>

GEAudio::GEAudio()
{
   alDevice = alcOpenDevice(NULL);
   alContext = alcCreateContext(alDevice, NULL);
   alcMakeContextCurrent(alContext);
      
   alGenBuffers(BUFFERS, alBuffers);	
   alGenSources(SOURCES, alSourcesAlloc);
   
   for(int i = 0; i < SOURCES; i++)
      alSources[i].Source = alSourcesAlloc[i];
   
   iCurrentSourceAssignment = 0;
   setListenerPosition(0.0f, 0.0f, 0.0f);
}

GEAudio::~GEAudio()
{
   alDeleteSources(SOURCES, alSourcesAlloc);
   alDeleteBuffers(BUFFERS, alBuffers);

   alcMakeContextCurrent(NULL);
   alcDestroyContext(alContext);
   alcCloseDevice(alDevice);
}

void GEAudio::loadSound(unsigned int SoundIndex, NSString* SoundFile)
{
   if(SoundIndex >= BUFFERS)
      return;

   NSString* sSoundFilePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: SoundFile];
   const char* sSoundFile = [sSoundFilePath UTF8String];
   std::ifstream iFile(sSoundFile);
   
   if(!iFile)
      return;

   CFURLRef cfFileURL = (CFURLRef)[NSURL fileURLWithPath:sSoundFilePath];

   ALenum alFormat;
   ALsizei alSize;
	ALsizei alFrequency;
   ALchar* alData = NULL;
   bool bSuccess;

   // load audio file
   bSuccess = loadAudioFile(cfFileURL, &alFormat, (void**)&alData, &alSize, &alFrequency);
   
   // load audio data into the specified buffer
   if(bSuccess)
      alBufferData(alBuffers[SoundIndex], alFormat, alData, alSize, alFrequency);

   if(alData)
      free(alData);
}

void GEAudio::unloadSound(unsigned int SoundIndex)
{
   if(SoundIndex < BUFFERS)
      alBufferData(alBuffers[SoundIndex], 0, NULL, 0, 0);
}

void GEAudio::unloadAllSounds()
{
   for(unsigned int i = 0; i < BUFFERS; i++)
      alBufferData(alBuffers[i], 0, NULL, 0, 0);
}

int GEAudio::assignSource(unsigned int EntityID)
{
   unsigned int iIndexOldestAssignment = 0;
   unsigned int iOldestAssignment = alSources[0].Assignment;
   
   for(int i = 0; i < SOURCES; i++)
   {
      // this source is free
      if(alSources[i].Free)
      {
         assignSource(i, EntityID);
         return i;
      }
      
      if(alSources[i].Assignment < iOldestAssignment)
      {
         iOldestAssignment = alSources[i].Assignment;
         iIndexOldestAssignment = i;
      }
   }
   
   // there are no free sources
   assignSource(iIndexOldestAssignment, EntityID);
   return iIndexOldestAssignment;
}

void GEAudio::assignSource(unsigned int iSourceIndex, unsigned int iEntityID)
{
   alSourceStop(alSources[iSourceIndex].Source);
   alSources[iSourceIndex].EntityID = iEntityID;
   alSources[iSourceIndex].Assignment = ++iCurrentSourceAssignment;
   alSources[iSourceIndex].Free = false;
}

void GEAudio::releaseSource(unsigned int SourceIndex)
{
   if(SourceIndex < SOURCES)
   {
      alSources[SourceIndex].EntityID = 0;
      alSources[SourceIndex].Free = true;
   }
}

void GEAudio::cleanSources()
{
   for(unsigned int i = 0; i < SOURCES; i++)
      alSourcei(alSources[i].Source, AL_BUFFER, NULL);
}

void GEAudio::playSound(unsigned int SoundIndex, unsigned int SourceIndex)
{
   if(SoundIndex < BUFFERS && SourceIndex < SOURCES)
   {      
      alSourcei(alSources[SourceIndex].Source, AL_BUFFER, alBuffers[SoundIndex]);   
      alSourcePlay(alSources[SourceIndex].Source);
   }
}

void GEAudio::stop(unsigned int SourceIndex)
{
   if(SourceIndex < SOURCES)
      alSourceStop(alSources[SourceIndex].Source);
}

void GEAudio::moveListener(float dX, float dY, float dZ)
{
   float fCurrentX, fCurrentY, fCurrentZ;

   alGetListener3f(AL_POSITION, &fCurrentX, &fCurrentY, &fCurrentZ);
   
   fCurrentX += dX;
   fCurrentY += dY;
   fCurrentZ += dZ;
   
   alListener3f(AL_POSITION, fCurrentX, fCurrentY, fCurrentZ);
}

void GEAudio::moveSource(unsigned int SourceIndex, float dX, float dY, float dZ)
{
   if(SourceIndex >= SOURCES)
      return;

   float fCurrentX, fCurrentY, fCurrentZ;

   alGetSource3f(alSources[SourceIndex].Source, AL_POSITION, &fCurrentX, &fCurrentY, &fCurrentZ);
   
   fCurrentX += dX;
   fCurrentY += dY;
   fCurrentZ += dZ;
   
   alSource3f(alSources[SourceIndex].Source, AL_POSITION, fCurrentX, fCurrentY, fCurrentZ);
}

unsigned int GEAudio::getEntityID(unsigned int SourceIndex)
{
   if(SourceIndex < SOURCES)
      return alSources[SourceIndex].EntityID;
   else
      return 0;
}

void GEAudio::setListenerPosition(float X, float Y, float Z)
{
   alListener3f(AL_POSITION, X, Y, Z);
}

void GEAudio::setSourceVolume(unsigned int SourceIndex, float Volume)
{
   if(SourceIndex < SOURCES)
      alSourcef(alSources[SourceIndex].Source, AL_GAIN, Volume);
}

void GEAudio::setSourcePosition(unsigned int SourceIndex, float X, float Y, float Z)
{
   if(SourceIndex < SOURCES)
      alSource3f(alSources[SourceIndex].Source, AL_POSITION, X, Y, Z);
}

void GEAudio::setSourceDirection(unsigned int SourceIndex, float X, float Y, float Z)
{
   if(SourceIndex < SOURCES)      
      alSource3f(alSources[SourceIndex].Source, AL_DIRECTION, X, Y, Z);
}

bool GEAudio::loadAudioFile(CFURLRef cfURL, ALenum* alFormat, ALvoid** alData, ALsizei* alSize, 
                            ALsizei* alFrequency)
{
   ExtAudioFileRef sAudioFileRef;
   OSStatus osError;
   
   // get file reference
   osError = ExtAudioFileOpenURL(cfURL, &sAudioFileRef);

   if(osError != noErr)
      return false;

   // get the number of audio frames from the file
   UInt32 iSize;
   UInt64 iFileLengthFrames; 

   iSize = sizeof(UInt64);

   osError = ExtAudioFileGetProperty(sAudioFileRef, kExtAudioFileProperty_FileLengthFrames, 
                                     &iSize, (void*)&iFileLengthFrames);

   // get the description of the audio file
   AudioStreamBasicDescription sDescription;

   iSize = sizeof(AudioStreamBasicDescription);

   osError = ExtAudioFileGetProperty(sAudioFileRef, kExtAudioFileProperty_FileDataFormat, 
                                     &iSize, (void*)&sDescription);

   // define the new PCM format
   sDescription.mFormatID = kAudioFormatLinearPCM;
   sDescription.mFormatFlags = kAudioFormatFlagsNativeEndian |
                               kAudioFormatFlagIsSignedInteger |
                               kAudioFormatFlagIsPacked;

   if(sDescription.mChannelsPerFrame > 2)
      sDescription.mChannelsPerFrame = 2;

   sDescription.mBitsPerChannel = 16;   
   sDescription.mBytesPerFrame = sDescription.mChannelsPerFrame * sDescription.mBitsPerChannel / 8;
   sDescription.mFramesPerPacket = 1;
   sDescription.mBytesPerPacket = sDescription.mBytesPerFrame * sDescription.mFramesPerPacket;
   
   // apply the new format
   osError = ExtAudioFileSetProperty(sAudioFileRef, kExtAudioFileProperty_ClientDataFormat,
                                     iSize, &sDescription);
   if(osError != noErr)
   {
      free(*alData);
      return false;
   }

   // load audio data from the file
   UInt32 iStreamSizeInBytes = (UInt32)(sDescription.mBytesPerFrame * iFileLengthFrames);

   *alData = malloc(iStreamSizeInBytes);

   if(!(*alData))
      return false;

   AudioBufferList sBufferList;
   UInt32 iNumFramesRead = (UInt32)iFileLengthFrames;
   
   sBufferList.mNumberBuffers = 1;
   sBufferList.mBuffers[0].mNumberChannels = sDescription.mChannelsPerFrame;  
   sBufferList.mBuffers[0].mDataByteSize = sDescription.mBytesPerFrame * iNumFramesRead;
   sBufferList.mBuffers[0].mData = (char*)*alData;
   
   osError = ExtAudioFileRead(sAudioFileRef, &iNumFramesRead, &sBufferList);
   
   if(osError != noErr)
   {
      free(*alData);
      return false;
   }
   
   // information for OpenAL buffer
   *alFormat = (sDescription.mChannelsPerFrame == 2)? AL_FORMAT_STEREO16: AL_FORMAT_MONO16;
   *alFrequency = (ALsizei)sDescription.mSampleRate;
   *alSize = iNumFramesRead * sDescription.mBytesPerFrame;

   return true;
}
