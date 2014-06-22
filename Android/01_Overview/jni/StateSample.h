
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

#define BOUNDS_TOP      (GEDevice::getAspectRatio() * 0.89f)
#define BOUNDS_BOTTOM   (-GEDevice::getAspectRatio() * 0.89f)
#define BOUNDS_LEFT     (-1.0f * 0.83f)
#define BOUNDS_RIGHT    (1.0f * 0.83f)

#define FINGERS      10
#define TOUCH_SCALE  0.002f
#define ACC_SCALE    0.006f
#define STOPPED      0.004f
#define BOUNCE       0.55f
#define ROTATION     2.8f

class GEStateSample : public GEState
{
private:
   GECamera* cCamera;
   
   GEMesh* cMeshBanana;
   GEMesh* cMeshCube;
   
   float fMeshCubeR;
   float fMeshCubeRInc;
   float fMeshCubeG;
   float fMeshCubeGInc;
   float fMeshCubeB;
   float fMeshCubeBInc;
   
   GESprite* cSpriteBackground;
   GESprite* cSpriteBall;
   GESprite* cSpriteInfo[FINGERS];
   
   GELabel* cText;
   
   GEVector3 vBallPosition;
   GEVector3 vBallVelocity;
   
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
   GEStateSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void internalInit();
   void release();
   
   void update(float DeltaTime);
   void render();
   
   void inputTouchBegin(int ID, const GEVector2& Point);
   void inputTouchMove(int ID, const GEVector2& PreviousPoint, const GEVector2& CurrentPoint);
   void inputTouchEnd(int ID, const GEVector2& Point);
   
   void updateAccelerometerStatus(const GEVector3& Status);
};

#endif
