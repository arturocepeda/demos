
/*
   Arturo Cepeda PŽrez
   
	Scene Management

   --- GEScene.h ---
*/

#ifndef _GESCENE_H_
#define _GESCENE_H_

#include "GERendering.h"
#include "GEAudio.h"
#include "Utils.h"

class GEScene
{
protected:
   // rendering and audio system
   GERendering* cRender;
   GEAudio* cAudio;
   
   // frame counter
   unsigned int iCurrentFrame;

   // input management
   CLine* cPixelToPositionX;
   CLine* cPixelToPositionY;
   
   // scene management
   int iNextScene;   
   void sceneChange(unsigned int iNewScene);

public:
   GEScene(GERendering* Render, GEAudio* Audio, void* GlobalData);
   ~GEScene();

   virtual void init() = 0;      // IMPORTANT: always begin with [iNextScene = -1]
	virtual void update() = 0;
	virtual void release() = 0;

   virtual void inputTouchBegin(int ID, CGPoint* Point) = 0;
   virtual void inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint) = 0;
   virtual void inputTouchEnd(int ID, CGPoint* Point) = 0;

   int getNextScene();
};

#endif
