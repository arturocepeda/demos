
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
#include "Core/GERand.h"
#include <vector>

#define BOUNDS_TOP      GE::Core::Device::getAspectRatio()
#define BOUNDS_BOTTOM  -GE::Core::Device::getAspectRatio()
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
   GE::Vector3 Position;
   GE::Vector3 Destiny;
   GE::Vector3 Direction;
   
   int Type;
   float Speed;
   float Size;
   
   float Angle;
   float Opacity;
   int CurrentStep;
};

class GEStateSample : public GE::States::State
{
private:
   GE::Rendering::Sprite* cSpriteBackground;
   GE::Rendering::Sprite* cSpriteBug[BUG_TYPES][BUG_STEPS];
   
   GE::Rendering::Label* cTextEscaped;
   GE::Rendering::Label* cTextSmashed;
   char sTextBuffer[32];
   
   int iEscaped;
   int iSmashed;
   
   // bugs
   std::vector<SBug> vBugs;
   std::vector<SBug> vBugsSmashed;
   unsigned int iCurrentFrame;
   int iProbability;

   // random generators
   GE::Core::RandEvent cRandEvent;
   GE::Core::RandInt cRandBugType;
   GE::Core::RandFloat cRandBugSize;
   GE::Core::RandFloat cRandBugSpeed;
   
   void generateBug();
   
public:
   GEStateSample(GE::Rendering::RenderSystem* Render, GE::Audio::AudioSystem* Audio, void* GlobalData);
   
   void internalInit();
   void update(float DeltaTime);
   void render();
   void release();
   
   void inputTouchBegin(int ID, const GE::Vector2& Point);
};

#endif
