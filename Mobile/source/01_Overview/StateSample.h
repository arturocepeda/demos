
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
#include "Core/GEScene.h"

#define BOUNDS_TOP      (GE::Core::Device::getAspectRatio() * 0.89f)
#define BOUNDS_BOTTOM   (-GE::Core::Device::getAspectRatio() * 0.89f)
#define BOUNDS_LEFT     (-1.0f * 0.83f)
#define BOUNDS_RIGHT    (1.0f * 0.83f)

#define FINGERS      10
#define TOUCH_SCALE  1.0f
#define ACC_SCALE    0.006f
#define STOPPED      0.004f
#define BOUNCE       0.55f
#define ROTATION     2.8f

class GEStateSample : public GE::States::State
{
private:
   GE::Core::Scene* cScene;
   GE::Core::Entity* cEntitiesInfo[FINGERS];
   
   float fMeshCubeR;
   float fMeshCubeRInc;
   float fMeshCubeG;
   float fMeshCubeGInc;
   float fMeshCubeB;
   float fMeshCubeBInc;
   
   GE::Vector3 vBallPosition;
   GE::Vector3 vBallVelocity;
   
   struct
   {
      enum { Background, Banana, Info, Basketball };
   } 
   Textures;
   
   struct
   {
      enum { Music, Touch };
   }
   Sounds;
   
   void updateText();
   void updateCube();
   void updateBall();
   
public:
   GEStateSample(void* GlobalData);
   
   void internalInit();
   void release();
   
   void update();
   
   void inputTouchBegin(int ID, const GE::Vector2& Point);
   void inputTouchMove(int ID, const GE::Vector2& PreviousPoint, const GE::Vector2& CurrentPoint);
   void inputTouchEnd(int ID, const GE::Vector2& Point);
   
   void updateAccelerometerStatus(const GE::Vector3& Status);
};

#endif
