
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Device static class (iOS)
//
//  --- GEDevice.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

class GEDevice
{
private:
   
public:
   static bool iPhone();         // iPhone or iPad
   static bool displayRetina();  // iPad (3rd generation)
   
   static int getScreenWidth();
   static int getScreenHeight();
   
   static int getTouchPadWidth();
   static int getTouchPadHeight();
   
   static float getAspectRatio();
   
   static const char* getResourcePath(const char* ResourceName);
};
