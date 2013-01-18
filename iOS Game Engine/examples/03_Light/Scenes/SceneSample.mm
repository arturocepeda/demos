
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  iOS Game Engine
//
//  Sample application
//
//  --- SceneSample.mm ---
//
//////////////////////////////////////////////////////////////////


#include "SceneSample.h"
#include "banana.h"
#include "GEUtils.h"

GESceneSample::GESceneSample(GERendering* Render, GEAudio* Audio, void* GlobalData) :
               GEScene(Render, Audio, GlobalData)
{
}

void GESceneSample::init()
{   
   iNextScene = -1;
   cRender->setBackgroundColor(0.1f, 0.1f, 0.2f);
   
   // lighting
   cRender->setAmbientLightColor(1.0f, 1.0f, 1.0f);
   cRender->setAmbientLightIntensity(0.4f);
   
   cRender->setNumberOfActiveLights(1);
   cRender->setLightPosition(GELights.PointLight1, 0.0f, 0.0f, 1.0f);
   cRender->setLightColor(GELights.PointLight1, 1.0f, 1.0f, 1.0f);
   cRender->setLightIntensity(GELights.PointLight1, 0.6f);
   
   // cameras
   cCamera = new GECamera();
   cCamera->setPosition(0.0f, 0.0f, -4.0f);

   // textures
   cRender->loadTexture(Textures.Banana, @"banana.jpg");
   cRender->loadTexture(Textures.Bulb, @"bulb.png");
   
   // mesh
   cMeshBanana = new GEMesh();
   cMeshBanana->loadFromHeader(bananaNumVerts, bananaVerts, bananaNormals, bananaTexCoords);
   cMeshBanana->setTexture(cRender->getTexture(Textures.Banana));
   cMeshBanana->scale(2.5f, 2.5f, 2.5f);
   
   // sprite
   cSpriteBulb = new GESprite();
   cSpriteBulb->setTexture(cRender->getTexture(Textures.Bulb));
   cSpriteBulb->rotate(0.0f, 0.0f, PI * 0.5f);
   cSpriteBulb->scale(0.2f, 0.2f);
   cSpriteBulb->setOpacity(0.75f);
   
   vBulbPosition.set(0.0f, 0.0f, 0.0f);
   bMoveBulb = false;
}

void GESceneSample::update()
{
   cMeshBanana->rotate(-0.01f, -0.01f, -0.01f);
}

void GESceneSample::render()
{
   // camera
   cRender->set3D();
   cRender->useCamera(cCamera);

   // mesh
   cRender->useProgram(GEPrograms.MeshTexture);
   cRender->renderMesh(cMeshBanana);
   
   // sprite
   cRender->set2D();
   cRender->useProgram(GEPrograms.HUD);
   cRender->renderSprite(cSpriteBulb);
}

void GESceneSample::release()
{
   // release objects
   delete cCamera;
   delete cMeshBanana;
   delete cSpriteBulb;
}     

void GESceneSample::inputTouchBegin(int ID, CGPoint* Point)
{
   if(ID > 0)
      return;
   
   float fDX = cPixelToPositionX->y(Point->x) - vBulbPosition.X;
   fDX *= fDX;
   
   float fDY = cPixelToPositionY->y(Point->y) - vBulbPosition.Y;
   fDY *= fDY;
   
   if((fDX + fDY) < NEAR)
      bMoveBulb = true;
}

void GESceneSample::inputTouchMove(int ID, CGPoint* PreviousPoint, CGPoint* CurrentPoint)
{
   if(!bMoveBulb || ID > 0)
      return;
   
   float fPosX = cPixelToPositionX->y(CurrentPoint->x);
   float fPosY = cPixelToPositionY->y(CurrentPoint->y);
   
   cRender->setLightPosition(GELights.PointLight1, fPosX, fPosY, 1.0f);   
   vBulbPosition.set(fPosX, fPosY, 0.0f);
   cSpriteBulb->setPosition(vBulbPosition);
}

void GESceneSample::inputTouchEnd(int ID, CGPoint* Point)
{
   if(ID == 0)
      bMoveBulb = false;
}

void GESceneSample::updateAccelerometerStatus(float X, float Y, float Z)
{

}
