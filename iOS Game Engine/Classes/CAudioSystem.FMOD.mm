//
//  CAudioSystem.m
//  GameEngine
//
//  Created by Arturo Cepeda on 4/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CAudioSystem.FMOD.h"
#include <iostream>
#include <fstream>

CAudioSystem::CAudioSystem()
{
   int i;

   for(i = 0; i < CAUDIOSYSTEM_CHANNELS; i++)
      fmodChannel[i] = NULL;

   for(i = 0; i < CAUDIOSYSTEM_SOUNDS; i++)
      fmodSound[i] = NULL;

   FMOD::System_Create(&fmodSystem);
   fmodSystem->init(CAUDIOSYSTEM_CHANNELS, FMOD_INIT_NORMAL, 0);
}

CAudioSystem::~CAudioSystem()
{
   int i;
   
   for(i = 0; i < CAUDIOSYSTEM_CHANNELS; i++)
   {
      if(fmodChannel[i])
         fmodChannel[i]->stop();
   }

   for(i = 0; i < CAUDIOSYSTEM_SOUNDS; i++)
   {
      if(fmodSound[i])
         fmodSound[i]->release();
   }

   fmodSystem->close();
   fmodSystem->release();
}

void CAudioSystem::loadSound(unsigned int SoundIndex, NSString *SoundFile)
{
   if(SoundIndex >= CAUDIOSYSTEM_SOUNDS)
      return;

   NSString *sSoundFilePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: SoundFile];
   const char *sSoundFile = [sSoundFilePath UTF8String];
   std::ifstream iFile(sSoundFile);
   
   if(!iFile)
      return;

   if(fmodSound[SoundIndex])
      fmodSound[SoundIndex]->release();

   fmodSystem->createSound(sSoundFile, FMOD_DEFAULT, 0, &fmodSound[SoundIndex]);
}

void CAudioSystem::releaseSound(unsigned int SoundIndex)
{
   if(SoundIndex >= CAUDIOSYSTEM_SOUNDS)
      return;
      
   if(fmodSound[SoundIndex])
      fmodSound[SoundIndex]->release();
}

void CAudioSystem::playSound(unsigned int Sound, unsigned int Channel)
{
   if(Sound >= CAUDIOSYSTEM_SOUNDS || Channel >= CAUDIOSYSTEM_CHANNELS || fmodSound[Sound] == NULL)
      return;
      
   fmodSystem->playSound(FMOD_CHANNEL_FREE, fmodSound[Sound], false, &fmodChannel[Channel]);
}

void CAudioSystem::stopSound(unsigned int Channel)
{
   if(Channel >= CAUDIOSYSTEM_CHANNELS || fmodChannel[Channel] == NULL)
      return;
      
   fmodChannel[Channel]->stop();
   fmodChannel[Channel] = NULL;
}

void CAudioSystem::setVolume(unsigned int Channel, float Volume)
{
   if(Channel >= CAUDIOSYSTEM_CHANNELS || fmodChannel[Channel] == NULL)
      return;
      
   fmodChannel[Channel]->setVolume(Volume);
}

void CAudioSystem::setPan(unsigned int Channel, float Pan)
{
   if(Channel >= CAUDIOSYSTEM_CHANNELS || fmodChannel[Channel] == NULL)
      return;
      
   fmodChannel[Channel]->setPan(Pan);
}
