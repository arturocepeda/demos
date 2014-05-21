
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Audio System (OpenSL)
//
//  --- GEAudioOpenSL.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GEAudioOpenSL.h"
#include "GEDevice.h"
#include <iostream>
#include <fstream>

void GEAudioOpenSL::init()
{
   // create engine and output mix
   slCreateEngine(&slEngineObject, 0, NULL, 0, NULL, NULL);
	(*slEngineObject)->Realize(slEngineObject, SL_BOOLEAN_FALSE);
	(*slEngineObject)->GetInterface(slEngineObject, SL_IID_ENGINE, (void*)&slEngine);

   const SLuint32 slEngineIdCount = 1;
	const SLInterfaceID slEngineIds[1] = { SL_IID_ENVIRONMENTALREVERB };
	const SLboolean slEngineReq[1] = { SL_BOOLEAN_FALSE };
	(*slEngine)->CreateOutputMix(slEngine, &slOutputMix, slEngineIdCount, slEngineIds, slEngineReq);
	(*slOutputMix)->Realize(slOutputMix, SL_BOOLEAN_FALSE);

   // define format
   SLDataLocator_AndroidSimpleBufferQueue slDataLocatorIn;
	slDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
	slDataLocatorIn.numBuffers = 1;

	SLDataFormat_PCM slDataFormat;
	slDataFormat.formatType = SL_DATAFORMAT_PCM;
	slDataFormat.numChannels = 1;
   slDataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
	slDataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	slDataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
	slDataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
	slDataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

	SLDataSource slDataSource;
	slDataSource.pLocator = &slDataLocatorIn;
	slDataSource.pFormat = &slDataFormat;

	slDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	slDataLocatorOut.outputMix = slOutputMix;

	slDataSink.pLocator = &slDataLocatorOut;
	slDataSink.pFormat = NULL;

   // create audio players (sources)
   const SLuint32 slAudioPlayerIdCount = 3;
	const SLInterfaceID slAudioPlayerIds[] = { SL_IID_PLAY, SL_IID_BUFFERQUEUE, SL_IID_VOLUME };
	const SLboolean slAudioPlayerReq[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

   for(unsigned int i = 0; i < SOURCES; i++)
   {
	   (*slEngine)->CreateAudioPlayer(slEngine, &slSources[i].AudioPlayer, &slDataSource, &slDataSink, slAudioPlayerIdCount, slAudioPlayerIds, slAudioPlayerReq);
	   (*slSources[i].AudioPlayer)->Realize(slSources[i].AudioPlayer, SL_BOOLEAN_FALSE);

	   (*slSources[i].AudioPlayer)->GetInterface(slSources[i].AudioPlayer, SL_IID_PLAY, &slSources[i].PlaybackState);
	   (*slSources[i].AudioPlayer)->GetInterface(slSources[i].AudioPlayer, SL_IID_BUFFERQUEUE, &slSources[i].BufferQueue);
	   (*slSources[i].AudioPlayer)->GetInterface(slSources[i].AudioPlayer, SL_IID_VOLUME, &slSources[i].VolumeController);

	   (*slSources[i].VolumeController)->EnableStereoPosition(slSources[i].VolumeController, true);
      (*slSources[i].VolumeController)->SetVolumeLevel(slSources[i].VolumeController, 0.0f);
      (*slSources[i].VolumeController)->SetStereoPosition(slSources[i].VolumeController, 0.0f);
   }
}

void GEAudioOpenSL::release()
{
   (*slOutputMix)->Destroy(slOutputMix);
	(*slEngineObject)->Destroy(slEngineObject);
}

void GEAudioOpenSL::loadSound(unsigned int SoundIndex, const char* SoundFile)
{
   if(SoundIndex >= BUFFERS)
      return;
   
   char sFileName[256];
   unsigned char* pFileData;
   unsigned int iFileLenght;

   sprintf(sFileName, "Sounds/%s", SoundFile);
   iFileLenght = GEDevice::getFileLength(sFileName);
   pFileData = new unsigned char[iFileLenght];
   GEDevice::readFile(sFileName, pFileData, iFileLenght);

   loadPcmAudioData(SoundIndex, (const char*)pFileData, iFileLenght);
   delete[] pFileData;
}

void GEAudioOpenSL::unloadSound(unsigned int SoundIndex)
{
   if(slBuffers[SoundIndex].AudioData)
   {
      delete[] slBuffers[SoundIndex].AudioData;
      slBuffers[SoundIndex].AudioData = NULL;
   }
}

void GEAudioOpenSL::unloadAllSounds()
{
   for(unsigned int i = 0; i < BUFFERS; i++)
      unloadSound(i);
}

int GEAudioOpenSL::assignSource(unsigned int EntityID)
{
   return 0;
}

void GEAudioOpenSL::releaseSource(unsigned int SourceIndex)
{

}

void GEAudioOpenSL::cleanSources()
{

}

void GEAudioOpenSL::playSound(unsigned int SoundIndex, unsigned int SourceIndex)
{
   (*slSources[SourceIndex].BufferQueue)->Clear(slSources[SourceIndex].BufferQueue);
	(*slSources[SourceIndex].BufferQueue)->Enqueue(slSources[SourceIndex].BufferQueue, slBuffers[SoundIndex].AudioData, slBuffers[SoundIndex].AudioDataSize);
	(*slSources[SourceIndex].PlaybackState)->SetPlayState(slSources[SourceIndex].PlaybackState, SL_PLAYSTATE_PLAYING);
}

void GEAudioOpenSL::stop(unsigned int SourceIndex)
{

}

bool GEAudioOpenSL::isPlaying(unsigned int SourceIndex)
{
   return false;
}

void GEAudioOpenSL::moveListener(const GEVector3& Delta)
{

}

void GEAudioOpenSL::moveSource(unsigned int SourceIndex, const GEVector3& Delta)
{

}

unsigned int GEAudioOpenSL::getEntityID(unsigned int SourceIndex)
{
   return 0;
}

void GEAudioOpenSL::setListenerPosition(const GEVector3& Position)
{

}

void GEAudioOpenSL::setVolume(unsigned int SourceIndex, float Volume)
{
   (*slSources[SourceIndex].VolumeController)->SetVolumeLevel(slSources[SourceIndex].VolumeController, linearToMillibel(Volume));
}

void GEAudioOpenSL::setPosition(unsigned int SourceIndex, const GEVector3& Position)
{
   //(*slSources[SourceIndex].VolumeController)->SetStereoPosition(slSources[SourceIndex].VolumeController, floatToPermille(Pan));
}

void GEAudioOpenSL::setDirection(unsigned int SourceIndex, const GEVector3& Direction)
{

}

SLmillibel GEAudioOpenSL::linearToMillibel(float fGain)
{
	if(fGain >= 1.0f)
		return 0;

	if(fGain <= 0.0f)
		return SL_MILLIBEL_MIN;

	return (SLmillibel)(2000 * log10(fGain));
}

SLpermille GEAudioOpenSL::floatToPermille(float fPanning)
{
	return (SLpermille)(1000 * fPanning);
}

void GEAudioOpenSL::loadPcmAudioData(unsigned int iSoundIndex, const char* sData, unsigned int iSize)
{
	const int BUFFER_SIZE = 8;
	char sBuffer[BUFFER_SIZE];

	strncpy(sBuffer, sData, 4);
	sBuffer[4] = '\0';
	sData += 4;

	if(strcmp(sBuffer, "RIFF") != 0)
	   return;

	sData += 4;

	strncpy(sBuffer, sData, 4);
	sBuffer[4] = '\0';
	sData += 4;

	if(strcmp(sBuffer, "WAVE") != 0)
	   return;

	strncpy(sBuffer, sData, 4);
	sBuffer[4] = '\0';
	sData += 4;

	if(strcmp(sBuffer, "fmt ") != 0)
	   return;

	sData += 4;
	sData += 2;
	slBuffers[iSoundIndex].Channels = *(short*)sData;
	sData += 2;

	slBuffers[iSoundIndex].SampleRate = *(int*)sData;
	sData += 4;
	sData += 4;
	sData += 2;
	sData += 2;

	do
	{
		strncpy(sBuffer, sData, 4);
		sBuffer[4] = '\0';
		sData += 2;

	} while(strcmp(sBuffer, "data") != 0);

	sData += 2;
	slBuffers[iSoundIndex].AudioDataSize = *(int*)sData;
	sData += 4;

	if(slBuffers[iSoundIndex].AudioData)
		delete[] slBuffers[iSoundIndex].AudioData;

	slBuffers[iSoundIndex].AudioData = new char[slBuffers[iSoundIndex].AudioDataSize];
	memcpy(slBuffers[iSoundIndex].AudioData, sData, slBuffers[iSoundIndex].AudioDataSize);
}
