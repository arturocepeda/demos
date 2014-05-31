
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

#include "Scenes/GEScene.h"
#include <vector>

#define BOUNDS_TOP      GEDevice::getAspectRatio()
#define BOUNDS_BOTTOM  -GEDevice::getAspectRatio()
#define BOUNDS_LEFT    -1.0f
#define BOUNDS_RIGHT    1.0f

#define PROB_CHANGE     1200

#define BUG_TYPES       3
#define BUG_STEPS       3
#define BUG_SIZE_MIN    15
#define BUG_SIZE_MAX    40
#define BUG_SPEED_MIN   20
#define BUG_SPEED_MAX   50

struct SBug
{
   GEVector3 Position;
   GEVector3 Destiny;
   GEVector3 Direction;
   
   int Type;
   float Speed;
   float Size;
   
   float Angle;
   float Opacity;
   int CurrentStep;
};

class GESceneSample : public GEScene
{
private:
   GESprite* cSpriteBackground;
   GESprite* cSpriteBug[BUG_TYPES][BUG_STEPS];
   
   GELabel* cTextEscaped;
   GELabel* cTextSmashed;
   char sTextBuffer[32];
   
   int iEscaped;
   int iSmashed;
   
   // bugs
   std::vector<SBug> vBugs;
   std::vector<SBug> vBugsSmashed;
   unsigned int iCurrentFrame;
   int iProbability;
   
   void generateBug();
   
public:
   GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData);
   
   void internalInit();
   void update(float DeltaTime);
   void render();
   void release();
   
   void inputTouchBegin(int ID, const GEVector2& Point);
};

#endif
