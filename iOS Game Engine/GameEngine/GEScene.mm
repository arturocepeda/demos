
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

   cPixelToPositionX = new CLine(0, -1.0f, GEDevice::getTouchPadSizeX(), 1.0f);   
   cPixelToPositionY = new CLine(0, GEDevice::getAspectRatio(), 
                                 GEDevice::getTouchPadSizeY(), -GEDevice::getAspectRatio());
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
