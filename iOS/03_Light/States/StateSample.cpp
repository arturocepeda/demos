
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Sample application
//
//  --- StateSample.cpp ---
//
//////////////////////////////////////////////////////////////////


#include "StateSample.h"
#include "Core/GEUtils.h"
#include "Core/GEConstants.h"

#include "banana.h"

GEStateSample::GEStateSample(GERendering* Render, GEAudio* Audio, void* GlobalData)
   : GEState(Render, Audio, GlobalData)
{
}

void GEStateSample::internalInit()
{
   cRender->setBackgroundColor(GEColor(0.1f, 0.1f, 0.2f));
   
   // lighting
   cRender->setAmbientLightColor(GEColor(1.0f, 1.0f, 1.0f));
   cRender->setAmbientLightIntensity(0.4f);

   cRender->setNumberOfActiveLights(1);
   cRender->setLightPosition((unsigned int)GELights::PointLight1, GEVector3(0.0f, 0.0f, 1.0f));
   cRender->setLightColor((unsigned int)GELights::PointLight1, GEColor(1.0f, 1.0f, 1.0f));
   cRender->setLightIntensity((unsigned int)GELights::PointLight1, 0.6f);

   // cameras
   cCamera = new GECamera();
   cCamera->setPosition(0.0f, 0.0f, -4.0f);

   // textures
   cRender->loadTexture(Textures.Banana, "banana", "jpg");
   cRender->loadTexture(Textures.Bulb, "bulb", "png");
   
   // mesh
   cRender->createMesh(&cMeshBanana);
   cMeshBanana->loadFromArrays(bananaNumVerts, bananaVerts, bananaNormals, bananaTexCoords);
   cMeshBanana->setTexture(cRender->getTexture(Textures.Banana));
   cMeshBanana->scale(2.5f, 2.5f, 2.5f);
   
   // sprite
   cRender->createSprite(&cSpriteBulb);
   cSpriteBulb->setTexture(cRender->getTexture(Textures.Bulb));
   cSpriteBulb->setSize(GEVector2(0.5f, 0.5f));
   cSpriteBulb->setOpacity(0.75f);
   
   vBulbPosition.set(0.0f, 0.0f, 0.0f);
   bMoveBulb = false;
}

void GEStateSample::update(float DeltaTime)
{
   float fRotation = -DeltaTime * 0.001f;
   cMeshBanana->rotate(fRotation, fRotation, fRotation);
}

void GEStateSample::render()
{
   // camera
   cRender->set3D();
   cRender->useCamera(cCamera);

   // mesh
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::MeshTexture);
   cRender->renderMesh(cMeshBanana);
   
   // sprite
   cRender->set2D();
   cRender->useShaderProgram((unsigned int)GEShaderPrograms::HUD);
   cRender->renderSprite(cSpriteBulb);
}

void GEStateSample::release()
{
   // release objects
   delete cCamera;
   delete cMeshBanana;
   delete cSpriteBulb;
}     

void GEStateSample::inputTouchBegin(int ID, const GEVector2& Point)
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

void GEStateSample::inputTouchMove(int ID, const GEVector2& PreviousPoint, const GEVector2& CurrentPoint)
{
   if(!bMoveBulb || ID > 0)
      return;
   
   GEVector2 vScreen = cRender->pixelToScreen(CurrentPoint);   
   cRender->setLightPosition((unsigned int)GELights::PointLight1, GEVector3(vScreen.X, vScreen.Y, 1.0f));
   vBulbPosition.set(vScreen.X, vScreen.Y, 0.0f);
   cSpriteBulb->setPosition(vBulbPosition);
}

void GEStateSample::inputTouchEnd(int ID, const GEVector2& Point)
{
   if(ID == 0)
      bMoveBulb = false;
}
