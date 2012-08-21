//
//  CAudioSystem.h
//  GameEngine
//
//  Created by Arturo Cepeda on 4/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _CAUDIOSYSTEM_H_
#define _CAUDIOSYSTEM_H_

#include "fmod.hpp"

#define CAUDIOSYSTEM_CHANNELS   64
#define CAUDIOSYSTEM_SOUNDS    256

class CAudioSystem
{
private:
   FMOD::System *fmodSystem;
   FMOD::Channel *fmodChannel[CAUDIOSYSTEM_CHANNELS];
   FMOD::Sound *fmodSound[CAUDIOSYSTEM_SOUNDS];

public:
   CAudioSystem();
   ~CAudioSystem();
   
   void loadSound(unsigned int SoundIndex, NSString *SoundFile, const char *Format);
   void releaseSound(unsigned int SoundIndex);
   void playSound(unsigned int SoundIndex, unsigned int Channel);
   void stopSound(unsigned int Channel);
   
   void setVolume(unsigned int Channel, float Volume);
   void setPan(unsigned int Channel, float Pan);
};

#endif
