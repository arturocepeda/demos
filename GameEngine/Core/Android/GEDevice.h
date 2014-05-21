
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Device static class (Android)
//
//  --- GEDevice.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

class GEDevice
{
public:
   static int ScreenWidth;
   static int ScreenHeight;

   static int getScreenWidth();
   static int getScreenHeight();
   
   static int getTouchPadWidth();
   static int getTouchPadHeight();
   
   static float getAspectRatio();
   
   static unsigned int getFileLength(const char* Filename);
   static unsigned int readFile(const char* Filename, unsigned char* ReadBuffer, unsigned int BufferSize);
};
