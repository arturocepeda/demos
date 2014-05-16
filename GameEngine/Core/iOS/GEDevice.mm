
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Device static class (iOS)
//
//  --- GEDevice.mm ---
//
//////////////////////////////////////////////////////////////////

#include "GEDevice.h"

bool GEDevice::iPhone()
{
   return [[UIDevice currentDevice].model hasPrefix:@"iPhone"];
}

bool GEDevice::displayRetina()
{
   return ([[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2.0);
}

int GEDevice::getScreenWidth()
{
   return (int)([[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale]);
}

int GEDevice::getScreenHeight()
{
   return (int)([[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale]);
}

int GEDevice::getTouchPadWidth()
{
   return [[UIScreen mainScreen] bounds].size.width;
}

int GEDevice::getTouchPadHeight()
{
   return [[UIScreen mainScreen] bounds].size.height;
}

float GEDevice::getAspectRatio()
{
   return [[UIScreen mainScreen] bounds].size.height / [[UIScreen mainScreen] bounds].size.width;
}

const char* GEDevice::getResourcePath(const char* ResourceName)
{
   NSString* nsResourceName = [NSString stringWithUTF8String:ResourceName];
   NSString* nsFilePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:nsResourceName];
   return [nsFilePath UTF8String];
}
