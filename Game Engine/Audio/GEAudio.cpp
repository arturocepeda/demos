
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Audio
//
//  --- GEAudio.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GEAudio.h"

GEAudio::GEAudio()
{
    for(unsigned int i = 0; i < CHANNELS; i++)
        fVolume[i] = 1.0f;
}

GEAudio::~GEAudio()
{
}

void GEAudio::update()
{
}
