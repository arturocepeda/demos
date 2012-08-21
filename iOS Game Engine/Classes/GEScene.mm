
/*
   Arturo Cepeda PŽrez

	Scene Management

   --- GEScene.mm ---
*/

#include "GEScene.h"

GEScene::GEScene(GERendering* Render, GEAudio* Audio, void* GlobalData)
{
   cRender = Render;
   cAudio = Audio;
   
   int iResX;
   int iResY;
   
   // iPhone
   if(CDevice::iPhone())
   {
      iResX = 320;
      iResY = 480;
   }
   
   // iPad
   else 
   {
      iResX = 768;
      iResY = 1024;
   }
   
   cPixelToPositionX = new CLine(0, CDevice::getOrthoLeft(), iResX, CDevice::getOrthoRight());
   cPixelToPositionY = new CLine(0, CDevice::getOrthoTop(), iResY, CDevice::getOrthoBottom());
}

GEScene::~GEScene()
{
   delete cPixelToPositionX;
   delete cPixelToPositionY;
}

void GEScene::sceneChange(unsigned int iNewScene)
{
   iNextScene = (int)iNewScene;
}

int GEScene::getNextScene()
{
   return iNextScene;
}
