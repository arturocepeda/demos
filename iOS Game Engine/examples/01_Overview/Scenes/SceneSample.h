
/*
   Arturo Cepeda Pérez
 
   Sample Scene
 
   --- SceneSample.h ---
 */

#ifndef _SCENESAMPLE_H_
#define _SCENESAMPLE_H_

#include "GEScene.h"

#define BOUNDS_TOP      (GEDevice::getAspectRatio() * 0.89f)
#define BOUNDS_BOTTOM   (-GEDevice::getAspectRatio() * 0.89f)
#define BOUNDS_LEFT     (-1.0f * 0.83f)
#define BOUNDS_RIGHT    (1.0f * 0.83f)

#define FINGERS      5
#define TOUCH_SCALE  0.002f
#define ACC_SCALE    0.004f
#define STOPPED      0.004f
#define BOUNCE       0.55f
#define ROTATION     280

class GESceneSample : public GEScene
{
private:
   GECamera* cCamera;
   
   GEMesh* cMeshBanana;
   GEMesh* cMeshCube;
   
   GESprite* cSpriteBackground;
   GESprite* cSpriteBall;
   GESprite* cSpriteInfo[FINGERS];
   
   GELabel* cText;
   
   GEVector vBallPosition;
   GEVector vBallVelocity;
   
   struct
   {
      enum {Background, Banana, Info, Basketball};
   } 
   Textures;
   
   struct
   {
      enum {Music, Touch};
   }
   Sounds;
   
   void updateText();
   void updateBanana();
   void updateCube();
   void updateBall();
   
public:
   GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void init();
	void update();
   void render();
	void release();
   
   void inputTouchBegin(int ID, CGPoint* Point);
   void inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint);
   void inputTouchEnd(int ID, CGPoint* Point);
   
   void updateAccelerometerStatus(float X, float Y, float Z);
};

#endif
