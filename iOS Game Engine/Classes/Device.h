//
//  Device.h
//  GameEngine
//
//  Created by Arturo Cepeda on 20/08/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _DEVICE_H_
#define _DEVICE_H_

#define GL_ORTHO_ZNEAR    -10.0f
#define GL_ORTHO_ZFAR      10.0f

class CDevice
{
private:
   static const float fOrthoRef;
   
public:
   static bool iPhone();         // iPhone or iPad
   static bool displayRetina();  // iPad (3rd generation)
   
   static float getOrthoTop();
   static float getOrthoBottom();
   static float getOrthoLeft();
   static float getOrthoRight();
};

#endif
