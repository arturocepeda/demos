
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Rendering
//
//  --- GEGraphicsDevice.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Core/GEUtils.h"

class GEGraphicsDevice : private GENonCopyable
{
public:
    GEGraphicsDevice();
    virtual ~GEGraphicsDevice();

    virtual void* getNativePointer() = 0;
};
