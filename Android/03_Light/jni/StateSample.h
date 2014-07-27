
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

class GEStateSample : public GE::States::State
{
private:
   GE::Rendering::Camera* cCamera;
   GE::Rendering::Mesh* cMeshBanana;
   GE::Rendering::Sprite* cSpriteBulb;
   
   bool bMoveBulb;
   GE::Vector3 vBulbPosition;

   struct
   {
      enum {Banana, Bulb};
   } 
   Textures;
   
public:
   GEStateSample(GE::Rendering::RenderSystem* Render, GE::Audio::AudioSystem* Audio, void* GlobalData);
   
   void internalInit();
   void release();
   void update(float DeltaTime);
   void render();
   
   void inputTouchBegin(int ID, const GE::Vector2& Point);
   void inputTouchMove(int ID, const GE::Vector2& PreviousPoint, const GE::Vector2& CurrentPoint);
   void inputTouchEnd(int ID, const GE::Vector2& Point);
};

#endif
