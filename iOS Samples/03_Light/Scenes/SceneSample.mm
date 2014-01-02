
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

void GESceneSample::internalInit()
{
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

void GESceneSample::inputTouchBegin(int ID, const GEVector2& Point)
{
   if(ID > 0)
      return;
   
   GEVector2 vDelta = cRender->pixelToScreen(Point);
   vDelta.X -= vBulbPosition.X;
   vDelta.X *= vDelta.X;
   vDelta.Y -= vBulbPosition.Y;
   vDelta.Y *= vDelta.Y;
   
   if((vDelta.X + vDelta.Y) < NEAR)
      bMoveBulb = true;
}

void GESceneSample::inputTouchMove(int ID, const GEVector2& PreviousPoint, const GEVector2& CurrentPoint)
{
   if(!bMoveBulb || ID > 0)
      return;
   
   GEVector2 vScreen = cRender->pixelToScreen(CurrentPoint);   
   cRender->setLightPosition(GELights.PointLight1, vScreen.X, vScreen.Y, 1.0f);
   vBulbPosition.set(vScreen.X, vScreen.Y, 0.0f);
   cSpriteBulb->setPosition(vBulbPosition);
}

void GESceneSample::inputTouchEnd(int ID, const GEVector2& Point)
{
   if(ID == 0)
      bMoveBulb = false;
}
