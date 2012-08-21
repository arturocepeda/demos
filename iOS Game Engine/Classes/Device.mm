//
//  Device.mm
//  GameEngine
//
//  Created by Arturo Cepeda on 20/08/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Device.h"

const float CDevice::fOrthoRef = 1.0f;

bool CDevice::iPhone()
{
   return [[UIDevice currentDevice].model hasPrefix:@"iPhone"];
}

bool CDevice::displayRetina()
{
   return ([[UIScreen mainScreen] respondsToSelector:@selector(scale)] && 
           [[UIScreen mainScreen] scale] == 2.0);
}

float CDevice::getOrthoTop()
{
   if(iPhone())
      return (fOrthoRef * 1.5f);
   else
      return (fOrthoRef * 1.33f);
}

float CDevice::getOrthoBottom()
{
   if(iPhone())
      return (fOrthoRef * -1.5f);
   else
      return (fOrthoRef * -1.33f);
}

float CDevice::getOrthoLeft()
{
   return (fOrthoRef * -1.0f);
}

float CDevice::getOrthoRight()
{
   return fOrthoRef;
}
