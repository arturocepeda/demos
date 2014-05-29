
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Sample application
//
//  --- SceneSample.h ---
//
//////////////////////////////////////////////////////////////////


#ifndef _SCENESAMPLE_H_
#define _SCENESAMPLE_H_

#include "Scenes/GEScene.h"

#define NEAR  0.05f

class GESceneSample : public GEScene
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
   GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void internalInit();
   void release();
   void update();
   void render();
   
   void inputTouchBegin(int ID, const GEVector2& Point);
   void inputTouchMove(int ID, const GEVector2& PreviousPoint, const GEVector2& CurrentPoint);
   void inputTouchEnd(int ID, const GEVector2& Point);
};

#endif
