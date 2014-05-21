
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Device static class (Android)
//
//  --- GEDevice.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GEDevice.h"

int GEDevice::ScreenWidth = 0;
int GEDevice::ScreenHeight = 0;

int GEDevice::getScreenWidth()
{
   return ScreenWidth;
}

int GEDevice::getScreenHeight()
{
   return ScreenHeight;
}

int GEDevice::getTouchPadWidth()
{
   return ScreenWidth;
}

int GEDevice::getTouchPadHeight()
{
   return ScreenHeight;
}

float GEDevice::getAspectRatio()
{
   return (float)ScreenHeight / ScreenWidth;
}
