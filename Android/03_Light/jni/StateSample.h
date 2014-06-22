
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Sample application
//
//  --- StateSample.h ---
//
//////////////////////////////////////////////////////////////////


#ifndef _STATESAMPLE_H_
#define _STATESAMPLE_H_

#include "States/GEState.h"

#define NEAR  0.05f

class GEStateSample : public GEState
{
private:
   GECamera* cCamera;
   GEMesh* cMeshBanana;
   GESprite* cSpriteBulb;
   
   bool bMoveBulb;
   GEVector3 vBulbPosition;

   struct
   {
      enum {Banana, Bulb};
   } 
   Textures;
   
public:
   GEStateSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void internalInit();
   void release();
   void update(float DeltaTime);
   void render();
   
   void inputTouchBegin(int ID, const GEVector2& Point);
   void inputTouchMove(int ID, const GEVector2& PreviousPoint, const GEVector2& CurrentPoint);
   void inputTouchEnd(int ID, const GEVector2& Point);
};

#endif
