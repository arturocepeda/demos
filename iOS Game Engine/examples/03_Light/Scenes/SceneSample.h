
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  iOS Game Engine
//
//  Sample application
//
//  --- SceneSample.h ---
//
//////////////////////////////////////////////////////////////////


#ifndef _SCENESAMPLE_H_
#define _SCENESAMPLE_H_

#include "GEScene.h"

#define NEAR  0.05f

class GESceneSample : public GEScene
{
private:
   GECamera* cCamera;
   GEMesh* cMeshBanana;
   GESprite* cSpriteBulb;
   
   bool bMoveBulb;
   GEVector vBulbPosition;

   struct
   {
      enum {Banana, Bulb};
   } 
   Textures;
   
public:
   GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void init();
   void release();
   
	void update();
   void render();
   
   void inputTouchBegin(int ID, CGPoint* Point);
   void inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint);
   void inputTouchEnd(int ID, CGPoint* Point);
   
   void updateAccelerometerStatus(float X, float Y, float Z);
};

#endif
