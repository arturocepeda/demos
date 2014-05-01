
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Audio System (OpenAL)
//
//  --- GEAudioOpenAL.mm ---
//
//////////////////////////////////////////////////////////////////

#include "GEAudioOpenAL.h"
#include <iostream>
#include <fstream>

void GEAudioOpenAL::init()
{
   alDevice = alcOpenDevice(NULL);
   alContext = alcCreateContext(alDevice, NULL);
   alcMakeContextCurrent(alContext);
      
   alGenBuffers(BUFFERS, alBuffers);	
   alGenSources(SOURCES, alSourcesAlloc);
   
   for(int i = 0; i < SOURCES; i++)
      alSources[i].Source = alSourcesAlloc[i];
   
   iCurrentSourceAssignment = 0;
   setListenerPosition(GEVector3(0.0f, 0.0f, 0.0f));
}

void GEAudioOpenAL::release()
{
   alDeleteSources(SOURCES, alSourcesAlloc);
   alDeleteBuffers(BUFFERS, alBuffers);

   alcMakeContextCurrent(NULL);
   alcDestroyContext(alContext);
   alcCloseDevice(alDevice);
}

void GEAudioOpenAL::loadSound(unsigned int SoundIndex, const char* SoundFile)
{
   if(SoundIndex >= BUFFERS)
      return;

   NSString* nsSoundFile = [NSString stringWithUTF8String: SoundFile];
   NSString* nsSoundFilePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: nsSoundFile];
   const char* sSoundFile = [nsSoundFilePath UTF8String];
   std::ifstream iFile(sSoundFile);
   
   if(!iFile)
      return;

   CFURLRef cfFileURL = (CFURLRef)[NSURL fileURLWithPath: nsSoundFilePath];

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

void GEAudioOpenAL::unloadSound(unsigned int SoundIndex)
{
   if(SoundIndex < BUFFERS)
      alBufferData(alBuffers[SoundIndex], 0, NULL, 0, 0);
}

void GEAudioOpenAL::unloadAllSounds()
{
   for(unsigned int i = 0; i < BUFFERS; i++)
      alBufferData(alBuffers[i], 0, NULL, 0, 0);
}

int GEAudioOpenAL::assignSource(unsigned int EntityID)
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

void GEAudioOpenAL::assignSource(unsigned int iSourceIndex, unsigned int iEntityID)
{
   alSourceStop(alSources[iSourceIndex].Source);
   alSources[iSourceIndex].EntityID = iEntityID;
   alSources[iSourceIndex].Assignment = ++iCurrentSourceAssignment;
   alSources[iSourceIndex].Free = false;
}

void GEAudioOpenAL::releaseSource(unsigned int SourceIndex)
{
   if(SourceIndex < SOURCES)
   {
      alSources[SourceIndex].EntityID = 0;
      alSources[SourceIndex].Free = true;
   }
}

void GEAudioOpenAL::cleanSources()
{
   for(unsigned int i = 0; i < SOURCES; i++)
      alSourcei(alSources[i].Source, AL_BUFFER, NULL);
}

void GEAudioOpenAL::playSound(unsigned int SoundIndex, unsigned int SourceIndex)
{
   if(SoundIndex < BUFFERS && SourceIndex < SOURCES)
   {      
      alSourcei(alSources[SourceIndex].Source, AL_BUFFER, alBuffers[SoundIndex]);   
      alSourcePlay(alSources[SourceIndex].Source);
   }
}

void GEAudioOpenAL::stop(unsigned int SourceIndex)
{
   if(SourceIndex < SOURCES)
      alSourceStop(alSources[SourceIndex].Source);
}

bool GEAudioOpenAL::isPlaying(unsigned int SourceIndex)
{
   ALenum alState;
    
   alGetSourcei(SourceIndex, AL_SOURCE_STATE, &alState);
    
   return alState == AL_PLAYING;
}

void GEAudioOpenAL::moveListener(const GEVector3& Delta)
{
   float fCurrentX, fCurrentY, fCurrentZ;

   alGetListener3f(AL_POSITION, &fCurrentX, &fCurrentY, &fCurrentZ);
   
   fCurrentX += Delta.X;
   fCurrentY += Delta.Y;
   fCurrentZ += Delta.Z;
   
   alListener3f(AL_POSITION, fCurrentX, fCurrentY, fCurrentZ);
}

void GEAudioOpenAL::moveSource(unsigned int SourceIndex, const GEVector3& Delta)
{
   if(SourceIndex >= SOURCES)
      return;

   float fCurrentX, fCurrentY, fCurrentZ;

   alGetSource3f(alSources[SourceIndex].Source, AL_POSITION, &fCurrentX, &fCurrentY, &fCurrentZ);
   
   fCurrentX += Delta.X;
   fCurrentY += Delta.Y;
   fCurrentZ += Delta.Z;
   
   alSource3f(alSources[SourceIndex].Source, AL_POSITION, fCurrentX, fCurrentY, fCurrentZ);
}

unsigned int GEAudioOpenAL::getEntityID(unsigned int SourceIndex)
{
   if(SourceIndex < SOURCES)
      return alSources[SourceIndex].EntityID;
   else
      return 0;
}

void GEAudioOpenAL::setListenerPosition(const GEVector3& Position)
{
   alListener3f(AL_POSITION, Position.X, Position.Y, Position.Z);
}

void GEAudioOpenAL::setVolume(unsigned int SourceIndex, float Volume)
{
   if(SourceIndex < SOURCES)
      alSourcef(alSources[SourceIndex].Source, AL_GAIN, Volume);
}

void GEAudioOpenAL::setPosition(unsigned int SourceIndex, const GEVector3& Position)
{
   if(SourceIndex < SOURCES)
      alSource3f(alSources[SourceIndex].Source, AL_POSITION, Position.X, Position.Y, Position.Z);
}

void GEAudioOpenAL::setDirection(unsigned int SourceIndex, const GEVector3& Direction)
{
   if(SourceIndex < SOURCES)      
      alSource3f(alSources[SourceIndex].Source, AL_DIRECTION, Direction.X, Direction.Y, Direction.Z);
}

bool GEAudioOpenAL::loadAudioFile(CFURLRef cfURL, ALenum* alFormat, ALvoid** alData, ALsizei* alSize, 
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
      return false;

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
